#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <cstdlib> // For system()

using namespace std;

class User {
public:
    string username;
    string password;
    string accountNumber;
    double balance;

    User(string user, string pass) : username(user), password(pass), balance(0.0) {
        accountNumber = generateAccountNumber();
    }

    string generateAccountNumber() {
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(100000, 999999);
        return "ACC" + to_string(dist(mt));
    }
};

class BankingApp {
private:
    vector<User> users;

    void loadUsers() {
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string username, password, accountNumber;
            double balance;
            if (iss >> username >> password >> accountNumber >> balance) {
                User user(username, password);
                user.accountNumber = accountNumber;
                user.balance = balance;
                users.push_back(user);
            }
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (const auto& user : users) {
            file << user.username << " " << user.password << " " 
                 << user.accountNumber << " " << user.balance << "\n";
        }
        file.close();
    }

public:
    BankingApp() {
        loadUsers();
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void registerUser() {
        clearScreen();
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (const auto& user : users) {
            if (user.username == username) {
                cout << "Username already exists!\n";
                return;
            }
        }

        User newUser(username, password);
        users.push_back(newUser);
        saveUsers();
        cout << "Registration successful! Your account number is " << newUser.accountNumber << "\n";
    }

    User* loginUser() {
        clearScreen();
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                cout << "Login successful!\n";
                return &user;
            }
        }
        cout << "Invalid username or password!\n";
        return nullptr;
    }

    void viewAccountDetails(User* user) {
        clearScreen();
        cout << "Account Number: " << user->accountNumber << "\n";
        cout << "Current Balance: $" << user->balance << "\n";
    }

    void withdrawMoney(User* user) {
        clearScreen();
        double amount;
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        if (amount <= user->balance) {
            user->balance -= amount;
            saveUsers();
            cout << "Withdrawal successful! New balance: $" << user->balance << "\n";
        } else {
            cout << "Insufficient funds!\n";
        }
    }

    void depositMoney(User* user) {
        clearScreen();
        double amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;
        user->balance += amount;
        saveUsers();
        cout << "Deposit successful! New balance: $" << user->balance << "\n";
    }

void transferMoney(User* sender) {
    clearScreen();
    
    if (users.empty()) {
        cout << "No other users are registered yet.\n";
        return;
    }
    
    listAllUsers();  // Wyświetlenie listy użytkowników

    string receiverAccount;
    double amount;
    cout << "\nEnter recipient's account number: ";
    cin >> receiverAccount;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (amount > sender->balance) {
        cout << "Insufficient funds!\n";
        return;
    }

    User* receiver = nullptr;
    for (auto& user : users) {
        if (user.accountNumber == receiverAccount) {
            receiver = &user;
            break;
        }
    }

    if (!receiver) {
        cout << "Recipient account number not found!\n";
        return;
    }

    sender->balance -= amount;
    receiver->balance += amount;
    saveUsers();
    cout << "Transfer successful! New balance: $" << sender->balance << "\n";
}

void listAllUsers() {
    cout << "\nRegistered Users:\n";
    for (const auto& user : users) {
        cout << "Username: " << user.username 
                  << ", Account Number: " << user.accountNumber << "\n";
    }
}


    void showMenu(User* user) {
        int choice;
        do {
            clearScreen();
            cout << "\n1. View Account Details\n";
            cout << "2. Withdraw Money\n";
            cout << "3. Deposit Money\n";
            cout << "4. Transfer Money\n";
            cout << "5. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: viewAccountDetails(user); break;
                case 2: withdrawMoney(user); break;
                case 3: depositMoney(user); break;
                case 4: transferMoney(user); break;
                case 5: cout << "Logging out...\n"; break;
                default: cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 5);
    }
};

int main() {
    BankingApp app;
    int option;
    User* loggedInUser = nullptr;

    do {
        if (!loggedInUser) {
            app.clearScreen();
            cout << "\n1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> option;

            switch (option) {
                case 1: 
                    app.registerUser(); 
                    break;
                case 2: 
                    loggedInUser = app.loginUser();
                    if (loggedInUser) {
                        app.showMenu(loggedInUser);
                    }
                    break;
                case 3: 
                    cout << "Exiting...\n"; 
                    break;
                default: 
                    cout << "Invalid choice! Please try again.\n";
            }
        } else {
            app.showMenu(loggedInUser);
        }
    } while (option != 3);

    return 0;
}
