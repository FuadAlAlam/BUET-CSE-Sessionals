import java.util.HashMap;
import java.util.Map;
import java.util.Collection;

public class RegistrarSystem {
    private final Map<String, Student> students = new HashMap<>();
    private final Map<String, Course> courses = new HashMap<>();

    public void addStudent(Student s) {
        if (s != null) students.put(s.id, s);
    }

    public void addCourse(Course c) {
        if (c != null) courses.put(c.code, c);
    }

    public Student getStudent(String id) {
        return students.get(id);
    }

    public Course getCourse(String code) {
        return courses.get(code);
    }

    public Collection<Student> getAllStudents() {
        return students.values();
    }

    public Collection<Course> getAllCourses() {
        return courses.values();
    }
}
