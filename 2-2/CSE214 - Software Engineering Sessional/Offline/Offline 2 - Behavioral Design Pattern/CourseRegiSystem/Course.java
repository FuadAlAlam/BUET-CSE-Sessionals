import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class Course {
    public final String code;
    public final String title;
    private int capacity;
    public CourseStatus status;
    private CourseState state;
    private final List<Student> enrolled = new ArrayList<>();
    private final LinkedList<Student> waitlist = new LinkedList<>();

    public Course(String code, String title, int capacity, CourseStatus status) {
        this.code = code;
        this.title = title;
        this.capacity = Math.max(0, capacity);
        this.status = status;
        this.state = CourseStateFactory.getState(status);
    }

    public void setState(CourseState newState) {
        this.state = newState;
        this.status = newState.getStatus();
    }

    public CourseState getState() {
        return state;
    }

    public boolean isVisibleToStudents() {
        return state.isVisibleToStudents();
    }

    public boolean isStudentEnrolled(Student s) {
        return enrolled.contains(s);
    }

    public boolean isStudentWaitlisted(Student s) {
        return waitlist.contains(s);
    }

    public void addEnrolledStudent(Student s) {
        if (!enrolled.contains(s)) {
            enrolled.add(s);
        }
    }

    public void removeEnrolledStudent(Student s) {
        enrolled.remove(s);
    }

    public void addWaitlistedStudent(Student s) {
        if (!waitlist.contains(s)) {
            waitlist.add(s);
        }
    }

    public void removeWaitlistedStudent(Student s) {
        waitlist.remove(s);
    }

    public Student pollWaitlist() {
        return waitlist.poll();
    }

    public void setCapacityDirect(int newCapacity) {
        this.capacity = Math.max(0, newCapacity);
    }

    public List<Student> getEnrolledStudentsCopy() {
        return new ArrayList<>(enrolled);
    }

    public List<Student> getWaitlistedStudentsCopy() {
        return new ArrayList<>(waitlist);
    }

    public void clearAllStudents() {
        enrolled.clear();
        waitlist.clear();
    }

    public boolean tryEnroll(Student s) {
        if (s == null)
            return false;

        state = CourseStateFactory.getState(status);

        if (!state.canEnroll()) {
            System.out.println(state.getEnrollmentRejectionMessage(code));
            return false;
        }

        if (enrolled.contains(s)) {
            System.out.println("Already enrolled: " + s.name + " in " + code);
            return true; // no-op
        }

        if (enrolled.size() < capacity) {
            enrolled.add(s);
            s.addEnrolledCourseDirect(this);
            System.out.println("Enrolled: " + s.name + " in " + code);
            if (enrolled.size() >= capacity) {
                setState(FullState.getInstance());
                System.out.println(code + " is now FULL.");
            }
            return true;
        } else {
            // capacity reached while OPEN -> FULL and suggest waitlist
            setState(FullState.getInstance());
            System.out.println(code + " reached capacity; status set to FULL. Try waitlisting.");
            return false;
        }
    }

    public boolean addToWaitlist(Student s) {
        if (s == null)
            return false;

        state = CourseStateFactory.getState(status);

        if (!state.canWaitlist()) {
            System.out.println(state.getWaitlistRejectionMessage(code));
            return false;
        }

        if (enrolled.contains(s)) {
            System.out.println("Already enrolled; no need to waitlist: " + s.name + " for " + code);
            return false;
        }
        if (waitlist.contains(s)) {
            System.out.println("Already waitlisted: " + s.name + " for " + code);
            return false;
        }

        waitlist.add(s);
        s.addWaitlistCourseDirect(this);
        System.out.println("Waitlisted: " + s.name + " for " + code);
        return true;
    }

    public boolean dropStudent(Student s) {
        if (s == null)
            return false;
        boolean changed = false;

        if (enrolled.contains(s)) {
            enrolled.remove(s);
            s.removeCourseDirect(this);
            System.out.println("Dropped from enrolled: " + s.name + " from " + code);
            changed = true;

            // After drop: maybe promote one from waitlist if space
            state = CourseStateFactory.getState(status);

            if (state.getStatus() != CourseStatus.CANCELLED && state.getStatus() != CourseStatus.DRAFT) {
                if (enrolled.size() < capacity) {
                    if (!waitlist.isEmpty()) {
                        Student promoted = waitlist.poll();
                        if (promoted != null) {
                            enrolled.add(promoted);
                            promoted.addEnrolledCourseDirect(this);
                            promoted.removeWaitlistCourseDirect(this);
                            System.out.println("Promoted from waitlist: " + promoted.name + " into " + code);
                        }
                    }
                    // Recalculate status: FULL -> OPEN if space
                    if (status == CourseStatus.FULL && enrolled.size() < capacity) {
                        setState(OpenState.getInstance());
                        System.out.println(code + " status changed to OPEN due to available capacity.");
                    }
                    if (enrolled.size() >= capacity) {
                        setState(FullState.getInstance());
                    }
                }
            }
        } else if (waitlist.contains(s)) {
            waitlist.remove(s);
            s.removeCourseDirect(this);
            System.out.println("Removed from waitlist: " + s.name + " for " + code);
            changed = true;
        } else {
            System.out.println(s.name + " is neither enrolled nor waitlisted for " + code);
        }
        return changed;
    }

    public void setCapacity(int newCapacity) {
        if (newCapacity < 0)
            newCapacity = 0;
        System.out.println("Setting capacity of " + code + " to " + newCapacity);
        this.capacity = newCapacity;

        state = CourseStateFactory.getState(status);

        if (state.getStatus() == CourseStatus.CANCELLED) {
            System.out.println("Course is CANCELLED; capacity change has no effect.");
            return;
        }

        // Adjust status based on capacity and current enrolled
        if (enrolled.size() < capacity) {
            if (status != CourseStatus.DRAFT) {
                setState(OpenState.getInstance());
                System.out.println(code + " status changed to OPEN (capacity allows enrollment).");
            }
        } else if (enrolled.size() == capacity) {
            setState(FullState.getInstance());
            System.out.println(code + " status changed to FULL (at capacity).");
        } else {
            // more enrolled than capacity: keep FULL, admin must resolve
            setState(FullState.getInstance());
            System.out.println(code + " over capacity; remains FULL.");
        }
    }

    public void setStatusAdmin(CourseStatus newStatus) {
        if (newStatus == null)
            return;
        if (newStatus == status) {
            System.out.println("No change: " + code + " already " + status);
            return;
        }

        state = CourseStateFactory.getState(status);

        if (!state.canTransitionTo(newStatus)) {
            System.out.println(state.getInvalidTransitionMessage(code, newStatus));
            return;
        }

        if (newStatus == CourseStatus.CANCELLED) {
            cancelCourse();
        } else if (status == CourseStatus.FULL && newStatus == CourseStatus.CLOSED) {
            closeWithRandomWaitlistSelection(capacity);
        } else {
            CourseStatus oldStatus = status;
            setState(CourseStateFactory.getState(newStatus));
            if (oldStatus == CourseStatus.CANCELLED && newStatus == CourseStatus.DRAFT) {
                System.out.println(code + " transitioned " + oldStatus + " -> " + newStatus + " (reinstating course)");
            } else {
                System.out.println(code + " transitioned " + oldStatus + " -> " + newStatus);
            }
        }
    }

    public void setStatusAdminInteractive(CourseStatus newStatus, Scanner scanner) {
        if (newStatus == null)
            return;
        if (newStatus == status) {
            System.out.println("No change: " + code + " already " + status);
            return;
        }

        // Special handling for FULL -> CLOSED with interactive prompt
        if (status == CourseStatus.FULL && newStatus == CourseStatus.CLOSED) {
            if (!waitlist.isEmpty()) {
                System.out.println(code + " has " + waitlist.size() + " student(s) on waitlist.");
                System.out.print(
                        "Do you want to increase capacity before closing? (Enter new capacity, or 0 to not increase): ");
                try {
                    int newCapacity = Integer.parseInt(scanner.nextLine().trim());
                    if (newCapacity > 0) {
                        if (newCapacity > capacity) {
                            capacity = newCapacity;
                            System.out.println("Capacity increased to " + newCapacity);
                            closeWithRandomWaitlistSelection(newCapacity);
                        } else {
                            System.out.println("New capacity must be greater than current capacity (" + capacity
                                    + "). No change.");
                            closeWithRandomWaitlistSelection(capacity);
                        }
                    } else {
                        System.out.println("No capacity increase.");
                        closeWithRandomWaitlistSelection(capacity);
                    }
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input. Closing without capacity increase.");
                    closeWithRandomWaitlistSelection(capacity);
                }
            } else {
                // No waitlist, just close
                closeWithRandomWaitlistSelection(capacity);
            }
            return;
        }

        // For all other transitions, use the standard method
        setStatusAdmin(newStatus);
    }

    private void cancelCourse() {
        setState(CancelledState.getInstance());
        // clear enrolled and waitlist, update students
        for (Student s : new ArrayList<>(enrolled)) {
            s.removeCourseDirect(this);
        }
        for (Student s : new ArrayList<>(waitlist)) {
            s.removeCourseDirect(this);
        }
        enrolled.clear();
        waitlist.clear();
        System.out.println(code + " has been CANCELLED. All students dropped and waitlist cleared.");
    }

    private void closeWithRandomWaitlistSelection(int targetCapacity) {
        setState(ClosedState.getInstance());
        System.out.println(code + " transitioned FULL -> CLOSED");

        // If there are waitlisted students and space available, promote random students
        if (!waitlist.isEmpty()) {
            int availableSlots = targetCapacity - enrolled.size();
            if (availableSlots > 0) {
                Random random = new Random();
                List<Student> waitlistCopy = new ArrayList<>(waitlist);
                int promotionCount = Math.min(availableSlots, waitlistCopy.size());

                System.out.println("Randomly selecting " + promotionCount + " student(s) from waitlist:");
                for (int i = 0; i < promotionCount; i++) {
                    int randomIndex = random.nextInt(waitlistCopy.size());
                    Student promoted = waitlistCopy.remove(randomIndex);
                    waitlist.remove(promoted);
                    enrolled.add(promoted);
                    promoted.addEnrolledCourseDirect(this);
                    System.out.println("  Randomly selected: " + promoted.name + " for " + code);
                }
            }
        }
    }

    public void printRoster() {
        System.out.println("Roster for " + code + " - " + title + " (" + status + ", cap=" + capacity + "):");
        if (enrolled.isEmpty()) {
            System.out.println("  [no enrolled]");
        } else {
            for (Student s : enrolled) {
                System.out.println("  " + s.id + " - " + s.name);
            }
        }
    }

    public void printWaitlist() {
        System.out.println("Waitlist for " + code + ":");
        if (waitlist.isEmpty()) {
            System.out.println("  [no waitlisted]");
        } else {
            for (Student s : waitlist) {
                System.out.println("  " + s.id + " - " + s.name);
            }
        }
    }

    // Exposed getters for UI/reporting
    public int getCapacity() {
        return capacity;
    }

    public int getEnrolledCount() {
        return enrolled.size();
    }

    public int getWaitlistCount() {
        return waitlist.size();
    }
}
