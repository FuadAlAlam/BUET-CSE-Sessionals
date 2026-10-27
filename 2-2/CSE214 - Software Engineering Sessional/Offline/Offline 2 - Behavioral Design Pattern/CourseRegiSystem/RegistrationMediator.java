import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class RegistrationMediator {

    private final RegistrarSystem registrar;

    public RegistrationMediator(RegistrarSystem registrar) {
        this.registrar = registrar;
    }

    public RegistrarSystem getRegistrar() {
        return registrar;
    }

    public boolean enrollStudent(Student student, Course course) {
        if (student == null || course == null)
            return false;

        CourseState state = CourseStateFactory.getState(course.status);
        if (!state.isVisibleToStudents()) {
            System.out.println("Course " + course.code + " is not available for student enrollment.");
            return false;
        }

        if (course.isStudentEnrolled(student)) {
            System.out.println(student.name + " is already enrolled in " + course.code);
            return true;
        }

        if (!state.canEnroll()) {
            System.out.println(state.getEnrollmentRejectionMessage(course.code));
            return false;
        }

        if (course.getEnrolledCount() >= course.getCapacity()) {
            course.setState(FullState.getInstance());
            System.out.println(course.code + " reached capacity; status set to FULL. Try waitlisting.");
            return false;
        }

        course.addEnrolledStudent(student);
        student.addEnrolledCourseDirect(course);
        System.out.println("Enrolled: " + student.name + " in " + course.code);

        if (course.getEnrolledCount() >= course.getCapacity()) {
            course.setState(FullState.getInstance());
            System.out.println(course.code + " is now FULL.");
        }

        return true;
    }

    public boolean addToWaitlist(Student student, Course course) {
        if (student == null || course == null)
            return false;

        CourseState state = CourseStateFactory.getState(course.status);

        if (!state.isVisibleToStudents()) {
            System.out.println("Course " + course.code + " is not available.");
            return false;
        }

        if (course.isStudentEnrolled(student)) {
            System.out.println("Already enrolled; no need to waitlist: " + student.name + " for " + course.code);
            return false;
        }

        if (course.isStudentWaitlisted(student)) {
            System.out.println("Already waitlisted: " + student.name + " for " + course.code);
            return false;
        }

        if (student.isWaitlistedFor(course)) {
            System.out.println(student.name + " is already waitlisted for " + course.code);
            return false;
        }

        if (!state.canWaitlist()) {
            System.out.println(state.getWaitlistRejectionMessage(course.code));
            return false;
        }

        course.addWaitlistedStudent(student);
        student.addWaitlistCourseDirect(course);
        System.out.println("Waitlisted: " + student.name + " for " + course.code);

        return true;
    }

    public boolean dropStudent(Student student, Course course) {
        if (student == null || course == null)
            return false;

        boolean changed = false;

        if (course.isStudentEnrolled(student)) {
            course.removeEnrolledStudent(student);
            student.removeCourseDirect(course);
            System.out.println("Dropped from enrolled: " + student.name + " from " + course.code);
            changed = true;

            CourseState state = CourseStateFactory.getState(course.status);
            if (state.getStatus() != CourseStatus.CANCELLED && state.getStatus() != CourseStatus.DRAFT) {
                if (course.getEnrolledCount() < course.getCapacity()) {
                    Student promoted = course.pollWaitlist();
                    if (promoted != null) {
                        course.addEnrolledStudent(promoted);
                        promoted.addEnrolledCourseDirect(course);
                        promoted.removeWaitlistCourseDirect(course);
                        System.out.println("Promoted from waitlist: " + promoted.name + " into " + course.code);
                    }

                    if (course.status == CourseStatus.FULL && course.getEnrolledCount() < course.getCapacity()) {
                        course.setState(OpenState.getInstance());
                        System.out.println(course.code + " status changed to OPEN due to available capacity.");
                    }

                    if (course.getEnrolledCount() >= course.getCapacity()) {
                        course.setState(FullState.getInstance());
                    }
                }
            }
        } else if (course.isStudentWaitlisted(student)) {
            course.removeWaitlistedStudent(student);
            student.removeCourseDirect(course);
            System.out.println("Removed from waitlist: " + student.name + " for " + course.code);
            changed = true;
        } else {
            System.out.println(student.name + " is neither enrolled nor waitlisted for " + course.code);
        }

        return changed;
    }

    public void setCourseCapacity(Course course, int newCapacity) {
        if (newCapacity < 0)
            newCapacity = 0;
        System.out.println("Setting capacity of " + course.code + " to " + newCapacity);
        course.setCapacityDirect(newCapacity);

        CourseState state = CourseStateFactory.getState(course.status);
        if (state.getStatus() == CourseStatus.CANCELLED) {
            System.out.println("Course is CANCELLED; capacity change has no effect.");
            return;
        }

        if (course.getEnrolledCount() < newCapacity) {
            if (course.status != CourseStatus.DRAFT) {
                course.setState(OpenState.getInstance());
                System.out.println(course.code + " status changed to OPEN (capacity allows enrollment).");
            }
        } else if (course.getEnrolledCount() == newCapacity) {
            course.setState(FullState.getInstance());
            System.out.println(course.code + " status changed to FULL (at capacity).");
        } else {
            course.setState(FullState.getInstance());
            System.out.println(course.code + " over capacity; remains FULL.");
        }
    }

    public void setCourseStatus(Course course, CourseStatus newStatus) {
        if (newStatus == null)
            return;
        if (newStatus == course.status) {
            System.out.println("No change: " + course.code + " already " + course.status);
            return;
        }

        CourseState currentState = CourseStateFactory.getState(course.status);

        if (!currentState.canTransitionTo(newStatus)) {
            System.out.println(currentState.getInvalidTransitionMessage(course.code, newStatus));
            return;
        }

        if (newStatus == CourseStatus.CANCELLED) {
            cancelCourse(course);
        } else if (course.status == CourseStatus.FULL && newStatus == CourseStatus.CLOSED) {
            closeFullCourse(course, course.getCapacity());
        } else {
            CourseState newState = CourseStateFactory.getState(newStatus);
            course.setState(newState);
            System.out.println(course.code + " transitioned " + currentState.getStatus() + " -> " + newStatus);
        }
    }

    public void setCourseStatusInteractive(Course course, CourseStatus newStatus, Scanner scanner) {
        if (newStatus == null)
            return;
        if (newStatus == course.status) {
            System.out.println("No change: " + course.code + " already " + course.status);
            return;
        }

        if (course.status == CourseStatus.FULL && newStatus == CourseStatus.CLOSED) {
            if (course.getWaitlistCount() > 0) {
                System.out.println(course.code + " has " + course.getWaitlistCount() + " student(s) on waitlist.");
                System.out.print(
                        "Do you want to increase capacity before closing? (Enter new capacity, or 0 to not increase): ");
                try {
                    int newCapacity = Integer.parseInt(scanner.nextLine().trim());
                    if (newCapacity > 0) {
                        if (newCapacity > course.getCapacity()) {
                            course.setCapacityDirect(newCapacity);
                            System.out.println("Capacity increased to " + newCapacity);
                            closeFullCourse(course, newCapacity);
                        } else {
                            System.out.println("New capacity must be greater than current capacity ("
                                    + course.getCapacity() + "). No change.");
                            closeFullCourse(course, course.getCapacity());
                        }
                    } else {
                        System.out.println("No capacity increase.");
                        closeFullCourse(course, course.getCapacity());
                    }
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input. Closing without capacity increase.");
                    closeFullCourse(course, course.getCapacity());
                }
            } else {
                closeFullCourse(course, course.getCapacity());
            }
            return;
        }

        setCourseStatus(course, newStatus);
    }

    private void cancelCourse(Course course) {
        course.setState(CancelledState.getInstance());

        List<Student> enrolledCopy = course.getEnrolledStudentsCopy();
        for (Student s : enrolledCopy) {
            s.removeCourseDirect(course);
        }

        List<Student> waitlistCopy = course.getWaitlistedStudentsCopy();
        for (Student s : waitlistCopy) {
            s.removeCourseDirect(course);
        }

        course.clearAllStudents();
        System.out.println(course.code + " has been CANCELLED. All students dropped and waitlist cleared.");
    }

    private void closeFullCourse(Course course, int targetCapacity) {
        course.setState(ClosedState.getInstance());
        System.out.println(course.code + " transitioned FULL -> CLOSED");

        if (course.getWaitlistCount() > 0) {
            int availableSlots = targetCapacity - course.getEnrolledCount();
            if (availableSlots > 0) {
                Random random = new Random();
                List<Student> waitlistCopy = course.getWaitlistedStudentsCopy();
                int promotionCount = Math.min(availableSlots, waitlistCopy.size());

                System.out.println("Randomly selecting " + promotionCount + " student(s) from waitlist:");
                for (int i = 0; i < promotionCount; i++) {
                    int randomIndex = random.nextInt(waitlistCopy.size());
                    Student promoted = waitlistCopy.remove(randomIndex);
                    course.removeWaitlistedStudent(promoted);
                    course.addEnrolledStudent(promoted);
                    promoted.addEnrolledCourseDirect(course);
                    promoted.removeWaitlistCourseDirect(course);
                    System.out.println("  Randomly selected: " + promoted.name + " for " + course.code);
                }
            }
        }
    }
}
