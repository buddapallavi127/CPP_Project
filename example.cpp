#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <unordered_map>
#include <ctime>
#include <iomanip>  // For formatting output in tables

using namespace std;
using namespace sql;
using namespace mysql;

class login {
    string username, emailid,mobile_no, password;
public:
    void already_customer(Connection*);
    void new_customer(Connection*);
    void skip_signin(Connection*);
};
void login::already_customer(Connection *con1) {
    cout << "Sign in using " << endl;
    cout << "1. Mobile number" << endl;
    cout << "2. Email" << endl;

    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from the input buffer

    PreparedStatement *pstmt = nullptr;
    ResultSet *res = nullptr;

    try {
        if (choice == 1) {
            cout << "Mobile No: ";
            getline(cin, mobile_no);
            
            pstmt = con1->prepareStatement("SELECT * FROM users WHERE mobile_no = ?");
            pstmt->setString(1, mobile_no);
            res = pstmt->executeQuery();

            if (!res->next()) {
                cout << "Mobile number not registered. Please create an account first.\n";
            } else {
                delete pstmt; // Cleanup
                pstmt = con1->prepareStatement("SELECT * FROM users WHERE mobile_no = ? AND password = ?");
                pstmt->setString(1, mobile_no);

                cout << "Password: ";
                getline(cin, password);
                pstmt->setString(2, password);
                res = pstmt->executeQuery();

                if (res->next()) {
                    cout << "Sign in successful!" << endl;
                } else {
                    cout << "Invalid credentials!" << endl;
                }
            }
        } else if (choice == 2) {
            cout << "Email: ";
            getline(cin, username);
            
            pstmt = con1->prepareStatement("SELECT * FROM users WHERE emailid = ?");
            pstmt->setString(1, username);
            res = pstmt->executeQuery();

            if (!res->next()) {
                cout << "Email not registered. Please create an account first.\n";
            } else {
                delete pstmt; // Cleanup
                pstmt = con1->prepareStatement("SELECT * FROM users WHERE emailid = ? AND password = ?");
                pstmt->setString(1, username);

                cout << "Password: ";
                getline(cin, password);
                pstmt->setString(2, password);
                res = pstmt->executeQuery();

                if (res->next()) {
                    cout << "Sign in successful!" << endl;
                } else {
                    cout << "Invalid credentials!" << endl;
                }
            }
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } catch (SQLException &e) {
        cerr << "SQLException: " << e.what() << endl;
    }

    delete res; // Clean up
    delete pstmt; // Clean up
}
void login::new_customer(Connection *con1) {
    cout << "Create a new account" << endl;

    cout << "Username: ";
    getline(cin, username);
    cout << "Email ID: ";
    getline(cin, emailid);
    cout << "Mobile No: ";
    getline(cin, mobile_no);
    cout << "Password: ";
    getline(cin, password);

    PreparedStatement *pstmt = nullptr;
    ResultSet *res = nullptr;

    try {
        pstmt = con1->prepareStatement("SELECT * FROM users WHERE username = ? OR emailid = ? OR mobile_no = ?");
        pstmt->setString(1, username);
        pstmt->setString(2, emailid);
        pstmt->setString(3, mobile_no);
        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "An account with this username, email ID, or mobile number already exists.\n";
        } else {
            delete pstmt; // Cleanup

            pstmt = con1->prepareStatement("INSERT INTO users (username, emailid, mobile_no, password) VALUES (?, ?, ?, ?)");
            pstmt->setString(1, username);
            pstmt->setString(2, emailid);
            pstmt->setString(3, mobile_no);
            pstmt->setString(4, password);
            pstmt->executeUpdate();

            cout << "Account created successfully!" << endl;
        }
    } catch (SQLException &e) {
        cerr << "SQLException: " << e.what() << endl;
    }

    delete res; // Clean up
    delete pstmt; // Clean up
}


void login::skip_signin(Connection *con1) {
    cout << "You have chosen to skip signing in." << endl;
    // Additional functionality can be added here if needed.
}

// Base class for product
class Product {
protected:
    int pid;
    string name;
    double price;
    int stock;

public:
    Product(int pid, const string& name, double price, int stock)
        : pid(pid), name(name), price(price), stock(stock) {}

