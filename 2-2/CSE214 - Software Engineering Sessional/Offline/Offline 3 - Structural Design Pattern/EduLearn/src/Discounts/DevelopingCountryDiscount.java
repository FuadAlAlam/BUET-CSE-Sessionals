package Discounts;

import Education.EduComponent;

public class DevelopingCountryDiscount extends DiscountDecorator {
    public DevelopingCountryDiscount(EduComponent wrappedComponent) {
        super(wrappedComponent);
    }

    @Override
    public double calculatePrice() {
        return Math.max(0, super.calculatePrice() - 10.0);
    }

    @Override
    public void display(String indent) {
        super.display(indent);
        System.out.println(indent + "   [Discount] Developing Country: -$10.0");
    }
}
