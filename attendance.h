#pragma once

// This file contain all attendance related classes and thier implementations
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<iomanip>
#include "helper.h"
#include "userclass.h"

using namespace std;

// Attendance class used to store attendance record of an employee
class attendance {
	// Stores employee id
	string empid;
	// Stroes guard id 
	string guardid;
	// Attendance date 
	Date date;
	// Time of entry
	Time entry;
	// Time of exit
	Time exit;
	// Remark of attendance ( Marked by guard or leave)
	string remark;

	friend class AttendanceFileHandling;
	friend class AttendanceConsoleHandling;
public:
	// Deafault constructor
	attendance(string empid = "NA", string guardid = "NA", string remark = "NA") {
		this->empid = empid;
		this->guardid = guardid;
		this->remark = remark;
	}

	// Overloaded construstor
	attendance(string empid, string guardid, Date date, Time entry, Time exit, string remark) {
		this->empid = empid;
		this->guardid = guardid;
		this->date = date;
		this->entry = entry;
		this->exit = exit;
		this->remark = remark;
	}

	// Copy constructor
	attendance(const attendance& obj) {
		empid = obj.empid;
		guardid = obj.guardid;
		date = obj.date;
		entry = obj.entry;
		exit = obj.exit;
		remark = obj.remark;
	}

	// Function to get id of employee whose attendance record is it
	string getEmpID() const {
		return empid;
	}

	// Function to get date of attendance record
	Date getDate() const {
		return date;
	}

	// Function to get working hours on given attendance record
	Time getWorkingHours() const {
		// Minus operator overloaded in Time class
		return exit - entry;
	}

	// Function to set guardid of record
	void setGuard(string guardid) {
		this->guardid = guardid;
	}

	// Overloading less than operator to sort attendance record based on date field
	bool operator < (attendance& obj) const {
		// < operator overloaded in Date class
		return date < obj.date;
	}

	// Function to print attendance records in file
	void printAttendanceRecord() {
		cout << " " << empid << "\t" << guardid << "\t" << date << "\t" << date.getDay() 
			<< "\t" << entry << "\t" << exit << "\t" << getWorkingHours() << "\t" << remark << endl;
	}
};

// Class to handle all attendance related writing and readings
class AttendanceFileHandling {
	// Store name of file dealing with attendance record
	string fileName;
public:
	AttendanceFileHandling() {
		fileName = "Attendance.txt";
	}

	// Function to write attendance records in file that are passed as vector
	void writeAttendanceRecords(const vector<attendance> record) {
		// Open file in write append mode
		ofstream write(fileName, ios::app);
		if (write.is_open()) {
			// Write all the attendance that are marked by guard
			for (auto ite : record)
				write << endl << ite.empid << " " << ite.guardid << " " << ite.date << " " << ite.entry << " " << ite.exit << " " << ite.remark;
			write.close();
		}
		else
			cout << "Attendance file couldnot be opened!" << endl;
	}

