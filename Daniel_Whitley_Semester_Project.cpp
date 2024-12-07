#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct Assignment {
    // Assignment member variables
    string assignmentName; // Name of the assignment
    string dayDue; // Day of the week that it is due
    double estTime; // Estimated time for the user to complete the assignment

    Assignment() { this->estTime = 0; }
};

struct Course {
    // Course member variables
    string courseName; // Name of the course
    string startMeetingTime; // When the class starts
    string endMeetingTime; // When the class ends
    string meetingDays; // The days of the week that the course meets
    vector<Assignment> assignments; // Creates a vector of Assignment structures
};

class Student {
private:
    // Private member variables
    string name; // Student name
    vector<Course> courses; // Vector of Course structures

public:
    // Accessor and mutator functions
    void setName(string username) { name = username; }
    string getName() { return name; }

    // Adds a course and assignments to the courses vector
    void addCourse();

    // Calls other functions to set name and other coursework items
    void studentInfo();

    // Displays all the entered data such as name, course info, and assignment info
    void showStudentInfo();

    // Returns the size of the courses vector
    int numCourses() { return courses.size(); }

    // Returns a course structure from the courses vector
    Course getCourse(int index) { return courses[index]; }
};

// Function declarations
void makeAssignment(Assignment& assignment); // Makes an assignment structure
void makeCourse(Course& course); // Makes a course structure
void toLower(string& userString); // Changes all the letters in a string to lowercase
void makeWeek(string (&weeklyLayout)[38][6], Student person); // Makes an array and fills in the basics like day of the week and time
void scheduleAssignments(string(&weeklyLayout)[38][6], Student person);// Fills in the array with the courses and times to do the homework assignments
void swap(Assignment& hw1, Assignment& hw2); // Swaps structures
void sort(vector<Assignment>& homeworks); // Sorts the structures in a vector of structures
int convert2Day(string day); // Takes a string and returns a number for the day
void schedule(vector<Assignment> userAssignments, string(&weeklyLayout)[38][6]); // Makes the schedule
void fitAssignment(string(&weeklyLayout)[38][6], string assignmentname, int consecRows); // Fits assignments into available times
void formatSchedule(string weeklyLayout[38][6]); // formats and writes the schedule in a file
string adjustWidth(int widthInt, string usersString); // Makes all the strings in a column the same width

int main() {
    Student student;
    string week[38][6];
    
    // Calls function for student to add information about courses
    student.studentInfo();

    // Calls function to write the entered data into a file called Assignments.txt
    student.showStudentInfo();

    // Calls function to fill an array with the layout of the weekly courses
    makeWeek(week, student);

    // Calls function to decide when to do the assignments and stores them in the array
    scheduleAssignments(week, student);

    // Calls function to write the schedule to Schedule.txt
    formatSchedule(week);

    return 0;
}

// Function definition, passes a structure by reference to the function and assigns 
// member variables from the user input
void makeAssignment(Assignment& assignment) {
    // Sets the assignment name member variable
    cout << "Enter the name of the assignment: ";
    getline(cin, assignment.assignmentName);

    // Sets the day due member variable
    cout << "Enter the due date as a day of the week: ";
    getline(cin, assignment.dayDue);

    // Sets the estimated time to complete member variable
    cout << "Enter the estimated time to complete the assignment to the nearest quarter of an hour: ";
    cin >> assignment.estTime;

    cout << "You are done adding assignment details." << endl;
}

