#include <iostream>
#include <limits>
#include <filesystem>
#include <fstream>
#include "Manager.h"
#include "Workout.h"
#include "Exercise.h"
#include "Login.h"
#include "Stats.h"

using namespace std;
namespace fs = std::filesystem;

string current_user;

// ------------------- VALIDATED INPUT -------------------
int get_int(const string& prompt, int min, int max) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= min && x <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ------------------- LIST REGISTERED USERS -------------------
void list_registered_users() {
    ifstream file("users.txt");
    string username, password;
    int count = 0;

    cout << "\nRegistered Users:\n";
    while (file >> username >> password) {
        count++;
        cout << count << ". " << username << endl;
    }

    if (count == 0)
        cout << "No registered users.\n";
    cout << endl;
}

// ------------------- WORKOUT MENU -------------------
void workout_menu(Workout& workout) {
    fs::create_directories("workouts/" + current_user);
    system("cls");

    while (true) {
        cout << "\n--- Workout: " << workout.get_name() << " ---\n\n";
        cout << "1. Add Exercise\n"
             << "2. Show Workout Info\n"
             << "3. Remove Exercise\n"
             << "4. Set Duration\n"
             << "5. Save Workout (updates overall statistics)\n"
             << "6. Finish Workout / Back to Main Menu\n";

        int choice = get_int("Choose: ", 1, 6);
        string filepath = "workouts/" + current_user + "/" + workout.get_name() + ".txt";

        switch (choice) {
            case 1: {
                Exercise e;
                e.input_exercise();
                workout.add_exercise(e);
                break;
            }
            case 2:
                system("cls");
                workout.show_exercises();
                break;
            case 3: {
                system("cls");
                workout.show_exercises();
                if (!workout.get_exercises().empty()) {
                    int i = get_int("Remove which? ", 1, workout.get_exercises().size());
                    workout.remove_exercise(i - 1);
                    system("cls");
                }
                break;
            }
            case 4:
                system("cls");
                workout.set_duration(get_int("Enter duration (minutes): ", 0, 10000));
                system("cls");
                break;
            case 5: { // Save Workout AND log to overall statistics
                workout.calculate_calories();         // ensure calories are up-to-date
                workout.save_to_file(filepath);
                workout.log_completion(current_user); // update history.txt
                system("cls");
                cout << "Saved and recorded in overall statistics.\n";
                break;
            }
            case 6: { // Finish without logging to overall statistics
                workout.save_to_file(filepath);
                system("cls");
                cout << "Finished workout. (Not recorded in overall statistics)\n";
                return;
            }
        }
    }
}

// ------------------- OVERALL STATISTICS -------------------
void view_overall_statistics() {
    // Load weekly calorie goal from file
    string goal_path = "workouts/" + current_user + "/weekly_goal.txt";
    int weekly_calorie_goal = 0;
    ifstream goal_file(goal_path);
    if (goal_file >> weekly_calorie_goal) {
        // successfully loaded previous goal
    }
    goal_file.close();

    Stats stats(current_user, weekly_calorie_goal);
    stats.show_statistics();

    cout << "Press Enter to return to menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("cls");
}