	// Function to read attendance records from file the vector passed as parameter
	// If no empid is passed read all the records and if empid is passed read records of that employee
	void readAttendanceRecords(vector<attendance>& record, const string empid = "default") {
		cout << "Reading attendance records....." << endl;
		// Open file in read mode to read attendance records
		ifstream read(fileName);
		if (read.is_open()) {
			// Temporary object to store attendance record
			attendance object;
			while (!read.eof()) {
				// Read attendance details in temporary object
				read >> object.empid >> object.guardid >> object.date >> object.entry >> object.exit >> object.remark;
				// If empid is passed than it will only read that specific employee record.
				// If empid is not passed than it will read all the records from attendance file.
				if (object.empid == empid || empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Unable to open Attendance file!" << endl;
		// Built-in sort fucntion to sort attendance records
		sort(record.begin(), record.end());
	}

	// Function that helps to overcome duplicate attendance records of employee for a given date
	// Function to check if attendance record of passed date and employee already exist or not
	bool checkAttendanceRecord(const string empid, const Date date) {
		bool flag = false;
		// Open file to read records
		ifstream read(fileName);
		if (read.is_open()) {
			// Temporary attendance object
			attendance object;
			while (!read.eof() && !flag) {
				// Read attendance details in temporary object
				read >> object.empid >> object.guardid >> object.date >> object.entry >> object.exit >> object.remark;
				// If empid is passed than it will only read that specific employee record.
				// If empid is not passed than it will read all the records from attendance file.
				if (object.empid == empid && object.date == date)
					flag = true;
			}
		}
		else
			cout << "Unable to open Attendance file!" << endl;
		return flag;
	}
};

// Attendance service interface for guard to mark attendance of employees
class AttendanceServiceGuardI {
public:
	virtual void markAttendanceGuard(string) = 0;
};

// Attendance service interface for leaveservice to award credits for applied leaves
class AttendanceServiceLeaveI {
public:
	virtual bool checkAttendanceRecord(string, Date) = 0;
	virtual void markAttendanceLeave(string, Date, Date) = 0;
};

// Class that provide attendance services
class AttendanceService : public AttendanceServiceGuardI, public AttendanceServiceLeaveI {
	// Store currently marked attendance records
	vector<attendance> empAttendanceRecords;
	// Reference to object of Attendance File Handling Class
	AttendanceFileHandling* fileHandling;
public:
	AttendanceService() {
		// Initializing reference with objects
		fileHandling = new AttendanceFileHandling();
	}

	// Function to mark attendance of employee for guards
	void markAttendanceGuard(string guardid) {
		int choice = 0;
		do {
			// Read attendance record details from guard
			cout << "Mark Employee Attendance" << endl << endl;

			// Temporary object to read attendance details
			attendance object;
			
			// Input of correct employee id
			bool flag = false;
			string inputEmpID;
			
			// Loop untill correct employee id entered by guard
			while (!flag) {
				cout << "Enter employee ID: ";
				cin >> inputEmpID;
				// Check if employee exist against that input id
				if (userFileHandling::checkEmployeeExistance(inputEmpID))
					flag = true;
				else
					cout << "Employee not found! Try Again" << endl;
			}

			// If employee od is correct than proceed
			if (flag) {
				// Input of correct date
				Date recordDate;
				flag = false;
				// Loop until correct date is enterd in specified format ( DD/MM )
				while (!flag) {
					cout << "Enter date i.e. ( DD/MM ): ";
					cin >> recordDate;
					if (recordDate.valid())
						flag = true;
					// Check if date entered is of Saturday or Sunday
					// Cannot mark attendance of Saturday and Sunday
					if (recordDate.getDay() == "Sun" or recordDate.getDay() == "Sat") {
						flag = false;
						cout << "You are marking attendance of weekend which is not allowed" << endl;
					}
				}

				// If correct date is eneterd than proceed forward
				if (flag) {
					// Input of entry and exit time in office of employee
					Time entryTime, exitTime;
					flag = false;
					// Loop till correct time is enterwd
					while (!flag) {
						cout << "Enter entry time i.e. HH:MM (24 hour format): ";
						cin >> entryTime;
						if (!entryTime.valid()) {
							cout << "Invalid entry time entered!" << endl;
							continue;
						}

						cout << "Enter exit time i.e. HH:MM (24 hour format): ";
						cin >> exitTime;
						if (!exitTime.valid())
						{
							cout << "Invalid exit time entered!" << endl;
							continue;
						}

						// Check if entry time is earlier than exit
						if (Time::checktime(entryTime, exitTime))
							flag = true;
						else
							cout << "Ambigous entry and exit time!" << endl;
					}
					// If all details are entered correctly return attendance object
					object = attendance(inputEmpID, "", recordDate, entryTime, exitTime, "Regular");
				}
			}
			
			// Set your id as guard id
			object.setGuard(guardid);
			
			// Check if attendance record of entered date is already present or not
			if (fileHandling->checkAttendanceRecord(object.getEmpID(), object.getDate()))
				cout << "Attendance record of " << object.getEmpID() << " already exist!" << endl;
			
			// If not present than proceed and check if guard already marked attendance of that employee
			else {
				bool flag = true;
				for (auto ite : empAttendanceRecords) {
					if (ite.getEmpID() == object.getEmpID() && ite.getDate() == object.getDate()) {
						cout << "Attendance record of " << object.getEmpID() << " already exist!" << endl;
						flag = false;
					}
				}
				
				// If not marked and all checks are satisfied push object in records
				if (flag)
					empAttendanceRecords.push_back(object);
			}
			cout << "1- Mark another attendance" << endl
				<< "2- Return" << endl;
			choice = validInput(1, 2);
		} while (choice == 1);
		// Once guard finished marking all attendances write all of them in file
		fileHandling->writeAttendanceRecords(empAttendanceRecords);
	}

	// Function to mark attendance of employee when leave application is approved
	void markAttendanceLeave(string empid, Date from, Date till) {
		// Loop for each date of approved leave application
		for (Date i = from; i < till or i == till; i = i + 1) {
			// If day is not Saturday or Sunday
			if (i.getDay() != "Sun" && i.getDay() != "Sat") {
				// Create attendance object of that date
				attendance object(empid, "NotAvailable", i, Time(9, 0), Time(17, 0), "Leave");
				// Push object in vector of attendance
				empAttendanceRecords.push_back(object);
			}
		}
		// Write attendance records in file
		fileHandling->writeAttendanceRecords(empAttendanceRecords);
	}

	// Function to check attendance record of employee for specific date
	bool checkAttendanceRecord(string empid, Date date) {
		return fileHandling->checkAttendanceRecord(empid, date);
	}
};

// Report interafce for employee
class AttendanceReportEmployeeI {
public:
	virtual void viewEmployeeAttendance(string) = 0;
};

// Report interafce for director and supervisor
class AttendanceReportAuthorityI {
public:
	virtual void viewEmployeeAttendance() = 0;
	virtual void viewEmployeeLessAttendance() = 0;
	virtual void viewEmployeeMonthlyAttendance() = 0;
};

// Class that generate attendance related reports
class AttendanceReports : public AttendanceReportEmployeeI, public  AttendanceReportAuthorityI {
	// Store attendance records
	vector<attendance> record;
	// Pointer to attendance file handling class
	AttendanceFileHandling* fileHandling;
public:
	AttendanceReports(string empid="default") {
		// Initializing pointers with objects
		fileHandling = new AttendanceFileHandling();
		// Read attendance record based on empid
		// If employee id is passed read that specific emplyee records
		// If employee id is default read all employees attendance records
		fileHandling->readAttendanceRecords(record,empid);
	}

	// Function for employee to generate report of attendance records
	void viewEmployeeAttendance(string empid) {
		system("cls");
		// Chcek if records are empty or not
		if (record.empty() != true) {
			cout << "Employee Attendance Record" << endl << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << " Employee ID\tGuard ID\tDate\t\tDay\tEntry\tExit\tHours\tRemarks" << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;


			Date rangefrom = Date::getWeekRange(record.begin()->getDate(), record.begin()->getDate().getDay());
			Date rangeto = rangefrom + 6;

			Time sum(0, 0);
			// Iterate through all the attendance records
			for (auto ite : record) {
				if (ite.getEmpID() == empid) {
					// Since all the attendance records are sorted so check if record date exceed week range
					// If it exceed range than calculate next week range
					while (!ite.getDate().between(rangefrom, rangeto)) {
						cout << "----------------------------------------------------------------------------------------" << endl;
						cout << "Week Report : " << "From: " << rangefrom << " To: " << rangeto << " Total Working Hours: " << sum << endl;
						cout << "----------------------------------------------------------------------------------------" << endl;
						rangefrom = rangeto + 1;
						rangeto = rangefrom + 6;
						sum = Time(0, 0);
					}

					// Print attendance record
					ite.printAttendanceRecord();
					// Sum working hours of week
					sum = sum + ite.getWorkingHours();
				}
			}
			// Print last week details since loop will terminate at last record but last week record will not be printed
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << "Week Report : " << "From: " << rangefrom << " To: " << rangeto << " Total Working Hours: " << sum << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
		}
		else
			cout << "No records found" << endl;
		system("pause");
	}

	// Function for supervisor and director to view employee attendance record of any employee
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

	// Function to view attendance records of employee with attendance less than specified percentage
	void viewEmployeeLessAttendance(string empid) {
		// Chcek if records are empty or not
		if (record.empty() != true) {
			Date rangefrom = Date::getWeekRange(record.begin()->getDate(), record.begin()->getDate().getDay());
			Date rangeto = rangefrom + 6;

			vector<attendance> buffer;

			Time sum(0, 0);
			// Iterate through all the attendance records
			for (auto ite : record) {
				if (ite.getEmpID() == empid) {
					// Since all the attendance records are sorted so check if record date exceed week range
					// If it exceed range than calculate next week range
					if (ite.getDate() > rangeto) {
						if (sum.getHours() < 40) {
							cout << "----------------------------------------------------------------------------------------" << endl;
							cout << " Employee ID\tGuard ID\tDate\t\tDay\tEntry\tExit\tHours\tRemarks" << endl;
							cout << "----------------------------------------------------------------------------------------" << endl;
							if (buffer.empty())
								cout << "No records found" << endl;
							for (auto itj : buffer) {
								itj.printAttendanceRecord();
							}
							cout << "----------------------------------------------------------------------------------------" << endl;
							cout << "Week Report :  " << "From = " << rangefrom << " To = " << rangeto << " Total Working Hours = " << sum << endl;
						}
						rangefrom = rangeto + 1;
						rangeto = rangefrom + 6;
						sum = Time(0, 0);
						buffer.clear();
					}

					buffer.push_back(ite);
					// Sum working hours of week
					sum = sum + ite.getWorkingHours();
				}
			}
			// Print last week details since loop will terminate at last record but last week record will not be printed
			if (sum.getHours() < 40) {
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << " Employee ID\tGuard ID\tDate\t\tDay\tEntry\tExit\tHours\tRemarks" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
				if (buffer.empty())
					cout << "No records found" << endl;
				for (auto itj : buffer) {
					itj.printAttendanceRecord();
				}
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Week Report :  " << "From = " << rangefrom << " To = " << rangeto << " Total Working Hours = " << sum << endl;
			}
		}
		else
			cout << "No records found" << endl;
	}

	// Function to view employees with attendance less than specified percentage
	void viewEmployeeLessAttendance() {
		system("cls");
		cout << "Employee Attendance Records" << endl << endl;
		vector<string> listUserID = userFileHandling::getAllEmployee();
		for (auto ite : listUserID) {
			cout << "Employee ID: " << ite << endl;
			viewEmployeeLessAttendance(ite);
			cout << endl << endl;
		}
		system("pause");
	}

	// Function for employee to generate report of attendance records monthly
	void viewEmployeeMonthlyAttendance(string empid, int mon, int per) {
		// Time object to sum working hours
		Time sum(0, 0);
		// Buffer to store attendance reords
		vector<attendance> buffer;

		// Iterate all the records
		for (auto& ite : record) {
			// If month matches than sum working hours of all record
			if (mon == ite.getDate().getMonth() && empid == ite.getEmpID())
			{
				sum = sum + ite.getWorkingHours();
				buffer.push_back(ite);
			}
		}

		// Calculating total number of working days and hours to generate report
		int total = 0;
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		int monthday[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		for (int i = 1; i < monthday[mon - 1]; i++) {
			int temp = 2024 - mon < 3;
			int index = (temp + temp / 4 - temp / 100 + temp / 400 + t[mon - 1] + i) % 7;
			if (index > 0 && index < 6)
				total += 8;
		}
		int limit = (total * per) / 100;

		// If percentage is less than a given bench mark print that report
		if (sum.getHours() < limit) {
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << "Employee ID: " << empid << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << " Employee ID\tGuard ID\tDate\t\tDay\tEntry\tExit\tHours\tRemarks" << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
			if (buffer.empty())
				cout << "No records found" << endl;
			for (auto itj : buffer) {
				itj.printAttendanceRecord();
			}
			cout << "Month Total Working Hours = " << total << endl
				<< "Hours Worked by Employee = " << sum.getHours() << endl;
			system("pause");
		}
	}

	// Function for employee to generate report of attendance records monthly
	void viewEmployeeMonthlyAttendance() {
		system("cls");
		// Input of month to generate report
		cout << "Month number ( 1 - 12 )" << endl;
		int mon = validInput(1, 12);

		// Input of percentage to generate report
		cout << "Enter Percentage ( 0 - 100 )" << endl;
		int per = validInput(0, 100);

		// Get list of all employees
		vector<string> listUserID = userFileHandling::getAllEmployee();
		for (auto ite : listUserID) {
			viewEmployeeMonthlyAttendance(ite, mon, per);
			cout << endl << endl;
		}
	}
};