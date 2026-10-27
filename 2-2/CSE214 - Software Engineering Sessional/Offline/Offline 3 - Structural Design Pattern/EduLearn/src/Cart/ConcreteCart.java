package Cart;

import Education.EduComponent;

import java.util.ArrayList;
import java.util.List;

public class ConcreteCart implements Cart {
    private List<EduComponent> items = new ArrayList<>();
    private int moduleCount = 0;

    @Override
    public void addItem(EduComponent item, boolean isModule) {
        items.add(item);
        if (isModule) moduleCount++;
    }

    @Override
    public List<EduComponent> getItems() {
        return items;
    }

    @Override
    public int getModuleCount() {
        return moduleCount;
    }

    @Override
    public double getTotalDuration() {
        return items.stream().mapToDouble(EduComponent::getDuration).sum();
    }

    @Override
    public double getTotalPrice() {
        return items.stream().mapToDouble(EduComponent::calculatePrice).sum();
    }

    @Override
    public void clear() {
        items.clear();
        moduleCount = 0;
    }
}
