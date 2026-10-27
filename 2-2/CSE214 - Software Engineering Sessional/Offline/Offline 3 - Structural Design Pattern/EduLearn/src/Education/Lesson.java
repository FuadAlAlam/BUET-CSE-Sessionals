package Education;

public class Lesson implements EduComponent {
    private String title;
    private double duration;
    private double price;

    public Lesson(String title, double duration, double price) {
        this.title = title;
        this.duration = duration;
        this.price = price;
    }

    @Override
    public double calculatePrice() {
        return this.price;
    }

    @Override
    public double getDuration() {
        return this.duration;
    }

    @Override
    public void display(String indent) {
        System.out.println(indent + "- Lesson: " + title
            + " (Duration: " + duration + " hrs, Price: $" + price + ")");
    }
}