    virtual void display() = 0; // Pure virtual function to display product details
    int getPid() const { return pid; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Function to update the stock of the product
    void updateStock(int newStock) {
        stock = newStock;
    }
};

// Groceries class derived from Product
class Groceries : public Product {
private:
    string brand;
    string category;
    string weight;
    string expiration_date;

public:
    Groceries(int pid, const string& name, double price, int stock,
              const string& brand, const string& category,
              const string& weight, const string& expiration_date)
        : Product(pid, name, price, stock), brand(brand), category(category),
          weight(weight), expiration_date(expiration_date) {}

    void display() override {
        // Print product details in tabular format with width set to 20
        cout << setw(20) << pid
             << setw(20) << name
             << setw(20) << price
             << setw(20) << stock
             << setw(20) << brand
             << setw(20) << category
             << setw(20) << weight
             << setw(20) << expiration_date
             << endl;
    }
};

// Clothes class derived from Product
class Clothes : public Product {
private:
    string size;

public:
    Clothes(int pid, const string& name, double price, int stock, const string& size)
        : Product(pid, name, price, stock), size(size) {}

    void display() override {
        // Print product details in tabular format with width set to 20
        cout << setw(20) << pid
             << setw(20) << name
             << setw(20) << price
             << setw(20) << stock
             << setw(20) << size
             << endl;
    }
};

// Electronics class derived from Product
class Electronics : public Product {
public:
    Electronics(int pid, const string& name, double price, int stock)
        : Product(pid, name, price, stock) {}

    void display() override {
        // Print product details in tabular format with width set to 20
        cout << setw(20) << pid
             << setw(20) << name
             << setw(20) << price
             << setw(20) << stock
             << endl;
    }
};

// Cart class to store items added to the cart
class Cart {
private:
    unordered_map<int, pair<Product*, int>> items; // Product ID -> <Product pointer, quantity>

public:
    void addItem(Product* product, int quantity) {
        // If the product is already in the cart, update the quantity
        if (items.find(product->getPid()) != items.end()) {
            items[product->getPid()].second += quantity;
        } else {
            items[product->getPid()] = make_pair(product, quantity);
        }
        cout << "Added " << quantity << " " << product->getName() << "(s) to the cart.\n";
    }

    void removeItem(int pid) {
        if (items.find(pid) != items.end()) {
            items.erase(pid);
            cout << "Removed item with Product ID: " << pid << " from the cart.\n";
        } else {
            cout << "Item with Product ID: " << pid << " not found in the cart.\n";
        }
    }

    void displayCart() {
        double total = 0.0;
        cout << "\nItems in your cart:\n";
        cout << setw(20) << "Product Name"
             << setw(20) << "Price"
             << setw(20) << "Quantity"
             << setw(20) << "Total\n";
        for (const auto& item : items) {
            Product* product = item.second.first;
            int quantity = item.second.second;
            double itemTotal = product->getPrice() * quantity;
            cout << setw(20) << product->getName()
                 << setw(20) << product->getPrice()
                 << setw(20) << quantity
                 << setw(20) << itemTotal
                 << endl;
            total += itemTotal;
        }
        cout << "\nTotal cart value: " << total << " INR\n\n";
    }

    bool isEmpty() {
        return items.empty();
    }

    // Getter for items to allow access outside the class
    unordered_map<int, pair<Product*, int>>& getItems() {
        return items;
    }

    // Friend function to access private items for checkout
    friend class Delivery;
};

// Delivery class to handle the checkout process
class Delivery {
private:
    string address;
    string orderDate;
    string deliveryDate;

public:
    Delivery(const string& address, const string& orderDate, const string& deliveryDate)
        : address(address), orderDate(orderDate), deliveryDate(deliveryDate) {}

    void display() {
        cout << "Delivery Address: " << address << "\n"
             << "Order Date: " << orderDate << "\n"
             << "Delivery Date: " << deliveryDate << "\n\n";
    }
};

// Database class for actual DB interaction
class Database {
private:
    sql::Connection *conn;

public:
    Database(const string& host, const string& user, const string& password, const string& dbname) {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://" + host + ":3306", user, password);
        conn->setSchema(dbname);
    }

