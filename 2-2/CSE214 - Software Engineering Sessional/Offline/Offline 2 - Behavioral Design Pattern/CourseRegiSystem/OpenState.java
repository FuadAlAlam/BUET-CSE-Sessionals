public class OpenState implements CourseState {

    private static final OpenState INSTANCE = new OpenState();

    private OpenState() {
    }

    public static OpenState getInstance() {
        return INSTANCE;
    }

    @Override
    public CourseStatus getStatus() {
        return CourseStatus.OPEN;
    }

    @Override
    public boolean isVisibleToStudents() {
        return true;
    }

    @Override
    public boolean canEnroll() {
        return true;
    }

    @Override
    public boolean canWaitlist() {
        return false;
    }

    @Override
    public boolean canTransitionTo(CourseStatus targetStatus) {
        switch (targetStatus) {
            case DRAFT:
            case CLOSED:
            case CANCELLED:
                return true;
            default:
                return false;
        }
    }

    @Override
    public String getEnrollmentRejectionMessage(String courseCode) {
        return courseCode + " reached capacity; status set to FULL. Try waitlisting.";
    }

    @Override
    public String getWaitlistRejectionMessage(String courseCode) {
        return "Course is OPEN; try enrolling instead: " + courseCode;
    }

    @Override
    public String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus) {
        return "Invalid transition from OPEN to " + targetStatus;
    }
}
