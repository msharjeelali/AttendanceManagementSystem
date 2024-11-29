#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<sstream>
#include "userclass.h"
#include "attendance.h"
#include "leave.h"

using namespace std;

// Employee class implementation
class Employee : public user {
	// Object of leave record to store information of employye leaves
	leaveRecordI* leaves;
	// Leave interface for employee for apply and cancel leave operations
	leaveInterfaceEmployee* leaveInterface;
	// Leave report interface to generate own leave reports
	leaveReportsEmployeeI* leaveReport;
	// Attendance report interface to generate own attenadance reports
	AttendanceReportEmployeeI* attendanceReport;

	// Functoin to get details of employee
	void getDetails() {
		system("CLS");
		cout << "Employee Details" << endl << endl;
		// Calling function of parent class
		user::getDetails();
		system("Pause");
	}

public:
	// Deafult constructor of employee
	Employee(string id, string firstname, string lastname, int casual, int earned, int official) :user(id, firstname, lastname) {
		// Iniialing all interfaces with objects

		leaves = new leaveRecord(casual, earned, official);
		leaveInterface = new leaveService();
		leaveReport = new leaveReports();
		attendanceReport = new AttendanceReports(id);
	}

	// Function to get leave balance
	int getBalance(int type) {
		// Calling function of leave record class to get balance of specific leave type
		return leaves->getBalance(type);
	}

	// Function to decrement leave balance of employee
	void decBalance(int count, int type) {
		// Calling function of leave record to decrement leave balance
		leaves->decBalance(count, type);
	}

	// Function to increment leave balance of employee
	void incBalance(int count, int type) {
		// Calling function of leave record to decrement leave balance
		leaves->addBalance(count, type);
	}

	// Function to display menu for employee
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			// Display all functions that employee can perfrom
			cout << "Employee Menu" << endl << endl
				<< "1- Employee Details" << endl
				<< "2- Attendence Details" << endl
				<< "3- Apply Leave" << endl
				<< "4- Cancel Leave" << endl
				<< "5- View Leave Details" << endl
				<< "6- Logout" << endl;
			// Get choice of employee between 1 and 5
			choice = validInput(1, 6);

			// Display details of employee
			if (choice == 1)
				this->getDetails();
			// Attendance report interface calling function to display own attendance records
			else if (choice == 2)
				attendanceReport->viewEmployeeAttendance(this->id);
			// Leave service interface calling function to apply for a leave
			else if (choice == 3)
				leaveInterface->applyLeave(this);
			// Leave service interface calling function to cancel for a leave
			else if (choice == 4)
				leaveInterface->cancelLeave(this);
			// Leave record interface calling function to display balance of employee leaves
			// Leave report interface calling function to display own leaves records
			else if (choice == 5) {
				leaves->print();
				leaveReport->viewLeaveRecord(id);
			}
		} while (choice != 6);
		// Update balance of employee when logout
		userFileHandling::updateBalance(this);
	}
};

// Supervisor class implementation
class Supervisor : public user {
	// Attendance report interface to generate different attendance reports
	AttendanceReportAuthorityI* attendaceReport;
	// Leave report interface to generate differnet leave reports
	leaveReportsSupervisorI* leaveReport;
	// Leave service interface to approve or reject leave applications
	leaveInterfaceSupervisor* leaveInterface;

	// Functoin to get details of supervisor
	void getDetails() {
		system("CLS");
		cout << "Supervisor Details" << endl << endl;
		// Calling function of parent class
		user::getDetails();
		system("Pause");
	}
public:
	// Supervisor default constructor
	Supervisor(string id = "NA", string first = "NA", string last = "NA") : user(id, first, last) {
		// Initializing all interfaces with objects
		
		attendaceReport = new AttendanceReports();
		leaveReport = new leaveReports();
		leaveInterface = new leaveService();
	}

	// These function were required since parent class have pure virtual functions which need to be override

	// No functionality in this class
	int getBalance(int type) { return 0; }
	void decBalance(int count, int type) {	}
	void incBalance(int count, int type) {	}

