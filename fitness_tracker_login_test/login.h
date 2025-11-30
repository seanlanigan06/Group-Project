#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>  // for system("cls") - clearing the screen
#include <conio.h>  // For getch() - password masking
#include <limits>
using namespace std;

// ---------------- PASSWORD MASKING ----------------
string get_masked_password() {
    string password = "";
    char ch;

    while (true) {
        ch = _getch();  // get character without echo

        if (ch == 13) { // ENTER
            cout << endl;
            if (password.empty()) {
                cout << "Password cannot be empty. Try again: ";
                continue;
            }
            break;
        }
        else if (ch == 8) { // BACKSPACE
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else if (ch == ' ') { 
            cout << "\nSpaces are not allowed. Try again.\nPassword: ";
            password.clear();
            continue;
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }

    return password;
}

// ---------------- INPUT VALIDATION ----------------
int get_validated_menu_choice() {
    int choice;

    while (true) {
        cout << "Choose an option: ";

        if (cin >> choice && choice >= 1 && choice <= 3)
            return choice;

        cout << "Invalid input. Enter 1-3.\n";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string get_validated_username() {
    string username = "";
    char ch;

    while (true) {
        ch = _getch();  // get character without echo

        if (ch == 13) { // ENTER
            cout << endl;
            if (username.empty()) {
                cout << "Username cannot be empty. Try again: ";
                continue;
            }
            break;
        }
        else if (ch == 8) { // BACKSPACE
            if (!username.empty()) {
                cout << "\b \b";
                username.pop_back();
            }
        }
        else if (ch == ' ') {
            cout << "\nSpaces are not allowed. Try again.\nUsername: ";
            username.clear();
            continue;
        }
        else {
            username.push_back(ch);
            cout << ch;   // unlike password, we show actual characters
        }
    }

    return username;
}

// ---------------- FILE CHECKS ----------------
bool user_exists(const string& username) {
    ifstream file("users.txt");
    string u, p;

    while (file >> u >> p)
        if (u == username)
            return true;

    return false;
}

bool validate_login(const string& username, const string& password) {
    ifstream file("users.txt");
    string u, p;

    while (file >> u >> p)
        if (u == username && p == password)
            return true;

    return false;
}

// ---------------- MAIN FUNCTIONS ----------------
void register_user() {
    string username, password;

    system("cls");
    cout << "\n===== REGISTER NEW ACCOUNT =====\n";
    cout << "Enter username: ";
    username = get_validated_username();

    if (user_exists(username)) {
        cout << "Username already exists! Choose another.\n";
        return;
    }

    cout << "Enter password: ";
    password = get_masked_password();

    ofstream file("users.txt", ios::app);
    file << username << " " << password << endl;

    system("cls");
    cout << "Registration successful!\n";
}

bool login_user() {
    const int MAX_ATTEMPTS = 3;   // maximum attempts for username/password
    string username, password;
    int attempts = 0;

    system("cls");
    cout << "\n========== LOGIN ==========\n";

    while (attempts < MAX_ATTEMPTS) {
        cout << "Username: ";
        username = get_validated_username();

        if (!user_exists(username)) {
            system("cls");
            cout << "Username does not exist. Try again.\n\n";
            attempts++;
            continue;  // go back to ask for username again
        }

        cout << "Password: ";
        password = get_masked_password();

        if (validate_login(username, password)) {
            system("cls");
            cout << "Login successful! Welcome, " << username << "!\n";
            return true;  // successful login
        } else {
            system("cls");
            cout << "Incorrect password. Try again.\n\n";
            attempts++;
        }
    }

    system("cls");
    cout << "Maximum login attempts exceeded. Returning to main menu.\n";
    return false;  // login failed after max attempts
}

// ---------------- MENU UI ----------------
void draw_menu() {
    cout << "\n=====================================\n";
    cout << "        USER ACCOUNT SYSTEM          \n";
    cout << "=====================================\n";
    cout << "  1. Register New User\n";
    cout << "  2. Login\n";
    cout << "  3. Exit\n";
    cout << "=====================================\n";
}