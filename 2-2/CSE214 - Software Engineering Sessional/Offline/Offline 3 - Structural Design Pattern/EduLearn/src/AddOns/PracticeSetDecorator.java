package AddOns;

import Education.EduComponent;

public class PracticeSetDecorator extends AddOnsDecorator {
    public PracticeSetDecorator(EduComponent wrappedComponent) {
        super(wrappedComponent);
    }

    @Override
    public double calculatePrice() {
        return super.calculatePrice() + 10.0;
    }

    @Override
    public void display(String indent) {
        super.display(indent);
        System.out.println(indent + "   [Add-on] Practice Question Set: +$10.0");
    }
}