// Function definition, passes a structure by reference to the function and assigns 
// member variables from the user input and allows for the user to enter assignments
void makeCourse(Course& course) {
    // Local variable declaration
    string answer2;
    Assignment currAssignment;

    // Prints a message
    cout << "Enter in times for breaks or time to end homework each day as a course without any assignments." << endl;

    // Sets the course name member variable
    cout << "Enter the course name: ";
    getline(cin, course.courseName);

    // Sets the meeting days member variables
    cout << "Enter the days that the course meets using single letter day codes : " << endl
        << "For example a Tuesday/Thursday class will be TH, "
        << "and a Monday/Wednesday/Friday class will be MWF.\n"
        << "Enter \"Online\" for online classes." << endl;
    cin >> course.meetingDays;
    cin.ignore();

    // Sets the meeting time member variables
    cout << "Enter the class starting time to the nearest quarter of an hour. Enter NA for online classes: ";
    getline(cin, course.startMeetingTime);

    cout << "Enter the class ending time to the nearest quarter of an hour. Enter NA for online classes: ";
    getline(cin, course.endMeetingTime);

    // Option to add assignments to the course 
    cout << "Would you like to add an assignment for this course (yes/no)? ";
    cin >> answer2;
    cin.ignore();
    toLower(answer2);

    // Continues to add assignments
    while (answer2 == "yes") {
        // Calls the function to make an assignment
        makeAssignment(currAssignment);

        // Adds the assignment to the assignments vector member variable
        course.assignments.push_back(currAssignment);

        cout << "Would you like to add another assignment for this course (yes/no)? ";
        cin >> answer2;
        cin.ignore();
        toLower(answer2);
    }

    cout << "You are done adding assignments to this course." << endl;
}

// Function definition, adds a course to the courses vector
// It calls the function to make assignments for the course 
void Student::addCourse() {
    // Local variable declarations
    string answer1;
    Course currCourse;

    cout << "Would you like to add a course? (yes/no): ";
    cin >> answer1;
    cin.ignore();
    toLower(answer1);

    // Continues to add courses
    while (answer1 == "yes") {
        // Calls the function to make a course
        makeCourse(currCourse);

        // Adds course to the courses vector
        courses.push_back(currCourse);
        currCourse.assignments.clear();

        cout << "Would you like to add another course? (yes/no)? ";
        cin >> answer1;
        cin.ignore();
        toLower(answer1);
    }

    cout << "You are done adding courses." << endl;
}

// Function definition, calls mutators to the private member variables
void Student::studentInfo() {
    // Local variable declarations
    string username;

    // Calls the mutator to set the student name member variable
    cout << "Enter your name: ";
    getline(cin, username);
    setName(username);

    // Calls the function to add courses
    addCourse();
}

// Function definition, calls accessors to the private member variables
void Student::showStudentInfo() {
    // Local variable declarations
    ofstream outStream;

    outStream.open("Assignments.txt");

    // Makes sure file can be opened
    if (outStream.fail()) {
        cout << "File failed to open. Program terminated." << endl;
        exit(1);
    }

    outStream << getName() << endl << endl;

    // Prints each course the student is enrolled in
    for (int i = 0; i < courses.size(); i++) {
        Course selectedCourse = courses[i];
        
        outStream << selectedCourse.courseName << ": " <<
            selectedCourse.meetingDays << ", " <<
            selectedCourse.startMeetingTime << "-" <<
            selectedCourse.endMeetingTime << endl;
        int count = 1;
        outStream << "  " << "Assignments: ";

        if (selectedCourse.assignments.empty()) {
            outStream << " None" << endl;
        }

        // For each course, print the assignments
        for (int j = 0; j < selectedCourse.assignments.size(); j++) {
            Assignment selectedAssignment = selectedCourse.assignments[j];
            outStream << endl << "      " << count << ". " << selectedAssignment.assignmentName << ":" << endl;
            outStream << "          Due: " << selectedAssignment.dayDue << endl;
            outStream << "          Estimated time to complete: " << selectedAssignment.estTime << " hours" << endl << endl;

            count += 1;
        }
    }

    // Closes the out file stream
    outStream.close();
}

// Function definition, makes a string lowercase for loop controls
void toLower(string& userString) {
    for (int i = 0; i < userString.length(); i++) {
        userString[i] = tolower(userString[i]);
    }
}

