#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>   // for system("cls")
#include <conio.h>   // for _getch()
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// ---------------- PASSWORD MASKING ----------------
inline string get_masked_password() {
    string password = "";
    char ch;

    while (true) {
        ch = _getch();

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
inline int get_validated_menu_choice(int minOption = 1, int maxOption = 4) {
    int choice;
    while (true) {
        cout << "Choose an option: ";
        if (cin >> choice && choice >= minOption && choice <= maxOption) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << "Invalid input. Enter " << minOption << "-" << maxOption << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

inline string get_validated_username() {
    string username = "";
    char ch;

    while (true) {
        ch = _getch();

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
            cout << ch; // show characters for username
        }
    }

    return username;
}

// ---------------- FILE CHECKS ----------------
inline bool user_exists(const string& username) {
    ifstream file("users.txt");
    string u, p;

    while (file >> u >> p)
        if (u == username)
            return true;

    return false;
}

inline bool validate_login(const string& username, const string& password) {
    ifstream file("users.txt");
    string u, p;

    while (file >> u >> p)
        if (u == username && p == password)
            return true;

    return false;
}

// ---------------- MAIN FUNCTIONS ----------------
inline void register_user() {
    string username, password;

    system("cls");
    cout << "\n===== REGISTER NEW ACCOUNT =====\n";
    cout << "Enter username: ";
    username = get_validated_username();

    if (user_exists(username)) {
        system("cls");
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

inline bool login_user(string& outUsername) {
    const int MAX_ATTEMPTS = 3;
    string username, password;
    int attempts = 0;

    cout << "\n========== LOGIN ==========\n";

    while (attempts < MAX_ATTEMPTS) {
        cout << "Username: ";
        username = get_validated_username();

        if (!user_exists(username)) {
            system("cls");
            cout << "Username does not exist. Try again.\n\n";
            attempts++;
            continue;
        }

        cout << "Password: ";
        password = get_masked_password();

        if (validate_login(username, password)) {
            outUsername = username;  // return logged-in username
            system("cls");
            cout << "Login successful! Welcome, " << username << "!\n";
            return true;
        } else {
            system("cls");
            cout << "Incorrect password. Try again.\n\n";
            attempts++;
        }
    }

    system("cls");
    cout << "Maximum login attempts exceeded. Returning to main menu.\n";
    return false;
}

// ---------------- DELETE USER ----------------
inline void delete_user(const string& username) {
    ifstream file("users.txt");
    ofstream temp("temp_users.txt");
    string u, p;
    bool found = false;

    // Copy all users except the one to delete
    while (file >> u >> p) {
        if (u != username) {
            temp << u << " " << p << endl;
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();

    if (!found) {
        cout << "User not found.\n";
        return;
    }

    // Replace original file with temp file
    remove("users.txt");
    rename("temp_users.txt", "users.txt");

    // Delete user's workouts folder
    string path = "workouts/" + username;
    if (fs::exists(path)) {
        fs::remove_all(path);
    }

    system("cls");
    cout << "User '" << username << "' and all their workouts have been deleted.\n";
}

// ---------------- MENU UI ----------------
inline void draw_menu() {
    cout << "\n=====================================\n";
    cout << "        USER ACCOUNT SYSTEM          \n";
    cout << "=====================================\n";
    cout << "  1. Register New User\n";
    cout << "  2. Login\n";
    cout << "  3. Delete User\n";
    cout << "  4. Exit\n";
    cout << "=====================================\n";
}

#endif