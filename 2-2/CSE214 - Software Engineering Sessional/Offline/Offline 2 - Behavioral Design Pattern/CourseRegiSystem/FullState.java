public class FullState implements CourseState {

    private static final FullState INSTANCE = new FullState();

    private FullState() {
    }

    public static FullState getInstance() {
        return INSTANCE;
    }

    @Override
    public CourseStatus getStatus() {
        return CourseStatus.FULL;
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
        return true;
    }

    @Override
    public boolean canTransitionTo(CourseStatus targetStatus) {
        switch (targetStatus) {
            case CLOSED:
            case CANCELLED:
                return true;
            default:
                return false;
        }
    }

    @Override
    public String getEnrollmentRejectionMessage(String courseCode) {
        return "Cannot enroll; course is FULL. You may waitlist: " + courseCode;
    }

    @Override
    public String getWaitlistRejectionMessage(String courseCode) {
        return "Cannot waitlist: " + courseCode;
    }

    @Override
    public String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus) {
        return "Invalid transition from FULL to " + targetStatus + " (FULL->OPEN is automatic on drop)";
    }
}
