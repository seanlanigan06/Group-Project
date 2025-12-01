#ifndef WORKOUT_H
#define WORKOUT_H

#include <iostream>
#include <vector>
#include <string>
#include "Exercise.h"

using namespace std;

class Workout {
private:
    string name;
    vector<Exercise> exercises;

    int duration_minutes;   // total duration
    int calories_burned;    // total calories

public:
    Workout() : duration_minutes(0), calories_burned(0) {}
    Workout(const string& name) : name(name), duration_minutes(0), calories_burned(0) {}

    string get_name() const { return name; }
    vector<Exercise>& get_exercises() { return exercises; }

    int get_duration() const { return duration_minutes; }
    int get_calories() const { return calories_burned; }
    void calculate_calories();

    void set_duration(int d);
    void add_exercise(const Exercise& ex);
    void remove_exercise(int index);
    void show_exercises() const;

    bool load_from_file(const string& filename);
    void save_to_file(const string& filename) const;

    void log_completion(const string& username); // logs workout to history.txt
};

#endif