// Function definiton, fills array with markers for the courses
void makeWeek(string (&weeklyLayout)[38][6], Student person) {
    // Local variable declarations
    string time, tod;
    string hour[] = { "8", "9", "10", "11", "12", "1", "2", "3", "4", "5" };
    string min[] = { "00", "15", "30", "45" };
    string days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
    Course userCourse;

    // Fills in the days
    for (int i = 0; i < 1; i++) {
        for (int j = 1; j < 6; j++) {
            weeklyLayout[i][j] = days[j - 1];
        }
    }

    // Fills in the times
    for (int i = 1; i < 38; i++) {
        time = hour[(i - 1) / 4];
        if ((stoi(time) >= 8) && (stoi(time) < 12)) {
            tod = " a.m.";
        }
        else {
            tod = " p.m.";
        }
        time += ':';
        time += min[(i - 1) % 4] + tod;
        
        // Writes the time string to the first column
        weeklyLayout[i][0] = time;
    }

    
    // Accesses the courses from the vector and fills in the meeting days and times
    for (int i = 0; i < person.numCourses(); i++) {
        int row1 = 0, row2 = 0, col = 0;
        userCourse = person.getCourse(i);

        // Turns the meeting times into usable indices
        while ((userCourse.startMeetingTime.substr(0, 4) != weeklyLayout[row1][0].substr(0, 4)) && (row1 < 37)) {
            row1 += 1;
        }

        // Turns the meeting times into usable indices
        while ((userCourse.endMeetingTime.substr(0, 4) != weeklyLayout[row2][0].substr(0, 4)) && (row2 < 37)) {
            row2 += 1;
        }
        
        // Accesses the meeting days to get the number of times a course meets in a week
        for (int j = 0; j < userCourse.meetingDays.length(); j++) {
            // Turns the string into usable indices
            toLower(userCourse.meetingDays);
            switch (userCourse.meetingDays[j]) {
                case 'm':
                    col = 1;
                    break;
                case 't':
                    col = 2;
                    break;
                case 'w':
                    col = 3;
                    break;
                case 'h':
                    col = 4;
                    break;
                case 'f':
                    col = 5;
                    break;
            }

            toLower(userCourse.meetingDays);
            if (userCourse.meetingDays == "online") {
                break;
            }
            // Assigns "XXX" to whenever a class is
            for (int x = row1; x <= row2; x++) {
                weeklyLayout[x][col] = "XXX";
            }
        }
    }
}

// Function defintion, fits the assignments into available times
// Assignments are prioritized based on day due
void scheduleAssignments(string(&weeklyLayout)[38][6], Student person) {
    // A vector to store all the student assignemnts
    vector<Assignment> allAssignments;
    
    // Reads all of the assignments from each Course structure in the Student class to the vector
    for (int i = 0; i < person.numCourses(); i++) {
        // Gets the current course
        for (int j = 0; j < person.getCourse(i).assignments.size(); j++) {
            allAssignments.push_back(person.getCourse(i).assignments[j]);
        }
    }
    
    // First organizes all the asignments by duedate
    sort(allAssignments);

    // Tries to fit the assignments into available spaces in the day
    // Assignments will not be done after 5pm or after the day they're due
    schedule(allAssignments, weeklyLayout);
}

// Function defintion sorts elements of a vector by due date
void sort(vector<Assignment>& homeworks) {
    // Local variable declarations
    int day1, day2;

    // Sorts assignments by the day due
    for (int i = homeworks.size(); i > 1; i--) {
        for (int j = 0; j < i - 1; j++) {
            toLower(homeworks[j].dayDue);
            toLower(homeworks[j + 1].dayDue);
            day1 = convert2Day(homeworks[j].dayDue);
            day2 = convert2Day(homeworks[j + 1].dayDue);

            if (day1 > day2) {
                swap(homeworks[j], homeworks[j + 1]);
            }
        }
    }

    // Sorts assignments by the time to complete
    for (int i = homeworks.size(); i > 1; i--) {
        for (int j = 0; j < i - 1; j++) {
            if ((homeworks[j].estTime < homeworks[j + 1].estTime) && (homeworks[j].dayDue == homeworks[j + 1].dayDue)) {
                swap(homeworks[j], homeworks[j + 1]);
            }
        }
    }
}

