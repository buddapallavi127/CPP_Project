#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Product Class
class Product {
private:
    int id;
    string name;
    double price;

public:
    Product(int id, string name, double price) : id(id), name(name), price(price) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }

    // Overloaded << operator for printing Product details
    friend ostream& operator<<(ostream& os, const Product& product) {
        os << "Product ID: " << product.id << ", Name: " << product.name << ", Price: $" << product.price;
        return os;
    }
};

// Cart Class
class Cart {
private:
    unordered_map<int, pair<Product, int>> items; // Product ID -> (Product, Quantity)

public:
    // Add product to cart (Function Overloading)
    void addItem(const Product& product, int quantity) {
        if (items.find(product.getId()) != items.end()) {
            items[product.getId()].second += quantity; // Update quantity if product exists
        } else {
            items[product.getId()] = make_pair(product, quantity);
        }
    }

    void addItem(int productId, string productName, double productPrice, int quantity) {
        Product product(productId, productName, productPrice);
        addItem(product, quantity); // Reuse existing addItem method
    }

    // Overloaded += operator to combine two carts
    Cart& operator+=(const Cart& other) {
        for (const auto& [id, pair] : other.items) {
            addItem(pair.first, pair.second); // Add or update items from other cart
        }
        return *this;
    }

    // Display Cart Items
    void displayCart() const {
        if (items.empty()) {
            cout << "Your cart is empty!" << endl;
            return;
        }
        cout << "Cart Items:" << endl;
        for (const auto& [id, pair] : items) {
            cout << pair.first << " x " << pair.second << endl;
        }
    }
};

// Database Class
class Database {
private:
    unordered_map<int, Product> products; // Product ID -> Product

public:
    void addProduct(const Product& product) {
        products[product.getId()] = product;
    }

    Product* getProductById(int id) {
        if (products.find(id) != products.end()) {
            return &products[id];
        }
        return nullptr;
    }
};

// Main Function
int main() {
    Database db;

    // Adding products to the database
    db.addProduct(Product(1, "Apple", 0.5));
    db.addProduct(Product(2, "Milk", 1.2));
    db.addProduct(Product(3, "Bread", 2.5));

    Cart cart1, cart2;

    // Adding items to cart using function overloading
    cart1.addItem(1, "Apple", 0.5, 10); // Using overloaded method
    cart1.addItem(*db.getProductById(2), 5); // Using product object

    cart2.addItem(3, "Bread", 2.5, 2);

    // Combining carts using operator overloading
    cart1 += cart2;

    // Display cart contents
    cart1.displayCart();

    // Printing a product using overloaded << operator
    Product* product = db.getProductById(1);
    if (product) {
        cout << *product << endl; // Using << operator
    }

    return 0;
}