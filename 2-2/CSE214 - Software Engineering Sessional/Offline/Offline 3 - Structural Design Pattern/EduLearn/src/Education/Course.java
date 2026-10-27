package Education;

import java.util.ArrayList;
import java.util.List;

public class Course implements EduComponent {
    private String title;
    private List<EduComponent> lessons = new ArrayList<>();

    public Course(String title) {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }

    public void addComponent(EduComponent component) {
        lessons.add(component);
    }

    @Override
    public double calculatePrice() {
        return lessons.stream().mapToDouble(EduComponent::calculatePrice).sum();
    }

    @Override
    public double getDuration() {
        return lessons.stream().mapToDouble(EduComponent::getDuration).sum();
    }

    @Override
    public void display(String indent) {
        System.out.println(indent + "+ Course: " + title
            + " (Total Duration: " + getDuration() + " hrs, Total Price: $" + calculatePrice() + ")");
        for (EduComponent lesson : lessons) {
            lesson.display(indent + "   ");
        }
    }
}
