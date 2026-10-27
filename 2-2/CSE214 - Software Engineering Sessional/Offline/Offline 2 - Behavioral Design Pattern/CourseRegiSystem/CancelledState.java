public class CancelledState implements CourseState {

    private static final CancelledState INSTANCE = new CancelledState();

    private CancelledState() {
    }

    public static CancelledState getInstance() {
        return INSTANCE;
    }

    @Override
    public CourseStatus getStatus() {
        return CourseStatus.CANCELLED;
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
        return targetStatus == CourseStatus.DRAFT;
    }

    @Override
    public String getEnrollmentRejectionMessage(String courseCode) {
        return "Cannot enroll; course is CANCELLED: " + courseCode;
    }

    @Override
    public String getWaitlistRejectionMessage(String courseCode) {
        return "Cannot waitlist; course not accepting waitlist: " + courseCode;
    }

    @Override
    public String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus) {
        return "Invalid: CANCELLED can only transition to DRAFT for " + courseCode;
    }
}
