#include<iostream>
#include<fstream>
#include<sstream>
#include "helper.h"

#pragma once

// Attendance class used to store attendance record of an employee
class attendance {
	string empid;			// Stores employee id
	string guardid;			// Stroes guard id 
	Date date;				// Attendance date 
	string weekday;			// Attenance weekday
	Time entry;				// Time of entry
	Time exit;				// Time of exit
public:
	// Deafault constructor
	attendance(string empid = "NA", string guardid = "NA", string weekday = "NA"){
		this->empid = empid;
		this->guardid = guardid;
		this->weekday = weekday;
	}
	
	// Overloaded construstor
	attendance(string empid, string guardid, Date date, string weekday, Time entry, Time exit) {
		this->empid = empid;
		this->guardid = guardid;
		this->date = date;
		this->weekday = weekday;
		this->entry = entry;
		this->exit = exit;
	}
	
	// Copy constructor
	attendance(const attendance& obj) {
		empid = obj.empid;
		guardid = obj.guardid;
		date = obj.date;
		weekday = obj.weekday;
		entry = obj.entry;
		exit = obj.exit;
	}

	// Function to get id of employee whose attendance record is it
	string getEmpID() const {
		return empid;
	}

	// Function to get working hours on given attendance record
	Time getWorkingHours() const {
		return exit - entry;
	}

	// Overloading less than operator to sort attendance record based on date field
	bool operator < (attendance& obj) const {
		return date < obj.date;
	}

	// Overloading cout operator to print attendance record on console
	friend ostream& operator <<(ostream& out, const attendance& obj) {
		out << " " << obj.empid << " " << obj.guardid << " " << obj.date << " " << obj.weekday << " " << obj.entry << " " << obj.exit << " " << obj.getWorkingHours() << endl;
		return out;
	}

	// Overloading ofstream operator to write attendance record in file
	friend ofstream& operator <<(ofstream& out, const attendance& obj) {
		out << " " << obj.empid << " " << obj.guardid << " " << obj.date << " " << obj.weekday << " " << obj.entry << " " << obj.exit << endl;
		return out;
	}
	
	// Overloading cin operator to input required attendance details and store in attendance object
	friend istream& operator >>(istream& in, attendance& obj) {
		// Input of correct employee id
		bool flag = false;
		// Loop untill correct employee id entered by guard
		while (!flag) {
			cout << "Enter employee ID: ";
			in >> obj.empid;
			string temp;
			ifstream read("Employee.txt");
			if (read.is_open()) {
				string fileuserid;
				while (getline(read, temp) and !flag) {
					stringstream ss(temp);
					ss >> fileuserid;
					if (fileuserid == obj.empid)
						flag = true;
				}
			}
			else
				cout << "File not found!" << endl;
			if (!flag)
				cout << "Employee not fuond!" << endl;
		}

		// Input of correct date
		flag = false;
		// Loop until correct date is enterd in specified format ( DD/MM )
		while (!flag) {
			cout << "Enter date i.e. ( DD/MM ): ";
			in >> obj.date;
			if (obj.date.valid())
				flag = true;
			// Check if attendance record of entered date is already present or not
			if (flag) {					
				string temp;
				ifstream read("Attendence.txt");
				if (read.is_open()) {
					string fileuserid;
					string filedate;
					while (getline(read, temp) and flag) {
						stringstream ss(temp);
						ss >> fileuserid;
						if (fileuserid == obj.empid) {
							ss >> fileuserid;
							ss >> filedate;
							cout << filedate << endl;
							if (obj.date.compare(filedate)) {
								flag = false;
								cout << "Employee record for given date already exist." << endl;
							}
						}
					}
				}
				else
					cout << "File not found!" << endl;

			}
		}

		// Input of correct date
		flag = false;
		// Loop until correct weekday is entered
		while (!flag) {
			cout << "Enter weekday: ";
			in >> obj.weekday;
			if (obj.weekday == "Monwday" or obj.weekday == "monday" or obj.weekday == "Tuesday" or obj.weekday == "tuesday" or obj.weekday == "Wednesday" or obj.weekday == "wednesday" or obj.weekday == "Thursday" or obj.weekday == "thursday" or obj.weekday == "Fridays" or obj.weekday == "friday")
				flag = true;
			if (!flag)
				cout << "Invalid day entered!" << endl;
		}

		// Input of entry and exit time in office of employee
		flag = false;
		// Loop till correct time is enterwd
		while (!flag) {
			cout << "Enter entry time i.e. HH:MM (24 hour format): ";
			in >> obj.entry;
			if (obj.entry.valid())
				flag = true;
			if (!flag)
				cout << "Invalid entry time entered!" << endl;

			cout << "Enter exit time i.e. HH:MM (24 hour format): ";
			in >> obj.exit;
			if (obj.exit.valid())
				flag = true;
			if (!flag)
				cout << "Invalid exit time entered!" << endl;
			// Check if entry time is earlier than exit
			if (Time::checktime(obj.entry, obj.exit))
				flag = true;
			else
				cout << "Ambigous entry and exit time!" << endl;
		}

		return in;
	}

	// Overloading ifstream operator to read attendance record from file
	friend ifstream& operator >>(ifstream& in, attendance& obj) {
		in >> obj.empid;
		in >> obj.guardid;
		in >> obj.date;
		in >> obj.weekday;
		in >> obj.entry;
		in >> obj.exit;
		return in;
	}
};

// Class that provide attendance services
class attendanceService{
	attendance empAttendance;
public:
	// Deafult constructor that also initialize guard id to attendance object
	attendanceService(string guardid) : empAttendance("NA", guardid, "NA") {};
	
	// Function to mark attendance of employee
	void markAttendance() {
		cin >> empAttendance;
		ofstream write("Attendance.txt", ios::app);
		write << empAttendance;
		write.close();
	}
};

