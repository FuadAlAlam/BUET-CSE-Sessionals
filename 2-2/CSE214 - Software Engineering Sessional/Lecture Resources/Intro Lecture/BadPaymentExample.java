// BadPaymentExample.java
// Example: CheckoutService is tightly coupled to BkashPaymentGateway (no interface)

class BkashPaymentGateway {
    public void pay(double amount) {
        System.out.println("Paying " + amount + " using bKash...");
    }
}

class CheckoutService {
    // Hard-coded dependency on a concrete class
    private BkashPaymentGateway paymentGateway = new BkashPaymentGateway();

    public void checkout(double amount) {
        // What if tomorrow we want to use Nagad or a CreditCard gateway?
        // We must come here and change this class.
        paymentGateway.pay(amount);
    }
}

public class BadPaymentExample {
    public static void main(String[] args) {
        CheckoutService checkout = new CheckoutService();
        checkout.checkout(500.0);

        // LATER REQUIREMENT:
        // "We now want to support NagadPaymentGateway instead of BkashPaymentGateway."
        // -> You must MODIFY CheckoutService code (and recompile).
        //
        // LATER:
        // "We want to support BOTH bKash and Nagad, chosen at runtime."
        // -> You will start adding if/else, more fields, etc.
        //
        // This shows how tightly-coupled code becomes rigid and hard to change.
    }
}
