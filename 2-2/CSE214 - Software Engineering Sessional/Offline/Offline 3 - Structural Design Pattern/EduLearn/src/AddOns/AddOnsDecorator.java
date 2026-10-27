package AddOns;

import Education.EduComponent;

public abstract class AddOnsDecorator implements EduComponent {
    protected EduComponent wrappedComponent;

    public AddOnsDecorator(EduComponent wrappedComponent) {
        this.wrappedComponent = wrappedComponent;
    }

    @Override
    public double calculatePrice() {
        return wrappedComponent.calculatePrice();
    }

    @Override
    public double getDuration() {
        return wrappedComponent.getDuration();
    }

    @Override
    public void display(String indent) {
        wrappedComponent.display(indent);
    }
}
