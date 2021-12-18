//============================================================================
// Name        : CourseProgram.cpp
// Author      : Braxton Morrow
// Version     : 1.0
// Copyright   : Copyright © 2021 Braxton Morrow
// Description : Final Project
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVHandler.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct CourseInfo {
    string courseNumber; // unique identifier
    string courseName;
    std::vector<string> coursePrereq;
};

//============================================================================
// Display Course Info
//============================================================================

void displayCourseInfo(CourseInfo course) {
    cout << course.courseNumber << " | " << course.courseName << " | Prerequesities: ";
    for (int i = 0; i < course.coursePrereq.size(); i++) {
        cout << course.coursePrereq.at(i) << " ";
    }
    cout << endl;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all class info
 */
vector<CourseInfo> loadCourses(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold course information
    vector<CourseInfo> courses;

    try {
        csv::Parser file = csv::Parser(csvPath);
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the course info
            CourseInfo course;
            course.courseNumber = file[i][0];
            course.courseName = file[i][1];
            for (int j = 2; j < file[i].size(); j++) {
                course.coursePrereq.push_back(file[i][j]);
            }
            courses.push_back(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return courses;
}

void selectionSort(vector<CourseInfo>& courses) {

    //creates index to smallest min bid 
    unsigned int min;
    unsigned int max = courses.size();
    unsigned int place;

    for (place = 0; place < max; ++place) {
        min = place;

        //position = position hold for swaps
        for (int j = place + 1; j < max; ++j) {
            if (courses[j].courseNumber.compare(courses[min].courseNumber) < 0) {
                min = j;
            }
        }
        if (min != place) {
            swap(courses[place], courses[min]);

        }
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "Course.csv";
    }

    // Define a vector to hold all course info
    vector<CourseInfo> courses;

    int choice = 0;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Course Info" << endl;
        cout << "  2. Display Course List" << endl;
        cout << "  3. Display Course & Prerequisites" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                courses = loadCourses(csvPath);
                selectionSort(courses);
                break;

            case 2:
                // Loop and display the course info
                for (int i = 0; i < courses.size(); ++i) {
                    displayCourseInfo(courses[i]);
                }
                cout << endl;

                break;

            case 3: {
                string courseChoice;
                bool found = false;
                cout << "Type in desired course: " << endl;
                cin >> courseChoice;
                for (int i = 0; i < courses.size(); ++i) {
                    if (courseChoice == courses[i].courseNumber) {
                        displayCourseInfo(courses[i]);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Your input could not be found in the course list." << endl;
                }
                break;
            }
            
            case 4:
                break;

            default:
                cout << "Enter a valid input" << endl;
                break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}