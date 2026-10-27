package AddOns;

import Education.EduComponent;

public class LiveMentorDecorator extends AddOnsDecorator {
    public LiveMentorDecorator(EduComponent wrappedComponent) {
        super(wrappedComponent);
    }

    @Override
    public double calculatePrice() {
        return super.calculatePrice() + 20.0;
    }

    @Override
    public void display(String indent) {
        super.display(indent);
        System.out.println(indent + "   [Add-on] Live Mentor Support: +$20.0");
    }
}