	// Function to display menu for supervisor
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			// Display of supervisor actions
			cout << "Supervisor Menu" << endl << endl
				<< "1- Supervisor Details" << endl
				<< "2- Specific Employee Attendance Record" << endl
				<< "3- View Employee with Less Attendance (Weekly)" << endl
				<< "4- View Employee with Less Attendance (Monthly)" << endl
				<< "5- Process Leave Application" << endl
				<< "6- View Outstanding Leaves" << endl
				<< "7- Logout" << endl;
			// Input of choice between 1 and 7
			choice = validInput(1, 7);
			// Function to display supervisor detials
			if (choice == 1)
				this->getDetails();
			// Attendance report interface calling function to view employee attendance records
			// Any employee all attendance records
			else if (choice == 2)
				attendaceReport->viewEmployeeAttendance();
			// Attendance report interface calling function to view employee attendance records
			// Only employees who havenot completed 40 hours in a week
			else if (choice == 3)
				attendaceReport->viewEmployeeLessAttendance();
			// Attendance report interface calling function to view employee attendance records
			// Only employees who havenot attendance upto specified percentage for a specific month
			else if (choice == 4)
				attendaceReport->viewEmployeeMonthlyAttendance();
			// Leave service interface calling function to process leave applications that are under review by supervisor
			else if (choice == 5)
				leaveInterface->processLeaveSupervisor();
			// Leave report interface calling function to view all leaves that are under review by supervisor
			// Considering it as outstanding leaves
			else if (choice == 6)
				leaveReport->viewOutstandingLeavesSupervisor();
		} while (choice != 7);
	}
};

//Director class implementation
class Director : public user {
	// Attendance report interface to view attendance related reports
	AttendanceReportAuthorityI* attendanceReport;
	// Leave report interface to view leave related reports
	leaveReportsDirectorI* leaveReport;
	// Leave service interface to process leave applications
	leaveInterfaceDirector* leaveInterface;

	// Functoin to get details of director
	void getDetails() {
		system("CLS");
		cout << "Director Details" << endl << endl;
		// Calling function of parent class
		user::getDetails();
		system("Pause");
	}
public:
	// Director default constructor
	Director(string id = "NA", string first = "NA", string last = "NA") : user(id, first, last) {
		// Initializing interfaces with objects
		
		attendanceReport = new AttendanceReports();
		leaveReport = new leaveReports();
		leaveInterface = new leaveService();
	}

	// No functionality in this class
	int getBalance(int type) { return 0; }
	void decBalance(int count, int type) {	}
	void incBalance(int count, int type) {	}

	// Function to display menu for director
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			// Displaying all functions director can perform
			cout << "Director Menu" << endl << endl
				<< "1- Director Details" << endl
				<< "2- Specific Employee Attendance Record" << endl
				<< "3- View Employee with Less Attendance (Weekly)" << endl
				<< "4- View Employee with Less Attendance (Monthly)" << endl
				<< "5- Process Leave Applications" << endl
				<< "6- View Outstanding Leaves" << endl
				<< "7- Logout" << endl;
			// Input of choice of director between 1 and 7
			choice = validInput(1, 7);
			// Fucntion to display detials of supervisor
			if (choice == 1)
				this->getDetails();
			// Attendance report interface calling function to view employee attendance records
			// Any employee all attendance records
			else if (choice == 2)
				attendanceReport->viewEmployeeAttendance();
			// Attendance report interface calling function to view employee attendance records
			// Only employees who havenot completed 40 hours in a week
			else if (choice == 3)
				attendanceReport->viewEmployeeLessAttendance();
			// Attendance report interface calling function to view employee attendance records
			// Only employees who havenot attendance upto specified percentage for a specific month
			else if (choice == 4)
				attendanceReport->viewEmployeeMonthlyAttendance();
			// Leave service interface calling function to process leave applications that are under review by director
			else if (choice == 5)
				leaveInterface->processLeaveDirector();
			// Leave report interface calling function to view all leaves that are under review by director
			// Considering it as outstanding leaves
			else if (choice == 6)
				leaveReport->viewOutstandingLeavesDirector();
		} while (choice != 7);
	}
};

// Guard class implementation
class Guard : public user {
	// Attendance service interface to mark attendance
	AttendanceServiceGuardI* attendanceInterface;

	// Functoin to get details of guard
	void getDetails() {
		system("CLS");
		cout << "Guard Details" << endl << endl;
		// Calling parent class function
		user::getDetails();
		system("Pause");
	}
public:
	// Deafult constructor of guard
	Guard(string id = "NA", string firstname = "NA", string lastname = "NA") :user(id, firstname, lastname) {
		// Initializing interface with objects

		attendanceInterface = new AttendanceService();
	}

	// These function were required since parent class have pure virtual functions which need to be override

