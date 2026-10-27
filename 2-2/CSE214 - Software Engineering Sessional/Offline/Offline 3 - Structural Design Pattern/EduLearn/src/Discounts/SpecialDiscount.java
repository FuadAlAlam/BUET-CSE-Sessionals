package Discounts;

import Education.EduComponent;

public class SpecialDiscount extends DiscountDecorator {
    public SpecialDiscount(EduComponent wrappedComponent) {
        super(wrappedComponent);
    }

    @Override
    public double calculatePrice() {
        return Math.max(0, super.calculatePrice() - 12.0);
    }

    @Override
    public void display(String indent) {
        super.display(indent);
        System.out.println(indent + "   [Discount] Special (5+ hours): -$12.0");
    }
}