// ------------------- FITNESS TRACKER MENU -------------------
void fitness_tracker() {
    fs::create_directories("workouts/" + current_user);
    Manager manager(current_user);
    manager.load_list();

    // Load weekly calorie goal from file
    string goal_path = "workouts/" + current_user + "/weekly_goal.txt";
    int weekly_calorie_goal = 0;
    ifstream goal_file(goal_path);
    if (goal_file >> weekly_calorie_goal) {
        // successfully loaded previous goal
    }
    goal_file.close();

    while (true) {
        cout << "\n===== WORKOUT MANAGER (" << current_user << ") =====\n";
        cout << "1. Select Workout\n"
             << "2. Create Workout\n"
             << "3. Delete Workout\n"
             << "4. View Overall Statistics\n"
             << "5. Set Weekly Calorie Goal\n"
             << "6. Logout\n"
             << "7. Exit\n";

        int choice = get_int("Choose: ", 1, 7);

        switch (choice) {
            case 1: {
                auto list = manager.get_workouts();
                if (list.empty()) {
                    system("cls");
                    cout << "No workouts available.\n";
                    continue;
                }

                system("cls");
                cout << "\nAvailable Workouts:\n";
                for (size_t i = 0; i < list.size(); i++)
                    cout << i + 1 << ". " << list[i] << endl;

                int i = get_int("Select: ", 1, list.size()) - 1;

                Workout w(list[i]);
                string filepath = "workouts/" + current_user + "/" + list[i] + ".txt";
                if (!w.load_from_file(filepath)) {
                    cout << "No existing file for this workout. Starting new.\n";
                }
                workout_menu(w);
                break;
            }
            case 2: {
                system("cls");
                cout << "Enter new workout name: ";
                string name;
                getline(cin, name);

                manager.add_workout(name);
                manager.save_list();

                Workout w(name);
                string filepath = "workouts/" + current_user + "/" + name + ".txt";
                w.save_to_file(filepath);
                workout_menu(w);
                break;
            }
            case 3: {
                system("cls");
                auto list = manager.get_workouts();
                if (list.empty()) {
                    cout << "No workouts to delete.\n";
                    continue;
                }

                for (size_t i = 0; i < list.size(); i++)
                    cout << i + 1 << ". " << list[i] << endl;

                int i = get_int("Delete which? ", 1, list.size());
                string filepath = "workouts/" + current_user + "/" + list[i - 1] + ".txt";

                fs::remove(filepath);
                manager.delete_workout(i - 1);
                manager.save_list();
                system("cls");
                cout << "Deleted.\n";
                break;
            }
            case 4:
                system("cls");
                view_overall_statistics();
                break;
            case 5: { // Set weekly calorie goal
                system("cls");
                weekly_calorie_goal = get_int("Enter your weekly calorie goal: ", 0, 100000);
                ofstream goal_out(goal_path); // overwrite previous goal
                goal_out << weekly_calorie_goal;
                goal_out.close();
                system("cls");
                cout << "Weekly calorie goal set to " << weekly_calorie_goal << " calories.\n";
                break;
            }
            case 6:
                system("cls");
                cout << "Logged out.\n";
                return;
            case 7:
                system("cls");
                cout << "Goodbye!\n";
                cin.get();
                exit(0);
        }
    }
}

// ------------------- MAIN -------------------
int main() {
    while (true) {
        bool logged_in = false;

        while (!logged_in) {
            draw_menu();
            int choice = get_validated_menu_choice();

            ifstream user_file("users.txt");
            bool has_users = user_file.peek() != ifstream::traits_type::eof();
            user_file.close();

            switch (choice) {
                case 1: // Register
                    register_user();
                    break;
                case 2: // Login
                    if (!has_users) {
                        system("cls");
                        cout << "No registered users. Please register first.\n";
                        break;
                    }
                    system("cls");
                    list_registered_users();
                    logged_in = login_user(current_user);
                    break;
                case 3: { // Delete user
                    if (!has_users) {
                        system("cls");
                        cout << "No registered users. Nothing to delete.\n";
                        break;
                    }
                    system("cls");
                    list_registered_users();
                    cout << "Enter username to delete: ";
                    string del_user = get_validated_username();
                    if (user_exists(del_user)) {
                        cout << "Are you sure you want to delete '" << del_user << "'? (y/n): ";
                        char confirm;
                        cin >> confirm;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (confirm == 'y' || confirm == 'Y') {
                            delete_user(del_user);
                        } else {
                            cout << "Deletion cancelled.\n";
                        }
                    } else {
                        system("cls");
                        cout << "User does not exist.\n";
                    }
                    break;
                }
                case 4: // Exit
                    system("cls");
                    cout << "Goodbye!\n";
                    cin.get();
                    return 0;
            }
        }

        fitness_tracker();
        current_user.clear();
    }
}

// hi!