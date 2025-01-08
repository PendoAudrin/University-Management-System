#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Base Class: School
class School {
public:
    string selectedBranch;

    void chooseBranch() {
        cout << "Choose Branch:" << endl;
        cout << "1. Main Campus" << endl;
        cout << "2. Karen Campus" << endl;
        cout << "3. Ruiru Campus" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: selectedBranch = "Main Campus"; break;
            case 2: selectedBranch = "Karen Campus"; break;
            case 3: selectedBranch = "Ruiru Campus"; break;
            default: cout << "Invalid choice. Try again." << endl; break;
        }

        cout << "Branch " << selectedBranch << " selected successfully." << endl;
    }
};

// Derived Class: Student
class Student : public School {
public:
    string username, password, fullName, regNumber;
    bool feeCleared;
    int unitsRegistered;

    Student(string uname, string pass, string fname, string reg)
        : username(uname), password(pass), fullName(fname), regNumber(reg), feeCleared(false), unitsRegistered(0) {}

    virtual ~Student() {}

    bool authenticate(string uname, string pass) {
        return (username == uname && password == pass);
    }

    void registerUnits() {
        if (!feeCleared) {
            cout << "Fee not cleared. Units cannot be approved." << endl;
            return;
        }
        int numUnits;
        cout << "How many units would you like to register? ";
        cin >> numUnits;
        unitsRegistered += numUnits; // Update the registered units
        cout << "Units successfully registered: " << numUnits << endl;
    }

    void saveToFile() {
        ofstream file("students.txt", ios::app);
        if (file.is_open()) {
            file << username << " " << password << " " << fullName << " " << regNumber << " " << feeCleared << " " << unitsRegistered << endl;
            file.close();
        }
    }
};

// Derived Class: Undergraduate
class Undergraduate : public Student {
public:
    Undergraduate(string uname, string pass, string fname, string reg)
        : Student(uname, pass, fname, reg) {}

    void chooseLevel() {
        cout << "You have chosen the Undergraduate level." << endl;
    }
};

// Derived Class: Postgraduate
class Postgraduate : public Student {
public:
    Postgraduate(string uname, string pass, string fname, string reg)
        : Student(uname, pass, fname, reg) {}

    void chooseLevel() {
        cout << "You have chosen the Postgraduate level." << endl;
    }
};

// School Class (Main Base Class for Admission and Student Management)
class University : public School {
public:
    vector<Student*> students;

    void admitStudent() {
        string uname, pass, fname, reg;
        cout << "Enter Username: ";
        cin >> uname;
        cout << "Enter Password: ";
        cin >> pass;
        cout << "Enter Full Name: ";
        cin.ignore();
        getline(cin, fname);
        cout << "Enter Registration Number: ";
        cin >> reg;

        int levelChoice;
        cout << "Choose Level:" << endl;
        cout << "1. Undergraduate" << endl;
        cout << "2. Postgraduate" << endl;
        cin >> levelChoice;

        Student* newStudent;
        if (levelChoice == 1) {
            newStudent = new Undergraduate(uname, pass, fname, reg);
        } else if (levelChoice == 2) {
            newStudent = new Postgraduate(uname, pass, fname, reg);
        } else {
            cout << "Invalid choice. Admission failed." << endl;
            return;
        }

        students.push_back(newStudent);
        cout << "Admission successful!" << endl;

        // Call chooseBranch here to select the branch after admission
        chooseBranch();
    }

    void viewAllStudents() {
        if (students.empty()) {
            cout << "No students admitted yet." << endl;
            return;
        }
        cout << "List of Admitted Students:" << endl;
        for (const Student* student : students) {
            cout << "Username: " << student->username << ", Full Name: " << student->fullName
                 << ", Reg Number: " << student->regNumber
                 << ", Fee Cleared: " << (student->feeCleared ? "Yes" : "No")
                 << ", Units Registered: " << student->unitsRegistered << endl;
        }
    }

    Student* loginUser(string uname, string pass) {
        for (Student* student : students) {
            if (student->authenticate(uname, pass)) {
                cout << "Login successful!" << endl;
                student->chooseLevel();  // Choose level after login
                return student;
            }
        }
        cout << "Invalid username or password. Please try again." << endl;
        return nullptr;
    }
};

// Function to Clear Fee
void clearFee(Student* loggedInStudent) {
    if (!loggedInStudent) {
        cout << "You must be logged in to clear the fee." << endl;
        return;
    }
    loggedInStudent->feeCleared = true;
    cout << "Fee cleared successfully!" << endl;
}

// Main Function
int main() {
    University university;
    string loggedInUsername;
    Student* loggedInStudent = nullptr;
    int choice;

    cout << "-------------------------------------------------" << endl;
    cout << "Welcome to Jomo Kenyatta University of Agriculture and Technology" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "University Management System" << endl;
        cout << "1. Admit Student" << endl;
        cout << "2. Login" << endl;
        if (loggedInStudent) {
            cout << "3. Register Units" << endl;
            cout << "4. Clear Fee" << endl;
        }
        cout << "5. View All Students" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                university.admitStudent();
                break;
            case 2:
                {
                    string uname, pass;
                    cout << "Enter Username: ";
                    cin >> uname;
                    cout << "Enter Password: ";
                    cin >> pass;
                    loggedInStudent = university.loginUser(uname, pass);
                }
                break;
            case 3:
                if (loggedInStudent)
                    loggedInStudent->registerUnits();
                else
                    cout << "Please login first to register units." << endl;
                break;
            case 4:
                clearFee(loggedInStudent);
                break;
            case 5:
                university.viewAllStudents();
                break;
            case 6:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 6);

    for (Student* student : university.students) {
        student->saveToFile();
        delete student;
    }

    return 0;
}
