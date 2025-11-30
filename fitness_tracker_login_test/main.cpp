#include "login.h"
//#include "Workout.h"

void test_workout();

int main() {
    bool loggedIn = false;

    while (!loggedIn) {     // loop until successful login or exit
        draw_menu();
        int choice = get_validated_menu_choice();

        switch (choice) {
            case 1: 
                register_user(); 
                break;
            case 2:
                loggedIn = login_user();    // login successful, exit menu loop
                break;
            case 3:
                system("cls");
                cout << "Goodbye!\n";
                cin.get();
                cin.get();
                return 0;
        }
    }

    test_workout();

    cin.get();
    cin.get();
    return 0;
}


void test_workout() {   // this will contain the fitness tracker UI
    cout << "IT WORKS LETS GOOO!" << endl;
}