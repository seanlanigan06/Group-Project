#include "Workout.h"
#include <fstream>
#include <limits>
#include <filesystem>
#include <ctime>
namespace fs = std::filesystem;

// ------------------- CALORIES CALCULATION -------------------
void Workout::calculate_calories() {
    double total_reps = 0;
    for (const Exercise& ex : exercises)
        total_reps += ex.get_sets() * ex.get_reps();

    // Example formula: each rep burns 0.5 cal, scaled by duration (per 30 min)
    calories_burned = static_cast<int>(total_reps * 0.5 * (duration_minutes / 30.0));
}

// ------------------- EXERCISE MANAGEMENT -------------------
void Workout::add_exercise(const Exercise& ex) {
    exercises.push_back(ex);
    calculate_calories();
}

void Workout::remove_exercise(int index) {
    if (index >= 0 && index < (int)exercises.size())
        exercises.erase(exercises.begin() + index);
    calculate_calories();
}

void Workout::set_duration(int d) {
    duration_minutes = d;
    calculate_calories();
}

void Workout::show_exercises() const {
    cout << "Duration: " << duration_minutes << " minutes\n";
    cout << "Calories burned: " << calories_burned << "\n\n";

    if (exercises.empty()) {
        cout << "No exercises in this workout.\n";
        return;
    }

    for (size_t i = 0; i < exercises.size(); i++)
        cout << i + 1 << ". " << exercises[i] << endl;
}

// ------------------- FILE I/O -------------------
bool Workout::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) return false;

    int count;
    if (!(file >> duration_minutes)) return false;
    if (!(file >> calories_burned)) return false;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!(file >> count)) return false;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    exercises.clear();
    for (int i = 0; i < count; i++) {
        Exercise e;
        if (e.read_from_file(file))
            exercises.push_back(e);
    }

    calculate_calories(); // recalc in case file calories differ
    return true;
}

void Workout::save_to_file(const string& filename) const {
    ofstream file(filename);

    file << duration_minutes << "\n";
    file << calories_burned << "\n";

    file << exercises.size() << "\n";
    for (const Exercise& e : exercises)
        e.write_to_file(file);
}

void Workout::log_completion(const string& username) {
    // Ensure user directory exists
    fs::create_directories("workouts/" + username);

    ofstream file("workouts/" + username + "/history.txt", ios::app);
    if (!file) {
        cerr << "Error opening history file.\n";
        return;
    }

    // Format date as YYYY-MM-DD
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char date_buf[11];
    strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", ltm);

    // Write line: DATE "WORKOUT NAME" DURATION CALORIES
    file << date_buf << " \"" << name << "\" " 
         << duration_minutes << " " << calories_burned << "\n";

    file.close();
}