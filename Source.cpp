#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<sstream>
#include "attendance.h"
#include "leave.h"

using namespace std;

class user;
bool login();

// Interface of report class for employee
class reportInterfaceEmployee {
public:
	virtual void viewEmployeeAttendance(string) = 0;
};

// Interface of report class for supervisor
class reportInterfaceSupervisor {
public:
	virtual void viewEmployeeAttendance() = 0;
	virtual void viewPendingLeaveSupervisor() = 0;
};

// Interface of report class for director
class reportInterfaceDirector {
public:
	virtual void viewEmployeeAttendance() = 0;
};

// Report class implementation to generate reports of employee's attendance and leaves
class report: public reportInterfaceEmployee, public reportInterfaceSupervisor, public reportInterfaceDirector{
	vector<attendance> attendanceRecord;
	vector<leave*> leaveRecord;

	// Read attendance records of an employee
	void readAttendanceRecords(string empid) {
		ifstream read("Attendance.txt");
		if (read.is_open()) {
			while (!read.eof()) {
				attendance obj;
				read >> obj;
				if(obj.getEmpID() == empid)
					attendanceRecord.push_back(obj);
			}
			read.close();
		}
		else
			cout << "File not found!" << endl;
		sort(attendanceRecord.begin(), attendanceRecord.end());
	}
public:
	report() {

	}

	void viewEmployeeAttendance(string empid) {
		attendanceRecord.clear();
		readAttendanceRecords(empid);
		system("cls");
		if (attendanceRecord.empty() != true) {
			cout << "Employee Attendance Record" << endl << endl;
			
			vector<string>weekday { "monday", "tuesday", "wednesday", "thursday", "friday" };
			string day = attendanceRecord.begin()->getDay();
			transform(day.begin(), day.end(), day.begin(), ::tolower);
			int index = find(weekday.begin(),weekday.end(),day) - weekday.begin() + 1;

			Date rangefrom = attendanceRecord.begin()->getDate()  - index;
			Date rangeto = rangefrom + 6;

			Time sum(0,0);
			for (auto ite : attendanceRecord) {
				if (ite.getDate() > rangeto) {
					cout << "Week Report : " << endl
						<< "From: " << rangefrom << " To: " << rangeto 
						<< " Total Working Hours: " << sum << endl;
					rangefrom = rangeto + 1;
					rangeto = rangefrom + 6;
					cout << "--------------------------------------------------" << endl;
					sum = Time(0,0);
				}
				cout << ite;
				sum = sum + ite.getWorkingHours();
			}
			cout << "Week Report : " << endl
				<< "From: " << rangefrom << " To: " << rangeto
				<< "Total Working Hours: " << sum << endl;
			cout << "--------------------------------------------------" << endl;
		} else
			cout << "No records found" << endl;
		system("pause");
	}
	void viewEmployeeAttendance() {
		int choice = 0;
		do {
			system("cls");
			string empid;
			cout << "Enter employee ID: ";
			cin >> empid;
			viewEmployeeAttendance(empid);
			cout << "1- View another employee records" << endl
				<< "2- Return" << endl;
			choice = validInput(1, 2);
		} while (choice == 1);
	}
	void viewPendingLeaveSupervisor() {
		leaveRecord.clear();
		leave* obj = nullptr;
		ifstream read("PendingSuper.txt");
		if (read.is_open()) {
			string type;
			while (!read.eof()) {
				read >> type;
				if (type == "casual")
					obj = new casual("NA", new pendingSuper());
				else if (type == "earned")
					obj = new earned("NA", new pendingSuper());
				else if (type == "official")
					obj = new official("NA", new pendingSuper());
				else if (type == "unpaid")
					obj = new unpaid("NA", new pendingSuper());
				obj->readRecordFile(read);
				leaveRecord.push_back(obj);
			}
		}
		else
			cout << "File not found!" << endl;

		if (leaveRecord.empty()) {
			cout << "No leaves to process" << endl;
		}
		else {
			for (auto ite : leaveRecord)
				ite->printRecord();
		}
		system("pause");
	}
};

