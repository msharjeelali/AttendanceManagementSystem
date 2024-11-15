#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include "attendance.h"

using namespace std;

// Defining classes and functoin prototypes for ease of use;
class user;
class Employee;
class Guard;
class report;
class reportInterfaceEmployee;

int validInput(int, int);
bool login();

// Report class implementation to generate reports of employee's attendance and leaves
class report
{
protected:
	vector<attendance> attendanceRecord;

	// Function to read attendance record from file and store it in attendance record
	void readAttendanceRecords(string emp) {
		bool flag = false;
		string temp;
		ifstream read("Attendence.txt");
		if (read.is_open()) {
			attendance recordFromFile;
			while (!read.eof())
			{
				read >> recordFromFile;
				if (recordFromFile.getEmpID() == emp)		// Read record from file and check if record is of given object
					attendanceRecord.push_back(recordFromFile);
			}
		}
		else
			cout << "File not found!" << endl;	
	}

	// Function to view attendance record of an employee
	void viewEmpAttendance() {
		system("CLS");
		if (attendanceRecord.empty())			// If no records are found
			cout << "No records found!" << endl;
		else {
			for (auto ite : attendanceRecord)
				cout << ite;
		}
	}
};

// Interface of report class to restrict employee to specific number of functions
class reportInterfaceEmployee : public report {
public:
	// Deafult constructor
	reportInterfaceEmployee(string emp) {
		readAttendanceRecords(emp);									// Read attendance records of given employee in vector of attendance
		sort(attendanceRecord.begin(), attendanceRecord.end());		// Sort attendance records according to date to record
	}

	// Function to view employee attendance records
	void viewEmpAttendance() {
		report::viewEmpAttendance();
		system("Pause");
	}
};

// User class implementation
class user
{
protected:
	string id;
	string firstName;
	string lastName;

	virtual void getDetails() = 0;
public:
	user(string id = "NA", string firstname = "NA", string lastname = "NA") {
		this->id = id;
		this->firstName = firstname;
		this->lastName = lastname;
	}

	string getID() {
		return id;
	}
	virtual void menu() = 0;
};

class Employee : public user {
	int casual;
	int earned;
	int official;
	bool unpaid;
	reportInterfaceEmployee* interface;

	void getDetails() {
		system("CLS");
		cout << "Employee Details" << endl << endl;
		cout << "Name: " << firstName << " " << lastName << endl;
		cout << "ID:   " << id << endl;
		cout << "Availible Leaves: Casual = " << casual << endl;
		cout << "\t  Earned = " << earned << endl;
		cout << "Official Leaves = " << official << endl;
		cout << "Unpaid Leave Status = " << (unpaid ? "Active" : "Not Active") << endl;
		system("Pause");
	}
public:
	Employee(string id, string firstname, string lastname, int casual, int earned, int official, bool unpaid) :user(id, firstname, lastname) {
		this->casual = casual;
		this->earned = earned;
		this->official = official;
		this->unpaid = unpaid;
		interface = new reportInterfaceEmployee(id);
	}

	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Employee Menu" << endl << endl;
			cout << "1- Employee Details" << endl;
			cout << "2- Attendence Details" << endl;
			cout << "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();
			else if (choice == 2)
				interface->viewEmpAttendance();
		} while (choice != 3);
	}
};

class Guard : public user {
	attendanceService* Interface;

	void getDetails() {
		system("CLS");
		cout << "Guard Details" << endl << endl;
		cout << "Name: " << firstName << " " << lastName << endl;
		cout << "ID: " << id << endl;
		system("Pause");
	}

	void markAttendence() {
		Interface->markAttendance();
	}
public:
	Guard(string id, string firstname, string lastname) :user(id, firstname, lastname) {
		Interface = new attendanceService(id);
	}

	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Guard Menu" << endl << endl;
			cout << "1- Guard Details" << endl;
			cout << "2- Mark Attendence" << endl;
			cout << "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();
			else if (choice == 2)
				this->markAttendence();
		} while (choice != 3);
	}
};

// Current user (employee or guard)
user* current;

// Function to validate menu input
int validInput(int start, int end) {
	int choice;
	do {
		cout << "Enter your choice: ";
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
		}
	} while (choice< start or choice > end);
	return choice;
}

// User login functionality and choosing the user who logged in
bool login() {
	system("CLS");
	cout << "User must Login to continue" << endl << endl;

	// User credential input
	string userid, userpass;
	cout << "Enter user id : ";
	cin >> userid;
	cout << "Enter password : ";
	cin >> userpass;


	bool flag = false;
	// Checking if employee exist
	if (userid[0] == 'e') {
		string id, pass, first, last;
		int casual, earned, official, unpaid;
		ifstream read("Employee.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> id >> pass >> first >> last >> casual >> earned >> official >> unpaid;
				if (userid == id and userpass == pass)		// If employee entered correct username and password
				{
					flag = true;
					current = new Employee(id, first, last, casual, earned, official, unpaid);			// Create employee object as user
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	} // Checking if guard exist
	else if (userid[0] == 'g') {
		string id, pass, first, last;
		ifstream read("Guard.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> id >> pass >> first >> last;
				if (userid == id and userpass == pass)		// If guard entered correct username and password
				{
					flag = true;
					current = new Guard(id, first, last);		// Create guard object as user
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	} // Checking if supervisor exist
	else if (userid[0] == 's') {
		string readid, readpass;
		ifstream read("Supervisor.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> readid;
				read >> readpass;
				if (userid == readid and userpass == readpass)		// If supervisor entered correct username and password
					flag = true;
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	} // Checking if director exist
	else if (userid[0] == 'd') {
		string readid, readpass;
		ifstream read("Director.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> readid;
				read >> readpass;
				if (userid == readid and userpass == readpass)		// If employee entered correct username and password
					flag = true;
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	}
	return flag;
}

int main()
{
	cout << "Welcome to Attendence Management System" << endl;
	system("Pause");

	while (!login()) {
		cout << "Invalid Credentials" << endl;
		system("Pause");
	}

	current->menu();

	return 0;
}
