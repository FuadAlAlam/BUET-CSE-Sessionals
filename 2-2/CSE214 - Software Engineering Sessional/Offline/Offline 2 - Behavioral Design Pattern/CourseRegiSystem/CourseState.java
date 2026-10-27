public interface CourseState {
    
    CourseStatus getStatus();
    
    boolean isVisibleToStudents();
    
    boolean canEnroll();
    
    boolean canWaitlist();
    
    boolean canTransitionTo(CourseStatus targetStatus);
    
    String getEnrollmentRejectionMessage(String courseCode);
    
    String getWaitlistRejectionMessage(String courseCode);
    
    String getInvalidTransitionMessage(String courseCode, CourseStatus targetStatus);
}