// User class implementation
class user
{
protected:
	string id;				// Store user id
	string firstName;		// Store frist name
	string lastName;		// Store last name
public:
	user(string id = "NA", string firstname = "NA", string lastname = "NA") {
		this->id = id;
		this->firstName = firstname;
		this->lastName = lastname;
	}

	string getID() {
		return id;
	}
	string getName() {
		return firstName + lastName;
	}
	virtual void getDetails() {
		cout << "User id: " << id << endl << "Name: " << firstName << " " << lastName << endl;
	}
	virtual void menu() = 0;
};

// Employee class implementation
class Employee : public user {
	int casual;
	int earned;
	int official;
	bool unpaid;
	leaveInterfaceEmployee* leaveInterface;			// Leave interface for employee
	reportInterfaceEmployee* reportInterface;		// Report interface for employee

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
				reportInterface->viewEmployeeAttendance(this->id);		// View own attendance records
			else if (choice == 3)
				leaveInterface->applyLeave(id);		// Apply for the leave
		} while (choice != 4);
	}
};

// Supervisor class implementation
class Supervisor : public user {
	reportInterfaceSupervisor* reportInterface;		// Report interafce for supervisor
	leaveInterfaceSupervisor* leaveInterface;

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
		reportInterface = new report();
		leaveInterface = new leaveService();
	}

	// Function to display menu for supervisor
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Supervisor Menu" << endl << endl
				<< "1- Guard Details" << endl
				<< "2- Specific Employee Attendance Record" << endl
				<< "3- Process Leave Application" << endl
				<< "4- View Outstanding Leaves" << endl
				<< "5- Logout" << endl;
			choice = validInput(1, 5);
			if (choice == 1)
				this->getDetails();							// Get supervisor details
			else if (choice == 2)
				reportInterface->viewEmployeeAttendance();	// View employee attendance 
			else if (choice == 3) {
				reportInterface->viewPendingLeaveSupervisor();	// Show supervisor which leaves are outstanding so he can process
				leaveInterface->processLeaveSuper();		// Process leave applications
			}
			else if (choice == 4)
				reportInterface->viewPendingLeaveSupervisor(); // View supervisor outstanding leaves
		} while (choice != 5);
	}
};

 //Director class implementation
class Director : public user {
	reportInterfaceDirector* reportInterface;		// Report interface for director

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
		reportInterface = new report();
	}

	// Function to display menu for director
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Director Menu" << endl << endl
				<< "1- Director Details" << endl
				<< "2- Specific Employee Attendance Record" << endl
				<< "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();						// Get guard details
			else if (choice == 2)
				reportInterface->viewEmployeeAttendance();
		} while (choice != 3);
	}
};

// Guard class implementation
class Guard : public user {
	attendanceService* attendanceInterface;

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
		attendanceInterface = new attendanceService(id);
	}
	
	// Function to display menu for guard
	void menu() {
		int choice = 0;
		do {
			system("CLS");
			cout << "Guard Menu" << endl << endl
				<< "1- Guard Details" << endl
				<< "2- Mark Attendence" << endl
				<< "3- Logout" << endl;
			choice = validInput(1, 3);
			if (choice == 1)
				this->getDetails();						// Get guard details
			else if (choice == 2)
				attendanceInterface->markAttendance();	// Mark attendance of employee
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
			while (!read.eof() && !flag) {
				read >> id >> pass >> first >> last >> casual >> earned >> official >> unpaid;
				if (userid == id && userpass == pass)		// If employee entered correct username and password
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
			while (!read.eof() && !flag) {
				read >> id >> pass >> first >> last;
				if (userid == id && userpass == pass)		// If guard entered correct username and password
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
			while (!read.eof() && !flag) {
				read >> readid >> readpass >> first >> last;
				if (userid == readid && userpass == readpass)		// If supervisor entered correct username and password
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
			while (!read.eof() && !flag) {
				read >> readid >> readpass >> first >> last;
				if (userid == readid && userpass == readpass)		// If employee entered correct username and password
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