    ~Database() {
        delete conn;
    }

    // Fetch products by category and display them
    void fetchAndDisplayProducts(const string& category) {
        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res;

        if (category == "groceries") {
            res = stmt->executeQuery("SELECT * FROM groceries");
            cout << "\nDisplaying products for category: Groceries\n";
            cout << setw(20) << "Product ID"
                 << setw(20) << "Name"
                 << setw(20) << "Price"
                 << setw(20) << "Stock"
                 << setw(20) << "Brand"
                 << setw(20) << "Category"
                 << setw(20) << "Weight"
                 << setw(20) << "Expiration Date"
                 << endl;

            while (res->next()) {
                int pid = res->getInt("pid");
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");
                string brand = res->getString("brand");
                string weight = res->getString("weight");
                string expiration_date = res->getString("expiration_date");

                Groceries g(pid, name, price, stock, brand, category, weight, expiration_date);
                g.display();
            }
        } else if (category == "clothes") {
            res = stmt->executeQuery("SELECT * FROM clothes");
            cout << "\nDisplaying products for category: Clothes\n";
            cout << setw(20) << "Product ID"
                 << setw(20) << "Name"
                 << setw(20) << "Price"
                 << setw(20) << "Stock"
                 << setw(20) << "Size"
                 << endl;

            while (res->next()) {
                int pid = res->getInt("pid");
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");
                string size = res->getString("size");

                Clothes c(pid, name, price, stock, size);
                c.display();
            }
        } else if (category == "electronics") {
            res = stmt->executeQuery("SELECT * FROM electronics");
            cout << "\nDisplaying products for category: Electronics\n";
            cout << setw(20) << "Product ID"
                 << setw(20) << "Name"
                 << setw(20) << "Price"
                 << setw(20) << "Stock"
                 << endl;

            while (res->next()) {
                int pid = res->getInt("pid");
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");

                Electronics e(pid, name, price, stock);
                e.display();
            }
        }

        delete res;
        delete stmt;
    }

    // Get product by ID from the database
    Product* getProductById(int pid, const string& category) {
        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res;
        Product* product = nullptr;

        if (category == "groceries") {
            res = stmt->executeQuery("SELECT * FROM groceries WHERE pid = " + to_string(pid));
            if (res->next()) {
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");
                string brand = res->getString("brand");
                string weight = res->getString("weight");
                string expiration_date = res->getString("expiration_date");

                product = new Groceries(pid, name, price, stock, brand, category, weight, expiration_date);
            }
        } else if (category == "clothes") {
            res = stmt->executeQuery("SELECT * FROM clothes WHERE pid = " + to_string(pid));
            if (res->next()) {
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");
                string size = res->getString("size");

                product = new Clothes(pid, name, price, stock, size);
            }
        } else if (category == "electronics") {
            res = stmt->executeQuery("SELECT * FROM electronics WHERE pid = " + to_string(pid));
            if (res->next()) {
                string name = res->getString("name");
                double price = res->getDouble("price");
                int stock = res->getInt("stock");

                product = new Electronics(pid, name, price, stock);
            }
        }

        delete res;
        delete stmt;
        return product;
    }

