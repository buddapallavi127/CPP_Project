#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <string>

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
    string username, password;
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
            cout << "mobile_no: ";
            getline(cin, mobile_no);
            cout << "Password: ";
            getline(cin, password);

            pstmt = con1->prepareStatement("SELECT * FROM users WHERE mobile_no = ? AND password = ?");
            pstmt->setString(1, mobile_no);
            pstmt->setString(2, password);
            res = pstmt->executeQuery();

            if (res->next()) {
                cout << "Sign in successful!" << endl;
            } else {
                cout << "Invalid credentials!" << endl;
            }
        } else if (choice == 2) {
            cout << "Email: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);

            pstmt = con1->prepareStatement("SELECT * FROM users WHERE emailid = ? AND password = ?");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            res = pstmt->executeQuery();

            if (res->next()) {
                cout << "Sign in successful!" << endl;
            } else {
                cout << "Invalid credentials!" << endl;
            }
        } else {
            cout << "Check the options correctly" << endl;
        }
    } catch (SQLException &e) {
        cerr << "SQLException: " << e.what() << endl;
    }

    delete res; // Clean up
    delete pstmt; // Clean up
}

void login::new_customer(Connection *con1) {
    string username, emailid, password,mobile_no;

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

    try {
        pstmt = con1->prepareStatement("INSERT INTO users (username, emailid,mobile_no,password) VALUES (?, ?,?, ?)");
        pstmt->setString(1, username);
        pstmt->setString(2, emailid);
        pstmt->setString(3, mobile_no);
        pstmt->setString(4, password);
        pstmt->executeUpdate();

        cout << "Account created successfully!" << endl;
    } catch (SQLException &e) {
        cerr << "SQLException: " << e.what() << endl;
    }

    delete pstmt; // Clean up
}

void login::skip_signin(Connection *con1) {
    cout << "You have chosen to skip signing in." << endl;
    // Additional functionality can be added here if needed.
}

int main() {
    MySQL_Driver *driver;
    Connection *con;

    try {
        driver = get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "buddapallavi127"); // Change to your credentials
        con->setSchema("project");

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
    return 0;
}