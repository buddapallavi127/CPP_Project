#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Item class to represent a product
class Item {
private:
    string name;
    double price;

public:
    Item(string n, double p) : name(n), price(p) {}

    string getName() const { return name; }
    double getPrice() const { return price; }
};

// CartItem class to hold an item along with its quantity in the cart
class CartItem {
private:
    Item item;
    int quantity;

public:
    CartItem(Item i, int q) : item(i), quantity(q) {}

    Item getItem() const { return item; }
    int getQuantity() const { return quantity; }
    void setQuantity(int q) { quantity = q; }
    double totalPrice() const { return item.getPrice() * quantity; }
};

// ShoppingCart class to handle cart operations
class ShoppingCart {
private:
    vector<CartItem> cartItems;

public:
    // Add an item to the cart
    void addItem(const Item& item, int quantity) {
        for (auto& cartItem : cartItems) {
            if (cartItem.getItem().getName() == item.getName()) {
                cartItem.setQuantity(cartItem.getQuantity() + quantity);
                return;
            }
        }
        cartItems.emplace_back(item, quantity);
    }

    // Display the items in the cart
    void viewCart() const {
        if (cartItems.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }

        cout << fixed << setprecision(2);
        cout << "\n--- Your Cart ---\n";
        cout << left << setw(20) << "Item Name" << setw(10) << "Price" << setw(10) << "Quantity" << "Total\n";
        for (const auto& cartItem : cartItems) {
            cout << left << setw(20) << cartItem.getItem().getName()
                 << "$" << setw(10) << cartItem.getItem().getPrice()
                 << setw(10) << cartItem.getQuantity()
                 << "$" << cartItem.totalPrice() << endl;
        }
    }

    // Calculate the total cost of items in the cart
    double calculateTotal() const {
        double total = 0;
        for (const auto& cartItem : cartItems) {
            total += cartItem.totalPrice();
        }
        return total;
    }
};

// Display available items for selection
void displayItems(const vector<Item>& items) {
    cout << "\n--- Available Products ---\n";
    cout << left << setw(5) << "No." << setw(20) << "Item Name" << "Price\n";
    for (size_t i = 0; i < items.size(); ++i) {
        cout << left << setw(5) << (i + 1) << setw(20) << items[i].getName() << "$" << items[i].getPrice() << endl;
    }
}

int main() {
    // Predefined list of items available in the store
    vector<Item> availableItems;
    availableItems.emplace_back("Apple", 0.99);
    availableItems.emplace_back("Banana", 0.59);
    availableItems.emplace_back("Orange", 1.29);
    availableItems.emplace_back("Milk", 1.99);
    availableItems.emplace_back("Bread", 2.49);
    availableItems.emplace_back("Eggs", 3.99);

    ShoppingCart cart;
    int choice;

    do {
        // Simulating a simple website menu
        cout << "\n--- Simple Online Shopping ---\n";
        cout << "1. View available products\n";
        cout << "2. Add product to cart\n";
        cout << "3. View cart\n";
        cout << "4. Checkout\n";
        cout << "0. Exit\n";
        cout << "Please choose an option: ";
        cin >> choice;

        if (choice == 1) {
            // Display available items to the user
            displayItems(availableItems);
        }
        else if (choice == 2) {
            // Allow user to add a product to the cart
            displayItems(availableItems);
            int itemNumber, quantity;
            cout << "Enter the product number to add to cart: ";
            cin >> itemNumber;
            cout << "Enter quantity: ";
            cin >> quantity;

            if (itemNumber > 0 && itemNumber <= availableItems.size() && quantity > 0) {
                cart.addItem(availableItems[itemNumber - 1], quantity);
                cout << "Product added to your cart!\n";
            } else {
                cout << "Invalid product number or quantity.\n";
            }
        }
        else if (choice == 3) {
            // View the items in the cart
            cart.viewCart();
        }
        else if (choice == 4) {
            // Checkout and calculate total
            cout << fixed << setprecision(2);
            cout << "Total amount: $" << cart.calculateTotal() << endl;
            cout << "Thank you for shopping with us!\n";
            break;
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    cout << "Exiting the shopping site. Goodbye!\n";
    return 0;
}