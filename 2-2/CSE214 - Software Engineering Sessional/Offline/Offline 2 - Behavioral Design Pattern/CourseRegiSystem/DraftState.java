public class DraftState implements CourseState {

    private static final DraftState INSTANCE = new DraftState();

    private DraftState() {
    }

    public static DraftState getInstance() {
        return INSTANCE;
    }

    @Override
    public CourseStatus getStatus() {
        return CourseStatus.DRAFT;
    }

    @Override
    public boolean isVisibleToStudents() {
        return false;
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
            case CLOSED:
            case CANCELLED:
                return true;
            default:
                return false;
        }
    }

    @Override
    public String getEnrollmentRejectionMessage(String courseCode) {
        return "Cannot enroll; course is DRAFT (not visible): " + courseCode;
    }

    @Override
    public String getWaitlistRejectionMessage(String courseCode) {
        return "Cannot waitlist; course not accepting waitlist: " + courseCode;
    }

    @Override
    public String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus) {
        return "Invalid transition from DRAFT to " + targetStatus;
    }
}