    // Update stock in the database
    void updateStockInDatabase(int pid, int newStock, const string& category) {
        sql::Statement *stmt = conn->createStatement();
        if (category == "groceries") {
            stmt->executeUpdate("UPDATE groceries SET stock=" + to_string(newStock) + " WHERE pid=" + to_string(pid));
        } else if (category == "clothes") {
            stmt->executeUpdate("UPDATE clothes SET stock=" + to_string(newStock) + " WHERE pid=" + to_string(pid));
        } else if (category == "electronics") {
            stmt->executeUpdate("UPDATE electronics SET stock=" + to_string(newStock) + " WHERE pid=" + to_string(pid));
        }
        delete stmt;
    }
};

// Function for checkout process
void checkout(Cart& cart, const string& address) {
    if (cart.isEmpty()) {
        cout << "Your cart is empty. Please add items to your cart before checkout.\n";
        return;
    }

    // Get the current date for the order
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string orderDate = to_string(1900 + ltm->tm_year) + "-" +
                       to_string(1 + ltm->tm_mon) + "-" +
                       to_string(ltm->tm_mday);

    // Increment the date by 7 days for delivery
    ltm->tm_mday += 7; // Add 7 days
    mktime(ltm); // Normalize the date

    string deliveryDate = to_string(1900 + ltm->tm_year) + "-" +
                          to_string(1 + ltm->tm_mon) + "-" +
                          to_string(ltm->tm_mday);

    // Create a Delivery object
    Delivery delivery(address, orderDate, deliveryDate);
    delivery.display();

    // Calculate the total cost
    double total = 0.0;
    for (const auto& item : cart.getItems()) {
        Product* product = item.second.first;
        int quantity = item.second.second;
        total += product->getPrice() * quantity;
    }
    cout << "\nTotal Order Value: " << total << " INR\n";

    // Finalize checkout
    cout << "Thank you for your order!\n";
    cart.getItems().clear();  // Empty the cart after checkout
}

int main() {
    MySQL_Driver *driver;
    Connection *con;

    try {
        driver = get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "Cutiepie@152005"); // Change to your credentials
        con->setSchema("cpp_project");

        cout << "Connected to the database successfully!" << endl << endl;

        cout << "Sign in to your account" << endl;
        cout << "View your wish list" << endl;
        cout << "Track your purchases" << endl << endl;

        cout << "1. Already a customer? Sign in" << endl;
        cout << "2. New to Amazon.in? Create an account" << endl;
        cout << "3. Skip sign in" << endl;

        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline from the input buffer

        login l;

        if (choice == 1) l.already_customer(con);
        else if (choice == 2) l.new_customer(con);
        else if (choice == 3) l.skip_signin(con);
        else cout << "Check the options correctly" << endl;

        delete con; // Clean up
    } catch (SQLException &e) {
        cerr << "SQLException: " << e.what() << endl;
        return 1;
    }
   
    try {
        // Database connection details
        string host = "localhost";
        string user = "root"; // Your MySQL username
        string password = "Cutiepie@152005"; // Your MySQL password
        string dbname = "cpp_project"; // Database name

        // Create a Cart object to store items
        Cart cart;

        // Create a Database object
        Database db(host, user, password, dbname);

        bool continueShopping = true;
        while (continueShopping) {
            int choice;
            cout << "\nMenu:\n";
            cout << "1. Groceries\n";
            cout << "2. Clothes\n";
            cout << "3. Electronics\n";
            cout << "4. View Cart\n";
            cout << "5. Checkout\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            string category;
            switch (choice) {
                case 1: category = "groceries"; break;
                case 2: category = "clothes"; break;
                case 3: category = "electronics"; break;
                case 4:
                    cart.displayCart();
                    continue;  // Stay in the loop
                case 5: {
                    cout << "Enter your delivery address: ";
                    string address;
                    cin.ignore(); // To clear the input buffer
                    getline(cin, address);
                    checkout(cart, address);
                    continueShopping = false;  // Exit the loop after checkout
                    break;
                }
                case 0:
                    continueShopping = false;  // Exit the loop
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    continue;  // Stay in the loop
            }

            if (continueShopping && choice >= 1 && choice <= 3) {
                // Fetch and display products for the selected category
                db.fetchAndDisplayProducts(category);

                // Ask user to select a product by ID
                cout << "Enter the Product ID to add to cart: ";
                int pid;
                cin >> pid;

                // Fetch product details from the database
                Product* selectedProduct = db.getProductById(pid, category);
                if (selectedProduct != nullptr) {
                    int quantity;
                    cout << "Enter the quantity to add to cart: ";
                    cin >> quantity;

                    // Check if the requested quantity is available
                    if (quantity <= selectedProduct->getStock() && quantity > 0) {
                        cart.addItem(selectedProduct, quantity);

                        // Update stock in the database
                        int newStock = selectedProduct->getStock() - quantity;
                        db.updateStockInDatabase(pid, newStock, category);
                    } else {
                        cout << "Not enough stock available.\n";
                    }
                } else {
                    cout << "Product not found.\n";
                }
            }
        }
    } catch (sql::SQLException &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
