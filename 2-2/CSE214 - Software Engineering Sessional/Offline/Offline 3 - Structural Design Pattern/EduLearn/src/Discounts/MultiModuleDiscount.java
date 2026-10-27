package Discounts;

import Education.EduComponent;

public class MultiModuleDiscount extends DiscountDecorator {
    public MultiModuleDiscount(EduComponent wrappedComponent) {
        super(wrappedComponent);
    }

    @Override
    public double calculatePrice() {
        return Math.max(0, super.calculatePrice() - 15.0);
    }

    @Override
    public void display(String indent) {
        super.display(indent);
        System.out.println(indent + "   [Discount] Multi-Module: -$15.0");
    }
}
