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

class pendingSuper : public status {
public:
	pendingSuper() {

	}
	string getStatus() const {
		return "Supervisor approval pending";
	}
};

class pendingDirec : public status {
public:
	string getStatus() const {
		return "Director approval pending";
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
protected:
	int id;							// Stores leave id
	string empid;					// Stores employee id
	Date from;						// Leave request from which date
	Date till;						// Leave request to which date
	string address;					// Leave address			
	string reason;					// Reason for leave
	Date applied;					// Leave applied date
	Date approved;					// leave approved date
	status* current;				// Current status of leave
	
public:
	leave(string empid) : current(nullptr) {
		id = 0;
		this->empid = empid;
	}

	void setStatus(status* obj) {
		if (current != nullptr)		// Check if current is nullptr
			delete current;
		current = obj;				// Set new status to object
	}
	void readRecordFile(ifstream& in) {
		string read;
		string temp;
		getline(in, read);
		stringstream ss(read);
		ss >> temp;
		this->id = stoi(temp);
		ss >> this->empid >> this->from >> this->till >> this->applied >> this->address;
		while (ss >> temp)
			this->reason += " " + temp;
	}
	void printRecord() {
		cout << " " << id << " " << empid << " " << getType() << " " << from << " " << till << " " << applied
			<< " " << approved << " " << current->getStatus() << " " << address << " " << reason << endl;
	}
	void userLeaveApplication() {
		bool flag = false;
		// Loop until correct date is enterd in specified format ( DD/MM )
		while (!flag) {
			cout << "Leave from i.e. ( DD/MM ): ";
			cin >> this->from;
			cout << "Leave till i.e. ( DD/MM ): ";
			cin >> this->till;
			if (this->from.valid() && this->till.valid() && (this->from < this->till || this->from == this->till))
				flag = true;
			// Check if attendance record of entered date is already present or not
			if (flag) {
				ifstream read("PendingSuper.txt");
				if (read.is_open()) {
					string temp;
					string fileempid;
					Date datefrom;
					Date datetill;
					string extra;
					while (getline(read, temp) and flag) {
						stringstream ss(temp);
						ss >> extra >> extra >> fileempid >> datefrom >> datetill;
						if (fileempid == this->empid) {
							ss >> datefrom;
							ss >> datetill;
							if (this->from.between(datefrom, datetill)) {
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

		if (flag) {
			// Input of leave address
			cout << "Enter address: ";
			cin >> this->address;

			// Input of leave reason
			cout << "Enter reason: ";
			cin.ignore();
			getline(cin, this->reason);

			// Get current date as date of application
			time_t now = std::time(0);
			struct tm now_tm;
			localtime_s(&now_tm, &now);
			this->applied = Date::Date(now_tm.tm_mday, now_tm.tm_mon + 1, now_tm.tm_year + 1900);
		}
	}
	void writeLeaveFile() {
		ofstream write("PendingSuper.txt", ios::app);
		if (write.is_open()) {
			write << endl << getType() << " " << id << " " << empid << " " << from << " " << till 
				<< " " << applied << " " << address << " " << reason;
			cout << "Leave applied successfully" << endl;
		}
		else
			cout << "Leave application failed!" << endl;
		write.close();
	}

	virtual string getType() const = 0;
};

class casual : public leave {
	string getType() const{
		return "casual";
	}
public:
	casual(string empid = "NA",status* obj = nullptr) :leave(empid) {
		this->setStatus(obj);
	}
};

class earned : public leave {
	string getType() const{
		return "earned";
	}
public:
	earned(string empid = "NA",status* obj = nullptr) :leave(empid) {
		this->setStatus(obj);
	}
};

class official : public leave {
	string getType () const {
		return "official";
	}
public:
	official(string empid = "NA",status* obj = nullptr) :leave(empid) {
		this->setStatus(obj);
	}
};

class unpaid : public leave {
	string getType () const{
		return "unpaid";
	}
public:
	unpaid(string empid = "NA",status* obj = nullptr) :leave(empid) {
		this->setStatus(obj);
	}
};

// Interface of leave service for employee
class leaveInterfaceEmployee {
public:
	virtual void applyLeave(string) = 0;
};

// Interface of leave service for supervisor
class leaveInterfaceSupervisor {
public:
	virtual void processLeaveSuper() = 0;
};

// Class which provide all the leave realted services like apply for leave, cancel leave, approve leave, reject leave
class leaveService : public leaveInterfaceEmployee, public leaveInterfaceSupervisor{
	leave* leaveObj;					// pointer of parent class to create any child class object when needed
public:
	// Function to apply for leave i.e. create specified leave object
	void applyLeave(string empid) {
		system("CLS");
		cout << "Apply Leave Menu" << endl
			<< "1- Casual Leave" << endl
			<< "2- Earned Leave" << endl
			<< "3- Unpaid Leave" << endl
			<< "4- Official Leave" << endl
			<< "5- Exit" << endl;
		int choice = validInput(1, 5);
		if (choice == 1)
			leaveObj = new casual(empid);
		else if (choice == 2)
			leaveObj = new earned(empid);
		else if (choice == 3)
			leaveObj = new unpaid(empid);
		else if (choice == 4)
			leaveObj = new official(empid);
		else
			return;

		// Get leave deatils from the user
		leaveObj->userLeaveApplication();

		//Record leave details in the file
		leaveObj->writeLeaveFile();

		system("pause");
	}

	// Function for supervisor to approve leave
	void processLeaveSuper() {

		
	}

};