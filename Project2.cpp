//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Misty-Ann Mordoff
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// define a structure to hold course ID, course Name, and vector for prereqs
struct Course {
    string courseNum;
    string courseName;
    vector < string > preReqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printSchedule(Node* node);
    void printcourseDetails(Node* node, string courseNum);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void deleteRepeat(Node* node);
    void Insert(Course course);
    int numPreReqCourses(Course course);
    void printSchedule();
    void printcourseDetails(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr; //root = NULL
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteRepeat(root);
}

/**
 * Delete nodes as needed
 */
void BinarySearchTree::deleteRepeat(Node* node) {
    if (node) {
        deleteRepeat(node->left);
        deleteRepeat(node->right);
        delete node;
    }
}

/**
 * Insert node into BST
 */

void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
      // root is equal to new node bid
    // else
      // add Node root and bid
    if (root == nullptr) {
        root = new Node(course);
    }

    else {
        this->addNode(root, course);
    }
}

/**
 * Returns prereqs needed
 */
int BinarySearchTree::numPreReqCourses(Course course) {
    int count = 0;
    for (unsigned int p = 0; p < course.preReqs.size(); p++) {
        if (course.preReqs.at(p).length() > 0) {
            count++;
        }
    }

    return count;
}

/**
 * BST to private method
 */
void BinarySearchTree::printSchedule() {
    this->printSchedule(root);
}

/**
 * search for course ID
 */
void BinarySearchTree::printcourseDetails(string courseNum) {
    this->printcourseDetails(root, courseNum);
}

/*
* Traverse BST to set course location in BST
*/
void BinarySearchTree::addNode(Node* node, Course course) {
    //current course < current node course
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }

        else {
            this->addNode(node->left, course);
        }
    }

    else {
        //current course > or = current node course
        if (node->right == nullptr) {
            node->right = new Node(course);
        }

        else {
            this->addNode(node->right, course);
        }
    }
}

/*
* outputs courses in order
*/
void BinarySearchTree::printSchedule(Node* node) {
    if (node != nullptr) {
        printSchedule(node->left);

        cout << node->course.courseNum << ", " << node->course.courseName << endl;

        printSchedule(node->right);
    }

    return;
}

/*
*Displays single course with prereq
*/
void BinarySearchTree::printcourseDetails(Node* curr, string courseNum) {
    //traverse BST until match found
    while (curr != nullptr) {
        if (curr->course.courseNum.compare(courseNum) == 0) {
            cout << endl;
            cout << curr->course.courseNum << ", " << curr->course.courseName << endl;

            unsigned int size = numPreReqCourses(curr->course);
            cout << "Prerequisite(s) needed: ";

            //if prereq exist, output prereq
            unsigned int p = 0;
            for (p = 0; p < size; p++) {
                cout << curr->course.preReqs.at(p);

                if (p != size - 1) {
                    cout << ", ";
                }
            }

            //no prereqs and stop search
            if (p == 0) {
                cout << "No prerequisites required for this course." << endl;
            }

            cout << endl;

            return;
        }

        // if courseNum is small than current course num traverse left
        else if (courseNum.compare(curr->course.courseNum) < 0) {
            curr = curr->left;
        }

        //if courseNum is larger than current cousreNum traverse right
        else {
            curr = curr->right;
        }

        //course has not been found during search
        cout << "Course " << courseNum << " not found." << endl;
    }
}

//============================================================================
// Static methods used for testing
//============================================================================


/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
bool loadCourses(string csvPath, BinarySearchTree* binarytree) {
    //open file and separate into BST
	try {
		ifstream courseFile(csvPath);

		if (courseFile.is_open()) {
			while (!courseFile.eof()) {

				//use vector to hold course
				vector < string > courseInfo;
				string courseData;

				getline(courseFile, courseData);

				while (courseData.length() > 0) {
					unsigned int comma = courseData.find(',');

					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}

					else {
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}

				//load values into course and insert into bst and close file
				Course course;
				course.courseNum = courseInfo[0];
				course.courseName = courseInfo[1];

				for (unsigned int p = 2; p < courseInfo.size(); p++) {
					course.preReqs.push_back(courseInfo[p]);
				}

				binarytree->Insert(course);
			}
		}

		courseFile.close();
		return true;
	}

	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}

	return false;
}

	/*cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Course course;
            course.courseName = file[i][1];
            course.courseNum = file[i][0];


            // push this course to the end
            binarytree->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
*/

//outputs menu to user
void printMenu(){
    cout << "  1. Load Data Structures" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseId = argv[2];
        break;
    default:
        csvPath = "";
        break;
    }

    // Define a binary search tree to hold all bids
    BinarySearchTree* binarytree = nullptr;

    //greet user
    cout << "\nWelcome to the course planner!\n" << endl;


    //validate user input
    string choice = "0";
    int userChoice = choice[0] - '0';



    //if user input is not 9
    while (userChoice != 9) {
        printMenu();
        cin >> choice;

        //check user input to very single digit number
        if (choice.length() == 1) {
            userChoice = choice[0] - '0';
        }

        else {
            userChoice = 0;
        }

        bool successful = 1;


        switch (userChoice) {

        case 1:

            if (binarytree == nullptr) {
                binarytree = new BinarySearchTree;
            }

            if (csvPath.length() == 0) {
                cout << "Enter desired file you would like to load";
                cout << endl << "The current file option is: ABCU Advising Program Input";
                cin >> csvPath;
            }

            //determine if file was opened
            successful = loadCourses(csvPath, binarytree);

            if (successful) {
                cout << "Courses have been loaded.\n";
                cout << endl << endl;;

                printMenu();
                cin >> choice;

            }

            else {
                cout << "ERROR File not found!\n";
                cout << endl << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }

            }

            csvPath = "";

            break;



        case 2:
        	if (binarytree != nullptr && successful) {
        		cout << "Here is a sample schedule: \n";
        		binarytree->printSchedule();
        		cout << endl;
        		cout << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }

        	}

        	else {
        		cout << "Please load the courses first.\n";
        		cout << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }
        	}

        	break;

        case 3:
        	if (binarytree != nullptr && successful) {
        		if (courseId.length() == 0) {
        			cout << "\nWhat course do you want to know about?\n";

        			cin >> courseId;

        			for (auto& userChoice : courseId) userChoice = toupper(userChoice);
        		}

        		binarytree->printcourseDetails(courseId);

        		cout << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }
        	}

        	else {
        		cout << "Please load the courses first.\n";
        		cout << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }

        	}

        	courseId = "";

        	break;

        	//invalid data
        default:
        	if (userChoice == 9) {
        		cout << "\nThank you for using the course planner!" << endl;
        	}

        	if (userChoice != 1 && userChoice != 2 && userChoice != 3 && userChoice != 9) {
        		cout << choice << " is not a valid option\n" << endl;
                printMenu();
                cin >> choice;

                //check user input to very single digit number
                if (choice.length() == 1) {
                    userChoice = choice[0] - '0';
                }

                else {
                    userChoice = 0;
                }
        	}
	}
return 0;
}
}
