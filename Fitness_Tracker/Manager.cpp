#include "Manager.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

// Load the user's workout list
bool Manager::load_list() {
    workout_names.clear();

    // Ensure user directory exists
    fs::create_directories("workouts/" + username);

    string filepath = "workouts/" + username + "/workout_list.txt";
    ifstream file(filepath);
    if (!file) return false;

    string name;
    while (getline(file, name)) {
        if (!name.empty())
            workout_names.push_back(name);
    }

    return true;
}

// Save the user's workout list
void Manager::save_list() const {
    fs::create_directories("workouts/" + username);

    string filepath = "workouts/" + username + "/workout_list.txt";
    ofstream file(filepath);
    for (const string& name : workout_names)
        file << name << "\n";
}

// Add a new workout
void Manager::add_workout(const string& name) {
    workout_names.push_back(name);
}

// Delete a workout
void Manager::delete_workout(int index) {
    if (index >= 0 && index < (int)workout_names.size())
        workout_names.erase(workout_names.begin() + index);
}