#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <cstdlib> // For system()

class User {
public:
    std::string username;
    std::string password;
    std::string accountNumber;
    double balance;

    User(std::string user, std::string pass) : username(user), password(pass), balance(0.0) {
        accountNumber = generateAccountNumber();
    }

    std::string generateAccountNumber() {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(100000, 999999);
        return "ACC" + std::to_string(dist(mt));
    }
};

class BankingApp {
private:
    std::vector<User> users;

    void loadUsers() {
        std::ifstream file("users.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string username, password, accountNumber;
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
        std::ofstream file("users.txt");
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
        // Clear the console screen
        #ifdef _WIN32
            system("cls"); // For Windows
        #else
            system("clear"); // For Linux and macOS
        #endif
    }

    void registerUser() {
        clearScreen();
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        for (const auto& user : users) {
            if (user.username == username) {
                std::cout << "Username already exists!\n";
                return;
            }
        }

        User newUser(username, password);
        users.push_back(newUser);
        saveUsers();
        std::cout << "Registration successful! Your account number is " << newUser.accountNumber << "\n";
    }

    User* loginUser() {
        clearScreen();
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                std::cout << "Login successful!\n";
                return &user;
            }
        }
        std::cout << "Invalid username or password!\n";
        return nullptr;
    }

    void viewAccountDetails(User* user) {
        clearScreen();
        std::cout << "Account Number: " << user->accountNumber << "\n";
        std::cout << "Current Balance: $" << user->balance << "\n";
    }

    void withdrawMoney(User* user) {
        clearScreen();
        double amount;
        std::cout << "Enter amount to withdraw: ";
        std::cin >> amount;
        if (amount <= user->balance) {
            user->balance -= amount;
            saveUsers();
            std::cout << "Withdrawal successful! New balance: $" << user->balance << "\n";
        } else {
            std::cout << "Insufficient funds!\n";
        }
    }

    void depositMoney(User* user) {
        clearScreen();
        double amount;
        std::cout << "Enter amount to deposit: ";
        std::cin >> amount;
        user->balance += amount;
        saveUsers();
        std::cout << "Deposit successful! New balance: $" << user->balance << "\n";
    }

    void showMenu(User* user) {
        int choice;
        do {
            clearScreen();
            std::cout << "\n1. View Account Details\n";
            std::cout << "2. Withdraw Money\n";
            std::cout << "3. Deposit Money\n";
            std::cout << "4. Logout\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: viewAccountDetails(user); break;
                case 2: withdrawMoney(user); break;
                case 3: depositMoney(user); break;
                case 4: std::cout << "Logging out...\n"; break;
                default: std::cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 4);
    }
};

int main() {
    BankingApp app;
    int option;
    User* loggedInUser = nullptr;

    do {
        app.clearScreen();
        if (!loggedInUser) {
            std::cout << "\n1. Register\n";
            std::cout << "2. Login\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> option;

            switch (option) {
                case 1: 
                    system("cls");
                    app.registerUser(); 
                    break;
                case 2: 
                    system("cls");
                    loggedInUser = app.loginUser();
                    if (loggedInUser) {
                        app.showMenu(loggedInUser);
                    }
                    break;
                case 3: 
                    system("cls");
                    std::cout << "Exiting...\n"; 
                    break;
                default: 
                    std::cout << "Invalid choice! Please try again.\n";
            }
        } else {
            app.showMenu(loggedInUser);
        }
    } while (option != 3);

    return 0;
}