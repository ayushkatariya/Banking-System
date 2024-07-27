#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <random>
#include <set>
using namespace std;
void intro()
{
    system("clear");
    cout << "\n\x1b[41m                                                                                                                                    \x1b[0m";
    cout << "\n\x1b[41m                                                                                                                                    \x1b[0m";
    cout << "\n\x1b[41m                                           \x1b[1mB A N K   M A N A G E M E N T   S Y S T E M                                              \x1b[0m\x1b[0m";
    cout << "\n\x1b[41m                                                                                                                                    \x1b[0m";
    cout << "\n\x1b[41m                                                                                                                                    \x1b[0m";
    cout << "\n\n\n\n\x1b[33mMADE BY : Satam Gupta\n\nUniversity Roll No. : 2021420\n\nGraphic Era Deemed To Be University.\n\nEnter 'Y' to contiue  :  \x1b[0m";
    char y;
    cin >> y;
}
class Database
{
public:
    Database()
    {
        loadUsersFromFile();
        loadManagersFromFile();
    }

    void saveUsersToFile() // Save username and password to users.txt
    {
        ofstream userFile("users.txt");

        for (const auto &user : users)
        {
            userFile << user.first << " " << user.second.first << " " << user.second.second << endl;
        }
        userFile.close();
    }

    bool loadUsersFromFile() // verifies user with user name and password
    {
        ifstream userFile("users.txt");
        if (!userFile)
        {
            return false;
        }

        string line;
        while (getline(userFile, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                users[username].first = password;
            }
        }

        userFile.close();
        return true;
    }

    bool loadManagersFromFile() // validates the manager truthfullness
    {
        ifstream managerFile("managers.txt");
        if (!managerFile)
        {
            return false;
        }
        string line;
        while (getline(managerFile, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                managers[username] = password;
            }
        }
        managerFile.close();
        return true;
    }

    void createUser(const string &username, const string &password) // create a new account
    {
        users[username] = make_pair(password, 0.0);
        saveUsersToFile();
        cout << "\n\n\x1b[32mUser created successfully.\x1b[0m" << endl;
    }

    void deleteUser(const string &username)
    {
        users.erase(username);
        cout << "\n\n\x1b[32mDeleted Successfully....\x1b[0m";
        cout << "\n\n\x1b[33mEnter 'Y' to contiue : \x1b[33m";
        char y;
        cin >> y;
    }

    bool login() // login of a user
    {
        string username, password;
        cout << "\n\x1b[32mEnter username : \x1b[0m";
        cin >> username;

        if (users.find(username) != users.end())
        {
            cout << "\n\x1b[31mEnter password : \x1b[0m";
            cin >> password;
            if (users[username].first == password)
            {
                return true;
            }
        }
        cout << "\n\n\x1b[31mIncorrect username or password. Try again...\x1b[0m" << endl;
        return false;
    }

private:
    unordered_map<string, pair<string, double> > users;
    unordered_map<string, string> managers;
};

class Transaction
{
public:
    string type;   // Type of transaction
    double amount; // Amount transacted

    Transaction(const string &t, double a) : type(t), amount(a) {}
};
class BankAccount
{
private:
    string username;
    bool open;
    double balance;
    vector<Transaction> transactions;
    Database &database;
public:
    BankAccount(const string &username, Database &db) : username(username), open(true), balance(0), database(db)
    {
        // Read account balance from file users.txt
        ifstream file("users.txt");
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string storedUsername, storedPassword;
            double storedBalance;
            if (iss >> storedUsername >> storedPassword >> storedBalance)
            {
                if (storedUsername == username)
                {
                    balance = storedBalance;
                    break;
                }
            }
        }
    }

    void Withdraw(double amount)
    {
        if (open && amount <= balance && amount > 0)
        {
            balance -= amount;
            transactions.emplace_back("Withdrawal", amount);
            cout << "\x1b[32mSuccessfully withdrew Rs- " << amount << " from your account.\x1b[0m" << endl;
            cout << "\x1b[32mYour new balance is : \x1b[0m" << balance << endl;
            updateBalanceInFile();
        }
        else
        {
            cout << "\n\n\x1b[31mInvalid amount provided or account is deactivated. Contact support for further assistance. \x1b[0m" << endl;
        }
    }

    void Deposit(double amount)
    {
        if (open && amount > 0)
        {
            balance += amount;
            transactions.emplace_back("Deposit", amount);
            cout << "\x1b[32mSuccessfully deposited Rs- " << amount << " to your account.\x1b[0m" << endl;
            cout << "\x1b[32mYour new balance is : \x1b[0m" << balance << endl;
            updateBalanceInFile();
        }
        else
        {
            cout << "\n\n\x1b[31mInvalid amount provided or account is deactivated. Contact support for further assistance. \x1b[0m" << endl;
        }
    }

    void PrintAccountSummary()
    {
        cout << "\x1b[32mAccount Holder : \x1b[0m" << username << endl;
        cout << "\x1b[32mBalance : Rs- \x1b[0m" << balance << endl;
        cout << "\x1b[32mTransactions : \x1b[0m" << endl;
        for (const Transaction &transaction : transactions)
        {
            cout << transaction.type << ", Amount : Rs- " << transaction.amount << endl;
        }
        cout << " " << endl;
    }

    void CloseAccount(Database &database)
    {
        open = false;
        database.deleteUser(username);
        database.saveUsersToFile();
        cout << "\x1b[32mYour account has been closed.\x1b[0m" << endl;
    }

    void setUsername(const string &newUsername)
    {
        username = newUsername;
    }

    void updateBalanceInFile()
    {
        ifstream inputFile("users.txt");
        ofstream outputFile("temp.txt");

        string line;
        while (getline(inputFile, line))
        {
            istringstream iss(line);
            string storedUsername, storedPassword;
            double storedBalance;

            if (iss >> storedUsername >> storedPassword >> storedBalance)
            {
                if (storedUsername == username)
                {
                    outputFile << username << " " << storedPassword << " " << balance << endl;
                }
                else
                {
                    outputFile << line << endl;
                }
            }
        }

        inputFile.close();
        outputFile.close();

        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
};

