#ifndef STATS_H
#define STATS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>

using namespace std;

class Stats {
private:
    string username;
    int weekly_calorie_goal;

    struct Entry {
        string date;
        string workout_name;
        int duration;
        int calories;
    };

    vector<Entry> history;

    // Load history from file
    void load_history() {
        history.clear();
        ifstream file("workouts/" + username + "/history.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            Entry e;
            if (!(iss >> e.date)) continue;

            // Read quoted workout name
            if (iss.peek() == ' ') iss.get();
            if (iss.get() != '"') continue;
            getline(iss, e.workout_name, '"');

            if (!(iss >> e.duration >> e.calories)) continue;

            history.push_back(e);
        }
    }

public:
    Stats(const string& user, int goal = 0) : username(user), weekly_calorie_goal(goal) {
        load_history();
    }

    void show_statistics() {
        load_history(); // Always reload latest data

        int total_calories = 0, total_duration = 0;
        for (const auto& e : history) {
            total_calories += e.calories;
            total_duration += e.duration;
        }

        cout << "\n===== STATISTICS (" << username << ") =====\n";
        cout << "Total workouts completed: " << history.size() << "\n";
        cout << "Total calories burned: " << total_calories << "\n";
        cout << "Average workout duration: " 
             << (history.empty() ? 0 : total_duration / history.size()) << " minutes\n";
        cout << "Weekly goal: " << weekly_calorie_goal << " calories\n";
        cout << "Progress: ";
        if (weekly_calorie_goal == 0) cout << "n/a\n\n";
        else cout << (total_calories * 100 / weekly_calorie_goal) << "%\n\n";
    }
};

#endif