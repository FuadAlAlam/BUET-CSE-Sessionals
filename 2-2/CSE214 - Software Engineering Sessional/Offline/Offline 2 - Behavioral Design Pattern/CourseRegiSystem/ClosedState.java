public class ClosedState implements CourseState {

    private static final ClosedState INSTANCE = new ClosedState();

    private ClosedState() {
    }

    public static ClosedState getInstance() {
        return INSTANCE;
    }

    @Override
    public CourseStatus getStatus() {
        return CourseStatus.CLOSED;
    }

    @Override
    public boolean isVisibleToStudents() {
        return true;
    }

    @Override
    public boolean canEnroll() {
        return false;
    }

    @Override
    public boolean canWaitlist() {
        return false;
    }

    @Override
    public boolean canTransitionTo(CourseStatus targetStatus) {
        switch (targetStatus) {
            case OPEN:
            case DRAFT:
            case CANCELLED:
                return true;
            default:
                return false;
        }
    }

    @Override
    public String getEnrollmentRejectionMessage(String courseCode) {
        return "Cannot enroll; course is CLOSED: " + courseCode;
    }

    @Override
    public String getWaitlistRejectionMessage(String courseCode) {
        return "Cannot waitlist; course not accepting waitlist: " + courseCode;
    }

    @Override
    public String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus) {
        return "Invalid transition from CLOSED to " + targetStatus;
    }
}