class BankManager
{
public:
    BankManager()
    {
        loadManagersFromFile();
    }
    bool login()
    {
        string username, password;
        cout << "\x1b[35mEnter username : \x1b[0m";
        cin >> username;
        if (managers.find(username) != managers.end())
        {
            cout << "\x1b[31mEnter password : \x1b[0m";
            cin >> password;

            if (managers[username] == password)
            {
                return true;
            }
        }
        cout << "\x1b[31mIncorrect login. Please try again. \x1b[0m" << endl;
        return false;
    }

    bool loadManagersFromFile()
    {
        ifstream managerFile("managers.txt");
        if (!managerFile)
        {
            return false;
        }

        string line;
        while (getline(managerFile, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                managers[username] = password;
            }
        }
        managerFile.close();
        return true;
    }

private:
    unordered_map<string, string> managers;
};

int main()
{
    Database database;
    BankManager manager;
    string username, password;
    double initBalance;
    intro();
    BankAccount acc("", database);
    int choice;
    int bankingOption;
    double withdrawAmt;
    double depositAmt;
    bool loggedIn = false;
    bool managerLoggedIn = false;
    while (true)
    {
        //system("clear");
        if (!loggedIn)
        {
            cout << "\n\n\x1b[36m1. L o g i n.\n\n2. C r e a t e   A c c o u n t. \n\n3. M a n a g e r   L o g i n.\n\n4. E x i t.\x1b[0m\n";
            cin >> choice;

            if (choice == 1)
            {
                if (database.login())
                {
                    loggedIn = true;
                    username = username;
                    acc.setUsername(username);
                }
                cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                char y;
                cin >> y;
            }
            else if (choice == 2)
            {
                string newUsername, password;
                cout << "\x1b[32mEnter new username : \x1b[0m";
                cin >> newUsername;
                cout << "\x1b[31Enter password : \x1b[0m";
                cin >> password;
                database.createUser(newUsername, password);
                username = newUsername;
                acc.setUsername(username);
                cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                char y;
                cin >> y;
            }
            else if (choice == 3)
            {
                if (manager.login())
                {
                    loggedIn = true;
                    managerLoggedIn = true;
                    // Manager login options
                    cout << "\n\n\x1b[36mManager Options\x1b[0m\n";
                    cout << "\n\n\x1b[35m1. Get List of Users and Account Details\n2. Logout\x1b[0m\n";
                    cin >> bankingOption;
                    if (bankingOption == 1)
                    {
                        ifstream userFile("users.txt");
                        if (userFile.is_open())
                        {
                            string line;
                            while (getline(userFile, line))
                            {
                                cout << line << endl;
                            }
                            userFile.close();
                        }
                        cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                        char y;
                        cin >> y;
                    }
                    if (bankingOption == 2)
                    {
                        managerLoggedIn = false;
                        loggedIn = false;
                        cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                        char y;
                        cin >> y;
                    }
                }
            }
            else if (choice == 4)
            {
                break; // Exit the program
            }
        }
        else
        {
            if (managerLoggedIn)
            {
                cout << "\n\x1b[36mManager Options\x1b[0m\n";
                cout << "\n\n\x1b[32m1. Get List of Users and Account Details\n2. Logout\x1b[32m\n";
                cin >> bankingOption;
                if (bankingOption == 1)
                {
                    ifstream userFile("users.txt");
                    if (userFile.is_open())
                    {
                        string line;
                        while (getline(userFile, line))
                        {
                            cout << line << endl;
                        }
                        userFile.close();
                    }
                    cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                    char y;
                    cin >> y;
                }
                if (bankingOption == 2)
                {
                    loggedIn = false;
                    cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                    char y;
                    cin >> y;
                }
            }
            else
            {
                // Account login options
                cout << "\n\x1b[36mUser Options :\x1b[0m\n";
                cout << "\x1b[32m1. Withdraw\n2. Deposit\n3. Get Account Summary\n4. Close Account\n5. Logout\x1b[0m\n";
                cin >> bankingOption;
                if (bankingOption == 1)
                {
                    cout << "\x1b[35mPlease enter withdrawal amount : \x1b[0m";
                    cin >> withdrawAmt;
                    acc.Withdraw(withdrawAmt);
                }
                else if (bankingOption == 2)
                {
                    cout << "\x1b[35mPlease enter deposit amount : \x1b[0m";
                    cin >> depositAmt;
                    acc.Deposit(depositAmt);
                }
                else if (bankingOption == 3)
                {
                    acc.PrintAccountSummary();
                }
                else if (bankingOption == 4)
                {
                    acc.CloseAccount(database);
                    loggedIn = false;
                }
                else if (bankingOption == 5)
                {
                    loggedIn = false;
                }
                cout << "\n\n\x1b[32mEnter 'Y' to contiue : \x1b[0m";
                char y;
                cin >> y;
            }
        }
    }
    cout << "\n\n\x1b[46mThanks For Using This Software......\n\nBy Satam\x1b[0m\n\n";
    return 0;
}
