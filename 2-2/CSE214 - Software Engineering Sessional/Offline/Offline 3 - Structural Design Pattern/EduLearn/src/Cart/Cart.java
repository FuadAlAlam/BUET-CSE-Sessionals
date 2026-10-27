package Cart;

import Education.EduComponent;

import java.util.List;

public interface Cart {
    void addItem(EduComponent item, boolean isModule);
    List<EduComponent> getItems();
    int getModuleCount();
    double getTotalDuration();
    double getTotalPrice();
    void clear();
}
