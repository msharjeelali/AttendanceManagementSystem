#include<iostream>
#include<fstream>
#include<sstream>
#include "helper.h"

#pragma once

class attendance {
protected:
	string empid;
	string guardid;
	Date date;
	string day;
	Time entry;
	Time exit;
	Time hours;
public:
	attendance(string empid = "NA", string guardid = "NA", string day = "NA"){
		this->empid = empid;
		this->guardid = guardid;
		this->day = day;
	}
	attendance(string empid, string guardid, Date date, string day, Time entry, Time exit) {
		this->empid = empid;
		this->guardid = guardid;
		this->date = date;
		this->day = day;
		this->entry = entry;
		this->exit = exit;
		this->hours = exit - entry;
	}
	attendance(const attendance& obj) {
		empid = obj.empid;
		guardid = obj.guardid;
		date = obj.date;
		day = obj.day;
		entry = obj.entry;
		exit = obj.exit;
		hours = obj.hours;
	}

	string getEmpID() {
		return empid;
	}
	void printRecord() {
		cout << " " << empid << " " << guardid << " " << date << " " << day << " " << entry << " " << exit << " " << hours << endl;
	}
};

class attendanceInterfaceGuard : public attendance {
public:
	void markAttendance(string guardid) {
		bool flag = false;
		while (!flag) {
			cout << "Enter employee ID: ";
			cin >> empid;
			string temp;
			ifstream read("Employee.txt");
			if (read.is_open()) {
				string fileuserid;
				while (getline(read, temp) and !flag) {
					stringstream ss(temp);
					ss >> fileuserid;
					if (fileuserid == empid)
						flag = true;
				}
			}
			else
				cout << "File not found!" << endl;
			if (!flag)
				cout << "Employee not fuond!" << endl;
		}

		flag = false;
		while (!flag) {
			cout << "Enter date i.e. DD/MM ): ";
			cin >> date;
			if (date.valid())
				flag = true;
			if (flag) {
				string temp;
				ifstream read("Attendence.txt");
				if (read.is_open()) {
					string fileuserid;
					string filedate;
					while (getline(read, temp) and flag) {
						stringstream ss(temp);
						ss >> fileuserid;
						if (fileuserid == empid) {
							ss >> fileuserid;
							ss >> filedate;
							cout << filedate << endl;
							if (date.compare(filedate)) {
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

		flag = false;
		while (!flag) {
			cout << "Enter day: ";
			cin >> day;
			if (day == "Monday" or day == "monday" or day == "Tuesday" or day == "tuesday" or day == "Wednesday" or day == "wednesday" or day == "Thursday" or day == "thursday" or day == "Fridays" or day == "friday")
				flag = true;
			if (!flag)
				cout << "Invalid day entered!" << endl;
		}

		flag = false;
		while (!flag) {
			cout << "Enter entry time i.e. HH:MM (24 hour format): ";
			cin >> entry;
			if (entry.valid())
				flag = true;
			if (!flag)
				cout << "Invalid entry time entered!" << endl;

			cout << "Enter exit time i.e. HH:MM (24 hour format): ";
			cin >> exit;
			if (exit.valid())
				flag = true;
			if (!flag)
				cout << "Invalid exit time entered!" << endl;

			if (Time::checktime(entry,exit))
				flag = true;
			else
				cout << "Ambigous entry and exit time!" << endl;
		}

		this->guardid = guardid;
		ofstream write("Attendence.txt",ios::app);
		if (write.is_open()) {
			write << empid << " " << guardid << " " << date << " " << day << " " << entry << " " << exit << endl;
			cout << "Attendnce marked successfully" << endl;
			system("Pause");
		}
		else
			cout << "Unable to open attendence records" << endl;
	}
};

