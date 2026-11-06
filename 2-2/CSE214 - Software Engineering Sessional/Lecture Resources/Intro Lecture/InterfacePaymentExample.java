// InterfacePaymentExample.java
// Example: Program to an interface (PaymentGateway) for flexibility and testability.

interface PaymentGateway {
    void pay(double amount);
}

// Concrete implementation 1
class BkashPaymentGateway implements PaymentGateway {
    @Override
    public void pay(double amount) {
        System.out.println("Paying " + amount + " using bKash...");
    }
}

// Concrete implementation 2
class NagadPaymentGateway implements PaymentGateway {
    @Override
    public void pay(double amount) {
        System.out.println("Paying " + amount + " using Nagad...");
    }
}

// Another example: a fake gateway used only for testing
class FakePaymentGateway implements PaymentGateway {
    @Override
    public void pay(double amount) {
        System.out.println("[TEST] Pretending to pay " + amount + " (no real money moved).");
    }
}

class CheckoutService2 {
    // Depends on the interface, not a concrete class
    private final PaymentGateway paymentGateway;

    // Dependency is "injected" from outside
    public CheckoutService2(PaymentGateway paymentGateway) {
        this.paymentGateway = paymentGateway;
    }

    public void checkout(double amount) {
        paymentGateway.pay(amount);
    }
}

public class InterfacePaymentExample {
    public static void main(String[] args) {
        // Case 1: Use bKash in production
        CheckoutService2 checkoutBkash = new CheckoutService2(new BkashPaymentGateway());
        checkoutBkash.checkout(500.0);

        // Case 2: Use Nagad in production
        CheckoutService2 checkoutNagad = new CheckoutService2(new NagadPaymentGateway());
        checkoutNagad.checkout(500.0);

        // Case 3: Use a fake gateway in tests (no real network / no real money)
        CheckoutService2 testCheckout = new CheckoutService2(new FakePaymentGateway());
        testCheckout.checkout(123.45);

        // Note:
        // CheckoutService2 never changes when we add a new PaymentGateway type.
        // We only create new classes implementing PaymentGateway.
        // This is "programming to an interface, not an implementation."
    }
}
