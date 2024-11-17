#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include "helper.h"

using namespace std;

#pragma once
class status {
public:
	virtual string getStatus() const = 0;
	virtual ~status() = default;
};

class approved : public status {
public:
	string getStatus() const {
		return "approved";
	}
};

class pending : public status {
public:
	string getStatus() const {
		return "pending";
	}
};

class rejected : public status {
public:
	string getStatus() const {
		return "rejected";
	}
};

class cancelled : public status {
public:
	string getStatus() const {
		return "cancelled";
	}
};

class leave
{
	int id;
	string empid;			// Stores employee id
	string name;			// Stores employee name
	Date from;				// Leave request from which date
	Date till;				// Leave request to which date
	string address;			// Leave address			
	string reason;			// Reason for leave
	Date applied;			// Leave applied date
	Date approved;			// leave approved date
	unique_ptr<status> current;		// Current status of leave
	
	virtual string getType() const = 0;
	void setStatus(unique_ptr<status> obj) {
		current = move(obj);		// Move new obj to current
	}
	friend class leaveService;
public:
	leave(string empid,string name) : current(nullptr) {
		id = 0;
		this->empid = empid;
		this->name = name;
	}
	
	friend ostream& operator << (ostream & out,const leave & obj) {
		out << "Leave ID: " << obj.id << endl 
			<< "Employee ID: " << obj.empid << endl 
			<< "Name: " << obj.name << endl 
			<< "Period: " << obj.from << " - " << obj.till << endl 
			<< "Application Date: " << obj.applied << endl 
			<< "Type: " << obj.getType() << endl
			<< "Adress: " << obj.address << endl 
			<< "Reason: " << obj.reason << endl;
		return out;
	}
	friend ofstream& operator << (ofstream& out,const leave& obj) {
		out << obj.id << " " 
			<< obj.empid << " " 
			<< obj.from << " " 
			<< obj.till << " " 
			<< obj.getType() << " " 
			<< obj.applied << " " 
			<< obj.address << endl 
			<< obj.reason << endl;
		return out;
	}
	friend istream& operator >> (istream& in, leave& obj) {
		bool flag = false;
		// Loop until correct date is enterd in specified format ( DD/MM )
		while (!flag) {
			cout << "Leave from i.e. ( DD/MM ): ";
			in >> obj.from;
			cout << "Leave till i.e. ( DD/MM ): ";
			in >> obj.till;
			if (obj.from.valid() and obj.till.valid() and (obj.from < obj.till or obj.from == obj.till))
				flag = true;
			// Check if attendance record of entered date is already present or not
			if (flag) {
				ifstream read("PendingSuper.txt");
				if (read.is_open()) {
					string temp;
					string fileempid;
					Date datefrom;
					Date datetill;
					string reason;
					while (getline(read, temp) and flag) {
						getline(read, reason);
						stringstream ss(temp);
						ss >> fileempid;
						ss >> fileempid;
						if (fileempid == obj.empid) {
							cout << "Employee Found" << endl;
							ss >> datefrom;
							ss >> datetill;
							if (obj.from.between(datefrom,datetill)) {
								flag = false;
								cout << "Leave already applied of this period." << endl;
							}
						}
					}
				}
				else
					cout << "File not found!" << endl;
				read.close();
			}
			else {
				cout << "Invalid dates entered!" << endl;
			}
		}

		if(flag) {
			// New leave id
			obj.id = ++curr;

			// Input of leave address
			cout << "Enter address: ";
			in >> obj.address;

			// Input of leave reason
			cout << "Enter reason: ";
			cin.ignore();
			getline(cin, obj.reason);

			// Get current date as date of application
			time_t now = std::time(0);
			struct tm now_tm;
			localtime_s(&now_tm, &now);
			obj.applied = Date::Date(now_tm.tm_mday, now_tm.tm_mon + 1, now_tm.tm_year + 1900);
		}
		return in;
	}
	friend ifstream& operator >> (ifstream& in, leave& obj) {
		string first, last;
		in >> obj.empid >> obj.from >> obj.till >> obj.applied >> obj.address;
		getline(in, obj.reason);
		obj.name = first + last;
		return in;
	}
};

class casual : public leave {
	string getType() const{
		return "casual";
	}
public:
	casual(string empid,string name) :leave(empid,name) {
		
	}
	
};

class earned : public leave {
	string getType() const{
		return "earned";
	}
public:
	earned(string empid, string name) :leave(empid, name) {

	}
};

class official : public leave {
	string getType () const {
		return "official";
	}
public:
	official(string empid, string name) :leave(empid, name) {
	}
};

class unpaid : public leave {
	string getType () const{
		return "unpaid";
	}
public:
	unpaid(string empid, string name) :leave(empid, name) {
	}
};

// Interface of leave service for employee
class leaveInterfaceEmployee {
public:
	virtual void applyLeave(string,string) = 0;
};

// Class which provide all the leave realted services like apply for leave, cancel leave, approve leave, reject leave
class leaveService : public leaveInterfaceEmployee{
	leave* leaveObj;					// pointer of parent class to create any child class object when needed
public:

	// Function to apply for leave i.e. create specified leave object
	void applyLeave(string empid, string name) {
		system("CLS");
		cout << "Apply Leave Menu" << endl
			<< "1- Casual Leave" << endl
			<< "2- Earned Leave" << endl
			<< "3- Unpaid Leave" << endl
			<< "4- Official Leave" << endl
			<< "5- Exit" << endl;
		int choice = validInput(1, 5);
		if (choice == 1)
			leaveObj = new casual(empid, name);
		else if (choice == 2)
			leaveObj = new earned(empid, name);
		else if (choice == 3)
			leaveObj = new unpaid(empid, name);
		else if (choice == 4)
			leaveObj = new official(empid, name);
		else
			return;

		// Get leave deatils from the user
		cin >> *leaveObj;	

		// Record leave details in the file
		ofstream write("PendingSuper.txt",ios::app);
		if (write.is_open()) {
			write << *leaveObj;
			cout << "Leave applied successfully" << endl;
		}
		else
			cout << "Leave application failed!" << endl;
		write.close();
		system("pause");
	}


};