#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <vector>
using namespace std;

class Manager {
private:
    vector<string> workout_names;
    string username;  // user-specific folder/list

public:
    // Constructor: set username
    Manager(const string& user) : username(user) {}

    // Load/save workouts for this user
    bool load_list();
    void save_list() const;

    void add_workout(const string& name);
    void delete_workout(int index);

    vector<string> get_workouts() const { return workout_names; }
};

#endif