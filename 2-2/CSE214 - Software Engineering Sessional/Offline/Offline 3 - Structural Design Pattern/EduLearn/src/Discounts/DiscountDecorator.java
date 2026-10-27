package Discounts;

import Education.EduComponent;

public abstract class DiscountDecorator implements EduComponent {
    protected EduComponent wrappedComponent;

    public DiscountDecorator(EduComponent wrappedComponent) {
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