// Function defintion, swaps the Structures
void swap(Assignment& hw1, Assignment& hw2) {
    Assignment temp;

    temp = hw1;
    hw1 = hw2;
    hw2 = temp;
}

// Function definiton, turns a day string to a numeric value
int convert2Day(string day) {
    // Branch determines the numeric day representation
    if (day == "monday") { return 1; }
    else if (day == "tuesday") { return 2; }
    else if (day == "wednesday") { return 3; }
    else if (day == "thursday") { return 4; }
    else if (day == "friday") { return 5; }
    else if (day == "saturday") { return 5; }
    else if (day == "sunday") { return 5; }
    else {
        cout << "Error in the day." << endl;
        exit(1);
    }
}

// Function definition, takes the sorted assignments vector
// and fits the assignments to when they can be completed
void schedule(vector<Assignment> userAssignments, string(&weeklyLayout)[38][6]) {
    int numRows;

    // Loops through each assignment
    for (int i = 0; i < userAssignments.size(); i++) {
        numRows = static_cast<int>(userAssignments[i].estTime * 4);

        fitAssignment(weeklyLayout, userAssignments[i].assignmentName, numRows);
    }
}

// Function defintion, finds a certain number of consecutive blank indices in the array, and writes a string to them
void fitAssignment(string (&weeklyLayout)[38][6], string assignmentname, int consecRows) {
    int consecBlanks = 0;
    bool isFit = false;
    int rowStart, rowEnd;

    for (int j = 1; j < 6; j++) {
        for (int k = 1; k < 38; k++) {
            if (weeklyLayout[k][j].empty()) {
                consecBlanks++;
            }
            else {
                consecBlanks = 0;
            }
             
            if (consecBlanks == consecRows) {
                isFit = true;
                rowStart = k - consecRows + 1;
                rowEnd = k + 1;
                break;
            }
        }

        if (isFit) { 
            for (int l = rowStart; l <= rowEnd - 1; l++) {
                weeklyLayout[l][j] = assignmentname;
            }
            break;
        }
    }

    // Displays message if the assignment could not be fit
    if (!isFit) {
        cout << "Assignment: " << assignmentname << " could not be fit."
            << "You may need to break the assignment up." << endl;
    }
}

// Function defintion, takes the schedule array and formats it for output
void formatSchedule(string weeklyLayout[38][6]) {
    // Local variable declarations
    ofstream outstream;
    int maxWidth[6] = { 0, 0, 0, 0, 0, 0 };

    // Opens file to store the schedule
    outstream.open("Schedule.txt");

    // Makes sure file can be opened
    if (outstream.fail()) {
        cout << "File failed to open. Program terminated." << endl;
        exit(1);
    }

    // Gets the maximum width of each column for formatting
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 38; j++) {
            if (weeklyLayout[i][j].length() > maxWidth[i]) {
                maxWidth[i] = weeklyLayout[i][j].length();
            }
        }
    }

    // Removes the filler XXX from the array
    for (int i = 0; i < 38; i++) {
        for (int j = 0; j < 6; j++) {
            if (weeklyLayout[i][j] == "XXX") {
                weeklyLayout[i][j].clear();
            }
        }
    }

    // Writes data from the array to the file Schedule.txt
    for (int i = 0; i < 38; i++) {
        for (int j = 0; j < 6; j++) {
            outstream << setiosflags(ios::right) << adjustWidth(maxWidth[j], weeklyLayout[i][j]) << "  ";
        }
        outstream << endl;
    }

    // Closes the outstream
    outstream.close();
}

// Function definition, takes a string and a length and extends it to that length by adding spaces
string adjustWidth(int widthInt, string usersString) {
    // Local variable declarations
    string adjustment;
    int difference;

    // Number of spaces to be added
    difference = widthInt - usersString.length() + 1;

    // Adds spaces to a blank string
    for (int i = 0; i < difference; i++) {
        adjustment += " ";
    }

    // Adds the string to the new string
    adjustment += usersString;

    // Returns the new string
    return adjustment;
 }