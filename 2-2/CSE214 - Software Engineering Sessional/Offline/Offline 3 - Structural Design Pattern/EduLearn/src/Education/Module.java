package Education;

import java.util.ArrayList;
import java.util.List;

public class Module implements EduComponent {
    private String title;
    private List<EduComponent> courses = new ArrayList<>();

    public Module(String title) {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }

    public void addComponent(EduComponent component) {
        courses.add(component);
    }

    @Override
    public double calculatePrice() {
        return courses.stream().mapToDouble(EduComponent::calculatePrice).sum();
    }

    @Override
    public double getDuration() {
        return courses.stream().mapToDouble(EduComponent::getDuration).sum();
    }

    @Override
    public void display(String indent) {
        System.out.println(indent + ">> Module: " + title
            + " (Total Duration: " + getDuration() + " hrs, Total Price: $" + calculatePrice() + ")");
        for (EduComponent course : courses) {
            course.display(indent + "   ");
        }
    }
}
