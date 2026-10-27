public class CourseStateFactory {

    public static CourseState getState(CourseStatus status) {
        switch (status) {
            case DRAFT:
                return DraftState.getInstance();
            case OPEN:
                return OpenState.getInstance();
            case FULL:
                return FullState.getInstance();
            case CLOSED:
                return ClosedState.getInstance();
            case CANCELLED:
                return CancelledState.getInstance();
            default:
                throw new IllegalArgumentException("Unknown status: " + status);
        }
    }
}