	// No functionality in this class
	int getBalance(int type) { return 0; }
	void decBalance(int count, int type) {	}
	void incBalance(int count, int type) {	}

	// Function to display menu for guard
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			// Displaying all fucntions guard can perform
			cout << "Guard Menu" << endl << endl
				<< "1- Guard Details" << endl
				<< "2- Mark Attendence" << endl
				<< "3- Logout" << endl;
			// Input of guard choice between 1 and 3
			choice = validInput(1, 3);
			// Function to display guard details
			if (choice == 1)
				this->getDetails();
			// Attendace service interface calling fucniton to mark attendance to employees
			else if (choice == 2)
				attendanceInterface->markAttendanceGuard(this->getID());
		} while (choice != 3);

	}
};

// User login functionality
// Current user pointer to store object of employee, supervisor, director or guard according to userid and pass
// String userid and pass consist of id and password against that id entered by user to login
bool login(user*& current, string userid, string userpass) {
	bool flag = false;
	// Check if id is of employee
	if (userid[0] == 'e') {
		// Temporary variables to read data from file
		string id, pass, first, last;
		int casual, earned, official;
		// Open employee file in reading mode
		ifstream read("Employee.txt");
		if (read.is_open()) {
			// Loop untill we reach end of eof or employee is found
			while (!read.eof() && !flag) {
				// Read file data in temporary variables
				read >> id >> pass >> first >> last >> casual >> earned >> official;
				// If user entered id and password match any of employee record
				if (userid == id && userpass == pass)		
				{
					// To break loop
					flag = true;
					// Create employee object as current user
					current = new Employee(id, first, last, casual, earned, official);
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	}
	// Check if id is of guard
	else if (userid[0] == 'g') {
		// Temporary variables to read data from file
		string id, pass, first, last;
		// Open guard file in reading mode
		ifstream read("Guard.txt");
		if (read.is_open()) {
			// Loop untill we reach end of eof or guard is found
			while (!read.eof() && !flag) {
				// Read file data in temporary variables
				read >> id >> pass >> first >> last;
				// If user entered id and password match any of employee record
				if (userid == id && userpass == pass)
				{
					// To break loop
					flag = true;
					// Create employee guard as current user
					current = new Guard(id, first, last);
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	}
	// Check if id is of supervisor
	else if (userid[0] == 's') {
		// Temporary variables to read data from file
		string readid, readpass, first, last;
		// Open supervisor file in reading mode
		ifstream read("Supervisor.txt");
		if (read.is_open()) {
			// Loop untill we reach end of eof or supervisor is found
			while (!read.eof() && !flag) {
				// Read file data in temporary variables
				read >> readid >> readpass >> first >> last;
				// If user entered id and password match any of employee record
				if (userid == readid && userpass == readpass)
				{
					// To break loop
					flag = true;
					// Create employee object as current user
					current = new Supervisor(readid, first, last);
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	}
	// Check if id is of director
	else if (userid[0] == 'd') {
		// Temporary variables to read data from file
		string readid, readpass, first, last;
		// Open director file in reading mode
		ifstream read("Director.txt");
		if (read.is_open()) {
			// Loop untill we reach end of eof or director is found
			while (!read.eof() && !flag) {
				// Read file data in temporary variables
				read >> readid >> readpass >> first >> last;
				// If user entered id and password match any of employee record
				if (userid == readid && userpass == readpass)
				{
					// To break loop
					flag = true;
					// Create employee object as current user
					current = new Director(readid, first, last);
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	}
	// Return true if any user with argument is found
	// Return false if id and password doesnot match any user
	return flag;
}

// Main fucntionality
int main()
{
	// Current user how is going to use system
	// Employee, guard, supervisor or director
	user* current = nullptr;
	
	cout << "Welcome to Attendence Management System" << endl;

	// User must login into system to use it
	cout << "User must Login to continue" << endl << endl;
	bool flag = false;
	// Loop until user login with correct credentials
	while (!flag) {
		// User credential input
		string userid, userpass;
		cout << "Enter user id : ";
		cin >> userid;
		cout << "Enter password : ";
		cin >> userpass;
		// Function that initialze current* with specified object depending on the id and password passed
		flag = login(current, userid, userpass);
		// If login return false it means either there is no match for user id
		// Or password entered is wrong
		if (!flag)
			cout << "User not found" << endl;
	}

	// Display menu to user according to its type
	current->menu();

	return 0;
}