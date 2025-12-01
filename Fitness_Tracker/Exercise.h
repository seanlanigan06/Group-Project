#ifndef EXERCISE_H
#define EXERCISE_H

#include <iostream>
#include <string>

using namespace std;   // Added per your request

class Exercise {
private:
    string name;
    int sets;
    int reps;

public:
    // Constructors
    Exercise();
    Exercise(const string& name, int sets, int reps);
    ~Exercise();

    // Setters
    void set_name(const string& n);
    void set_sets(int s);
    void set_reps(int r);

    // Getters
    string get_name() const;
    int get_sets() const;
    int get_reps() const;

    // Input method
    void input_exercise();

    // File I/O
    void write_to_file(ostream& out) const;
    bool read_from_file(istream& in);

    // Print operator
    friend ostream& operator<<(ostream& os, const Exercise& ex);
};

#endif