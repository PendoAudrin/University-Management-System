#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Base Class
class Student {
public:
    string username, password, fullName, regNumber;
    bool feeCleared;
    int unitsRegistered; // New attribute to track registered units

    Student(string uname, string pass, string fname, string reg)
        : username(uname), password(pass), fullName(fname), regNumber(reg), feeCleared(false), unitsRegistered(0) {}

    virtual ~Student() {}

    virtual void chooseLevel() = 0; // Pure virtual function for level choice

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

    void chooseLevel() override {
        cout << "You have chosen the Undergraduate level." << endl;
    }
};

// Derived Class: Postgraduate
class Postgraduate : public Student {
public:
    Postgraduate(string uname, string pass, string fname, string reg)
        : Student(uname, pass, fname, reg) {}

    void chooseLevel() override {
        cout << "You have chosen the Postgraduate level." << endl;
    }
};

// Function to Choose Branch
void chooseBranch(string& selectedBranch) {
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
}

// Function to Register a User
void registerUser(vector<Student*>& students, const string& selectedBranch) {
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

    if (levelChoice == 1) {
        students.push_back(new Undergraduate(uname, pass, fname, reg));
    } else if (levelChoice == 2) {
        students.push_back(new Postgraduate(uname, pass, fname, reg));
    } else {
        cout << "Invalid choice. Registration failed." << endl;
        return;
    }

    cout << "Registration successful!" << endl;
}

// Function to Login a User
Student* loginUser(vector<Student*>& students, string& loggedInUsername) {
    string uname, pass;
    cout << "Enter Username: ";
    cin >> uname;
    cout << "Enter Password: ";
    cin >> pass;

    for (Student* student : students) {
        if (student->authenticate(uname, pass)) {
            loggedInUsername = uname;
            cout << "Login successful!" << endl;
            student->chooseLevel(); // Choose level after login
            return student;
        }
    }
    cout << "Invalid username or password. Please try again." << endl;
    return nullptr;
}

// Function to Clear Fee
void clearFee(Student* loggedInStudent) {
    if (!loggedInStudent) {
        cout << "You must be logged in to clear the fee." << endl;
        return;
    }
    loggedInStudent->feeCleared = true;
    cout << "Fee cleared successfully!" << endl;
}

// Function to View All Students
void viewAllStudents(const vector<Student*>& students) {
    if (students.empty()) {
        cout << "No students registered yet." << endl;
        return;
    }
    cout << "List of Registered Students:" << endl;
    for (const Student* student : students) {
        cout << "Username: " << student->username << ", Full Name: " << student->fullName
             << ", Reg Number: " << student->regNumber
             << ", Fee Cleared: " << (student->feeCleared ? "Yes" : "No")
             << ", Units Registered: " << student->unitsRegistered << endl;
    }
}

int main() {
    vector<Student*> students;
    string selectedBranch;
    string loggedInUsername;
    Student* loggedInStudent = nullptr;
    int choice;

    cout << "-------------------------------------------------" << endl;
    cout << "Welcome to Jomo Kenyatta University of Agriculture and Technology" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "University Management System" << endl;
        cout << "1. Choose Branch" << endl;
        cout << "2. Register Student" << endl;
        cout << "3. Login" << endl;
        if (loggedInStudent) {
            cout << "4. Register Units" << endl;
            cout << "5. Clear Fee" << endl;
        }
        cout << "6. View All Students" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                chooseBranch(selectedBranch);
                break;
            case 2:
                if (selectedBranch.empty()) {
                    cout << "You need to choose a branch first." << endl;
                    continue;
                }
                registerUser(students, selectedBranch);
                break;
            case 3:
                loggedInStudent = loginUser(students, loggedInUsername);
                break;
            case 4:
                if (loggedInStudent)
                    loggedInStudent->registerUnits();
                else
                    cout << "Please login first to register units." << endl;
                break;
            case 5:
                clearFee(loggedInStudent);
                break;
            case 6:
                viewAllStudents(students);
                break;
            case 7:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 7);

    for (Student* student : students) {
        student->saveToFile();
        delete student;
    }

    return 0;
}
