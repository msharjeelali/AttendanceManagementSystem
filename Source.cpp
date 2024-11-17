#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include "attendance.h"
#include "leave.h"

using namespace std;

bool login();

// Interface of report class for employee
class reportInterfaceEmployee {
public:
	virtual void viewEmployeeAttendance(string) = 0;
};

// Interface of report class for supervisor and director
class reportInterfaceAuthority {
public:
	virtual void viewEmployeeAttendance() = 0;
};

// Report class implementation to generate reports of employee's attendance and leaves
class report: public reportInterfaceEmployee{
protected:
	vector<attendance> attendanceRecord;

public:
	report() {
		ifstream read("Attendence.txt");
		if (read.is_open()) {
			while (!read.eof()) {
				attendance obj;
				read >> obj;
				attendanceRecord.push_back(obj);
			}
		}
		else
			cout << "File not found!" << endl;
		sort(attendanceRecord.begin(), attendanceRecord.end());
	}

	void viewEmployeeAttendance(string empid) {
		bool flag = false;
		cout << "Employee Attendance Record" << endl << endl;
		for (auto ite : attendanceRecord) {
			if (ite.getEmpID() == empid) {
				cout << ite;
				flag = true;
			}
		}
		if (!flag)
			cout << "No records found" << endl;
		system("pause");
	}

	void viewEmployeeAttendance() {
		system("cls");
		string empid;
		cout << "Enter employee ID: ";
		cin >> empid;
		viewEmployeeAttendance(empid);
	}
};

// User class implementation
class user
{
protected:
	string id;				// Store user id
	string firstName;		// Store frist name
	string lastName;		// Store last name

	friend class leaveService;
public:
	user(string id = "NA", string firstname = "NA", string lastname = "NA") {
		this->id = id;
		this->firstName = firstname;
		this->lastName = lastname;
	}

	virtual void getDetails() {
		cout << "User id: " << id << endl << "Name: " << firstName << " " << lastName << endl;
	}

	string getID() {
		return id;
	}
	
	string getName() {
		return firstName + lastName;
	}

	virtual void menu() = 0;
};

// Employee class implementation
class Employee : public user {
	int casual;
	int earned;
	int official;
	bool unpaid;
	leaveInterfaceEmployee* leaveInterface;
	reportInterfaceEmployee* reportInterface;

	// Functoin to get details of employee
	void getDetails() {
		system("CLS");
		cout << "Employee Details" << endl << endl;
		user::getDetails();
		cout << "Availible Leaves: Casual = " << casual << endl;
		cout << "\t  Earned = " << earned << endl;
		cout << "Official Leaves = " << official << endl;
		cout << "Unpaid Leave Status = " << (unpaid ? "Active" : "Not Active") << endl;
		system("Pause");
	}
public:
	// Deafult constructor of employee
	Employee(string id, string firstname, string lastname, int casual, int earned, int official, bool unpaid) :user(id, firstname, lastname) {
		this->casual = casual;
		this->earned = earned;
		this->official = official;
		this->unpaid = unpaid;
		leaveInterface = new leaveService();
		reportInterface = new report();
	}

	// Function to display menu for employee
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Employee Menu" << endl << endl
				<< "1- Employee Details" << endl
				<< "2- Attendence Details" << endl
				<< "3- Apply Leave" << endl
				<< "4- Logout" << endl;
			choice = validInput(1, 4);
			if (choice == 1)
				this->getDetails();			// Get details of employee
			else if (choice == 2)
				reportInterface->viewEmployeeAttendance(id);		// View own attendance records
			else if (choice == 3)
				leaveInterface->applyLeave(id,firstName+lastName);		// Apply for the leave
		} while (choice != 4);
	}
};

// Supervisor class implementation
class Supervisor : public user {
	// Functoin to get details of supervisor
	void getDetails() {
		system("CLS");
		cout << "Supervisor Details" << endl << endl;
		user::getDetails();
		system("Pause");
	}
public:
	// Supervisor default constructor
	Supervisor(string id = "NA",string first = "NA",string last = "NA") : user(id,first,last){

	}

	// Function to display menu for supervisor
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Supervisor Menu" << endl << endl;
			cout << "1- Guard Details" << endl;
			cout << "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();						// Get guard details
		} while (choice != 3);
	}
};

// Director class implementation
class Director : public user {
	// Functoin to get details of director
	void getDetails() {
		system("CLS");
		cout << "Director Details" << endl << endl;
		user::getDetails();
		system("Pause");
	}
public:
	// Director default constructor
	Director(string id = "NA", string first = "NA", string last = "NA") : user(id, first, last) {

	}

	// Function to display menu for director
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Director Menu" << endl << endl;
			cout << "1- Director Details" << endl;
			cout << "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();						// Get guard details
		} while (choice != 3);
	}
};

// Guard class implementation
class Guard : public user {

	// Functoin to get details of guard
	void getDetails() {
		system("CLS");
		cout << "Guard Details" << endl << endl;
		user::getDetails();
		system("Pause");
	}

public:
	// Deafult constructor of guard
	Guard(string id = "NA", string firstname = "NA", string lastname = "NA") :user(id, firstname, lastname) {
		
	}
	
	// Function to display menu for guard
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
				this->getDetails();						// Get guard details
		} while (choice != 3);
	}
};

// Current user (employee or guard)
user* current;

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
		string readid, readpass, first, last;
		ifstream read("Supervisor.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> readid >> readpass >> first >> last;
				if (userid == readid and userpass == readpass)		// If supervisor entered correct username and password
				{
					flag = true;
					current = new Supervisor(readid, first, last);	// Create supervisor object as user
				}
			}
		}
		else
			cout << "File Not Found!" << endl;
		read.close();
	} // Checking if director exist
	else if (userid[0] == 'd') {
		string readid, readpass, first, last;
		ifstream read("Director.txt");
		if (read.is_open()) {
			while (!read.eof() and !flag) {
				read >> readid >> readpass >> first >> last;
				if (userid == readid and userpass == readpass)		// If employee entered correct username and password
				{
					flag = true;
					current = new Director(readid, first, last);	// Create director object as user
				}
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
