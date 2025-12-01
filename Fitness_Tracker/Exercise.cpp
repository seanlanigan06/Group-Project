#include "Exercise.h"
#include <limits>

using namespace std;

// Constructors
Exercise::Exercise() : name("Unnamed"), sets(0), reps(0) {}
Exercise::Exercise(const string& name, int sets, int reps)
    : name(name), sets(sets), reps(reps) {}
Exercise::~Exercise() {}

// Setters
void Exercise::set_name(const string& n) {
    if (n.empty()) {
        cout << "Name cannot be empty. Setting name to 'Unnamed'.\n";
        name = "Unnamed";
    } else {
        name = n;
    }
}

void Exercise::set_sets(int s) {
    if (s < 0) {
        cout << "Sets cannot be negative. Setting to 0.\n";
        sets = 0;
    } else {
        sets = s;
    }
}

void Exercise::set_reps(int r) {
    if (r < 0) {
        cout << "Reps cannot be negative. Setting to 0.\n";
        reps = 0;
    } else {
        reps = r;
    }
}

// Getters
string Exercise::get_name() const { return name; }
int Exercise::get_sets() const { return sets; }
int Exercise::get_reps() const { return reps; }

// Input method
void Exercise::input_exercise() {
    system("cls");
    cout << "\nEnter exercise name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Name cannot be empty. Setting to 'Unnamed'.\n";
        name = "Unnamed";
    }

    cout << "Enter number of sets: ";
    while (!(cin >> sets) || sets < 0) {
        cout << "Invalid. Enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter number of reps: ";
    while (!(cin >> reps) || reps < 0) {
        cout << "Invalid. Enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    system("cls");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// File save
void Exercise::write_to_file(ostream& out) const {
    out << name << "\n" << sets << "\n" << reps << "\n";
}

// File load
bool Exercise::read_from_file(istream& in) {
    if (!getline(in, name)) return false;
    if (!(in >> sets)) return false;
    if (!(in >> reps)) return false;
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// Print operator
ostream& operator<<(ostream& os, const Exercise& ex) {
    os << "Exercise: " << ex.name
       << " | Sets: " << ex.sets
       << " | Reps: " << ex.reps;
    return os;
}