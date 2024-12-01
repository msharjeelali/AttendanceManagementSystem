#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <string>
#include "helper.h"
#include "attendance.h"

using namespace std;

// Interface of leave record according to DIP
class leaveRecordI {
public:
	// Function to get balance of specific leave type
	virtual int getBalance(int) = 0;
	// Function to add balance of specific leave type
	virtual void addBalance(int, int) = 0;
	// Function to subtract balance of specific leave type
	virtual void decBalance(int,int) = 0;
	// Function to print balance of all leaves
	virtual void print() = 0;
};

// Class to store leave balance of employee
class leaveRecord :public leaveRecordI{
	// Count of casual leaves reamining
	int casual;
	// Count of earned leaves remaining
	int earned;
	// Count of official leaves remaining
	int official;
	// Year of current leave data;
	int year;
public:
	// Default constructor
	leaveRecord(int casual = 15, int earned = 24, int official = 0,int year = 2024) {
		// Set balance of each leave type
		this->casual = casual;
		this->earned = earned;
		this->official = official;
		this->year = year;

		// Get current year
		int curr;
		time_t now = std::time(0);
		struct tm now_tm;
		localtime_s(&now_tm, &now);
		curr = now_tm.tm_year + 1900;

		// If year changes than reset all leave values
		if (curr > year) {
			casual = 15;
			earned = 24;
			official = 0;
			year = curr;
		}
	}

	// Function to get casual leaves count
	int getBalance(int type) {
		// If type is 1 means causal
		if (type == 1)
			return casual;
		// If type is 2 means earned
		else if (type == 2)
			return earned;
		// If type is 3 means official
		else if (type == 3)
			return official;
	}

	// Function to increment leaves count
	// It is called when leave is rejected or cancelled
	void addBalance(int count,int type) { 
		// If type is 1 add count to casual leave
		if (type == 1)
			casual += count;
		// If type is 2 add count to earned leave
		else if (type == 2)
			earned += count;
		// If type is 3 subtract count to official leave
		// Since official store official leaves availed while other store remaining leaves
		else if (type == 3)
			official -= count;
	}

	// Function to decrement leaves count
	// It is called when leave is approved or applied
	void decBalance(int count, int type) { 
		// If type is 1 subtract count from casual
		if (type == 1)
			casual -= count;
		// If type is 2 subtract count from earned
		else if (type == 2)
			earned -= count;
		// If type is 3 add count to official
		// Since official store official leaves availed while other store remaining leaves
		else if (type == 3)
			official += count;
	}

	// Function to display leave balances
	void print() {
		system("CLS");
		// Print leave balance
		cout << "Leave Balance: Casual = " << casual << ", Earned = " << earned << " Official Leaves Availed = " << official << endl;
	}
};

// State pattern implementation
class status {
public:
	// Fucntion to get status to leave application
	virtual string getStatus() const = 0;
	// Function to print message on console depending on leave status
	virtual string getMessage() const = 0;
};

// Approved state when a leave application is approved
class approved : public status {
public:
	// Return approved as status
	string getStatus() const {
		return "Approved";
	}
	// Return approved as message
	string getMessage() const {
		return "approved";
	}
};

// Under review by supervisor state of leave application
class pendingSuper : public status {
public:
	// Return status
	string getStatus() const {
		return "Under Review By Supervisor";
	}
	// Return message
	string getMessage() const {
		return "under review by supervisor";
	}
};

// Under review by director state of leave application
class pendingDirec : public status {
public:
	// Return state
	string getStatus() const {
		return "Under Review By Director";
	}
	// Return message
	string getMessage() const {
		return "approved by supervisor and under review by director";
	}
};

// Rejected state of leave application
class rejected : public status {
public:
	// Return rejected as status
	string getStatus() const {
		return "Rejected";
	}
	// Return rejected as message
	string getMessage() const {
		return "Rejected";
	}
};

// Cancelled state of leave applicaion
class cancelled : public status {
public:
	// Return cancelled as status
	string getStatus() const {
		return "Cancelled";
	}
	// Return cancelled as message
	string getMessage() const {
		return "Cancelled";
	}
};

// Observer pattern implementation
class Observer {
public:
	virtual void update(string, int, string) = 0;
};

// Notify class to write notification in file
class NotifyEmp : public Observer {
public:
	// Function to update employee about his leave applications
	void update(string empid, int leaveid, string message) {
		// Open notifications file and write notifications in it
		ofstream write("Notify.txt", ios::app);
		if (write.is_open()) {
			// Write employee id with leave id and notification
			write << empid  << " " << leaveid << " " << message << endl;
		}
		else
			cout << "Notify file not found" << endl;
	}
};

// Observer pattern implementation
class Subject {
	Observer* obs;
public:
	// Initialize observer
	Subject() {
		obs = new NotifyEmp();
	}

	// Function to notify employee about leave update
	void notifyObserver(string empid, int leaveid,string message) {
		obs->update(empid, leaveid,message);
	}
};

// Leave class
class leave: public Subject{
protected:
	// Stores leave id
	int id;							
	// Stores employee id
	string empid;					
	// Leave request from which date
	Date from;						
	// Leave request to which date
	Date till;						
	// Leave address			
	string address;					
	// Reason for leave
	string reason;					
	// Leave applied date
	Date applied;					
	// leave approved date
	Date approved;					
	// Current status of leave
	status* current;				

	friend class leaveConsoleHandling;
	friend class leaveFileHandling;
public:
	// Overloaded constructor to set just employe id
	leave(string empid) : current(nullptr) {
		id = 0;
		this->empid = empid;
	}

	// Getter and setter of leave class
	
	// Function that return leave id
	int getLeaveID() { return id; }
	// Function to set approval date of leave application
	void setApprovalDate(Date obj) { this->approved = obj; }
	// Function to set application date of leave application
	void setApplicationDate(Date obj) { this->applied = obj; }
	// Function to set leave id of leave application
	void setleaveID(int id) { this->id = id; }
	// Function to get employee id of leave application
	string getEmpID() { return empid; }
	// Function to get from date of leave application
	Date getFromDate() { return from; }
	// Function to get till date of leave application
	Date getTillDate() { return till; }
	
	// Function to set current status of leave application
	void setLeaveStatus(status* obj) {
		// Check if current is nullptr
		if (current != nullptr) {		
			// Delete current status object
			delete current;
			// If updated status is approved
			// Get current date as date of application
			if (obj->getStatus() == "approved") {
				time_t now = std::time(0);
				struct tm now_tm;
				localtime_s(&now_tm, &now);
				this->approved = Date::Date(now_tm.tm_mday, now_tm.tm_mon + 1, now_tm.tm_year + 1900);
			}
		}
		// Set new status to object
		current = obj;				
	}
	
	// Function to notify employee about his leave application current status
	// Called when ever status is changed
	void notify() {
		notifyObserver(empid, id, current->getMessage());
	}

	virtual string getType() const = 0;
	~leave() {
		delete current;
		current = nullptr;
	}
};

// Casual type of leave
class casual : public leave {
	// Function to get type of leave
	string getType() const {
		return "casual";
	}
public:
	casual(string empid = "NA") :leave(empid) {	}
};

// Earned type of leave
class earned : public leave {
	// Function to get type of leave
	string getType() const {
		return "earned";
	}
public:
	earned(string empid = "NA") :leave(empid) {	}
};

// Official type of leave
class official : public leave {
	// Function to get type of leave
	string getType() const {
		return "official";
	}
public:
	official(string empid = "NA") :leave(empid) { }
};

// Unpaid type of leave
class unpaid : public leave {
	// Function to get type of leave
	string getType() const {
		return "unpaid";
	}
public:
	unpaid(string empid = "NA") :leave(empid) { }
};

// Class to deal all leave related console operations
class leaveConsoleHandling {
public:

	// Function to print leave record on console
	void printLeaveConsole(const leave* obj) {
		// Print leave object in specified format
		cout << " " << obj->id << "\t\t" << obj->empid << "\t" << (obj->getType() == "official" ? "ofical" : obj->getType()) << "\t" << obj->from << "\t"
			<< obj->till << "\t" << obj->applied << "\t" << obj->approved << "\t" << obj->current->getStatus() << "\t"
			<< obj->address << "\t\t" << obj->reason << endl;
	}

	// Function to print leave records on console
	void printLeaveConsole(const vector<leave*> record) {
		// If records are empty
		if (record.empty())
			cout << "No records found" << endl;
		// If there is more than equal to one record
		else {
			// Print all leave objects in specified format
			cout << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			cout << " LeaveID\tEmployee ID\tType\tFrom\t\tTill\t\tApplied\t\tApproved\tStatus\t\tAddress\t\tReason" << endl;
			cout << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			// Iteratoe thorugh all leave objects in record
			for (auto ite : record)
				cout << " " << ite->id << "\t\t" << ite->empid << "\t" << (ite->getType() == "official"?"ofical":ite->getType()) << "\t" << ite->from << "\t"
				<< ite->till << "\t" << ite->applied << "\t" << ite->approved << "\t" << ite->current->getStatus() << "\t"
				<< ite->address << "\t\t" << ite->reason << endl;
		}
	}

	// Function to read leave application from the console
	void readLeaveApplication(leave* object) {

		bool flag = false;
		// Loop until correct date is enterd in specified format ( DD/MM )
		while (!flag) {
			// Input leave from date
			cout << "Leave from i.e. ( DD/MM ): ";
			cin >> object->from;
			// Input leave till date
			cout << "Leave till i.e. ( DD/MM ): ";
			cin >> object->till;
			// Check if both dates are vald
			// Check if from date is before or equal to till date
			if (object->from.valid() && object->till.valid() && (object->from < object->till || object->from == object->till))
				flag = true;
			// If above condition are not fullfilled
			if (!flag) 
				cout << "Invalid dates entered!" << endl;
			// If valid dates are enterd
			else if (object->getType() == "casual") {
				// Restrict user to apply for no more than 4 leaves
				if (object->till.getDiff(object->from) > 4) {
					cout << "You cannot apply for casual leave of more than 4 days" << endl;
					flag = false;
				}
			}
		}

		// If all date conditions are valid
		if (flag) {
			// Input of leave address
			cout << "Enter address: ";
			cin >> object->address;

			// Input of leave reason
			cout << "Enter reason: ";
			cin.ignore();
			getline(cin, object->reason);

			// Get current date as date of application
			time_t now = std::time(0);
			struct tm now_tm;
			localtime_s(&now_tm, &now);
			object->applied = Date::Date(now_tm.tm_mday, now_tm.tm_mon + 1, now_tm.tm_year + 1900);
		}
	}
};

// Clasa to deal all leave related file opeartions
class leaveFileHandling {
	// Variables that store name of different text files
	string underSuperFile;
	string underDirecFile;
	string approvedFile;
	string cancelledFile;
	string rejectedFile;

	friend class leaveConsoleHandling;
public:
	// Default constructor
	leaveFileHandling() {
		// Initialize all file names
		underSuperFile = "underSuper.txt";
		underDirecFile = "underDirec.txt";
		approvedFile = "approved.txt";
		cancelledFile = "cancelled.txt";
		rejectedFile = "rejected.txt";
	}

	// Function to write leave in files
	void writeLeaveRecord(leave* object) {
		string filename;
		// Check what is the status of leave application that we want to write
		if (object->current->getStatus() == "Approved")
			filename = approvedFile;
		else if (object->current->getStatus() == "Under Review By Supervisor")
			filename = underSuperFile;
		else if (object->current->getStatus() == "Under Review By Director")
			filename = underDirecFile;
		else if (object->current->getStatus() == "Rejected")
			filename = rejectedFile;
		else if (object->current->getStatus() == "Cancelled")
			filename = cancelledFile;

		// Open specifiied file in append mode for writing
		ofstream write(filename, ios::app);
		if (write.is_open()) {
			write << endl << object->getType() << " " << object->id << " " << object->empid << " "
				<< object->from << " " << object->till << " " << object->applied;
			// If leave is approved that also write approval date in file
			if (object->current->getStatus() == "Approved")
				write << " " << object->approved;
			write << " " << object->address << " " << object->reason;
		}
		else
			cout << "Leave file not found!" << endl;
	}

	// Function to write leave in director files
	void overWriteLeaveRecordDirec(vector<leave*> object) {

		// Open specified file in writing mode
		// Truncate file before writing any records
		ofstream write(underDirecFile, ios::out | ios::trunc);
		if (write.is_open()) {
			for (auto ite : object) {
				write << ite->getType() << " " << ite->id << " " << ite->empid << " "
					<< ite->from << " " << ite->till << " " << ite->applied
					<< " " << ite->address << " " << ite->reason << endl;
			}
		}
		else
			cout << "Leave file not found!" << endl;
	}

	// Function to write leave in supervisor files
	void overWriteLeaveRecordSuper(vector<leave*> object) {
		// Open specified file in writing mode
		// Truncate file before writing any records
		ofstream write(underSuperFile, ios::out | ios::trunc);
		if (write.is_open()) {
			for (auto ite : object) {
				write << ite->getType() << " " << ite->id << " " << ite->empid << " "
					<< ite->from << " " << ite->till << " " << ite->applied
					<< " " << ite->address << " " << ite->reason << endl;
			}
		}
		else
			cout << "Leave file not found!" << endl;
	}

	// Function to read approved leave records from file
	void readApprovedLeaveRecord(vector<leave*>& record, string empid = "default") {
		// Temporary object to store leave appliaction details
		leave* object = nullptr;
		string type;

		// Open the file in read mode
		ifstream read(approvedFile);
		if (read.is_open()) {

			// While we are reading data from the files
			while (!read.eof()) {

				// Read which type of leave is it and create leave object according to type
				read >> type;
				if (type == "casual")
					object = new casual("NA");
				else if (type == "earned")
					object = new earned("NA");
				else if (type == "official")
					object = new official("NA");
				else if (type == "unpaid")
					object = new unpaid("NA");

				// Read remaining details of leave from the file
				string readline;
				string temp;
				getline(read, readline);
				stringstream ss(readline);
				ss >> temp;
				object->id = stoi(temp);
				
				// Store information in variables of leave class
				ss >> object->empid >> object->from >> object->till >> object->applied >> object->approved >> object->address;
				while (ss >> temp)
					object->reason += " " + temp;
				object->setLeaveStatus(new approved());
	
				// If not passed than store all the leave records
				// Check if empid is passed than only store leave record of that employee
				if(object->empid == empid||empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Approved leave records file not found!" << endl;
	}

	// Function to read under review by supervisor leave records from file
	void readSuperLeaveRecord(vector<leave*>& record, string empid = "default") {
		// Temprary object to store leave appliaction details
		leave* object = nullptr;
		string type;

		ifstream read(underSuperFile);
		// Open the file in read mode
		if (read.is_open()) {
			// While we are reading data from the files
			while (read >> type) {
				// Read which type of leave is it and create leave object according to type
				
				if (type == "casual")
					object = new casual("NA");
				else if (type == "earned")
					object = new earned("NA");
				else if (type == "official")
					object = new official("NA");
				else if (type == "unpaid")
					object = new unpaid("NA");

				// Read remaining details of leave from the file
				string readline;
				string temp;
				getline(read, readline);
				stringstream ss(readline);
				ss >> temp;
				object->id = stoi(temp);

				// Store information in variables of leave class
				ss >> object->empid >> object->from >> object->till >> object->applied >> object->address;
				while (ss >> temp)
					object->reason += " " + temp;
				object->setLeaveStatus(new pendingSuper());
				// Check if empid is passed than only store leave record of that employee
				// If not passed than store all the leave records
				if (object->empid == empid||empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Approved leave records file not found!" << endl;
	}

	// Function to read under review by director leave records from file
	void readDirecLeaveRecord(vector<leave*>& record, string empid = "default") {
		// Temprary object to store leave appliaction details
		leave* object = nullptr;
		string type;

		// Open the file in read mode
		ifstream read(underDirecFile);
		if (read.is_open()) {
			
			// While we are reading data from the files
			while (read >> type) {
				
				// Read which type of leave is it and create leave object according to type
				if (type == "casual")
					object = new casual("NA");
				else if (type == "earned")
					object = new earned("NA");
				else if (type == "official")
					object = new official("NA");
				else if (type == "unpaid")
					object = new unpaid("NA");
				
				// Read remaining details of leave from the file
				string readline;
				string temp;
				getline(read, readline);
				stringstream ss(readline);
				ss >> temp;
				
				// Store information in variables of leave class
				object->id = stoi(temp);
				ss >> object->empid >> object->from >> object->till >> object->applied >> object->address;
				while (ss >> temp)
					object->reason += " " + temp;
				object->setLeaveStatus(new pendingDirec());
				// Check if empid is passed than only store leave record of that employee
				// If not passed than store all the leave records
				if(object->empid == empid||empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Approved leave records file not found!" << endl;
	}

	// Function to read under cancelled leave records from file
	void readCancelledLeaveRecord(vector<leave*>& record, string empid = "default") {
		// Temprary object to store leave appliaction details
		leave* object = nullptr;
		string type;

		// Open the file in read mode
		ifstream read(cancelledFile);
		if (read.is_open()) {
			// While we are reading data from the files
			while (read >> type) {
				// Read which type of leave is it and create leave object according to type
				if (type == "casual")
					object = new casual("NA");
				else if (type == "earned")
					object = new earned("NA");
				else if (type == "official")
					object = new official("NA");
				else if (type == "unpaid")
					object = new unpaid("NA");
				// Read remaining details of leave from the file
				string readline;
				string temp;
				getline(read, readline);
				stringstream ss(readline);
				// Store information in variables of leave class
				ss >> temp;
				object->id = stoi(temp);
				ss >> object->empid >> object->from >> object->till >> object->applied >> object->address;
				while (ss >> temp)
					object->reason += " " + temp;
				object->setLeaveStatus(new cancelled());
				// Check if empid is passed than only store leave record of that employee
				// If not passed than store all the leave records
				if(object->empid == empid||empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Approved leave records file not found!" << endl;
	}

	// Function to read rejected leave records from file
	void readRejectedLeaveRecord(vector<leave*>& record, string empid = "default") {
		// Temprary object to store leave appliaction details
		leave* object = nullptr;
		string type;

		// Open the file in read mode
		ifstream read(rejectedFile);
		if (read.is_open()) {
			// While we are reading data from the files
			while (!read.eof()) {
				// Read which type of leave is it and create leave object according to type
				read >> type;
				if (type == "casual")
					object = new casual("NA");
				else if (type == "earned")
					object = new earned("NA");
				else if (type == "official")
					object = new official("NA");
				else if (type == "unpaid")
					object = new unpaid("NA");
				// Read remaining details of leave from the file
				string readline;
				string temp;
				getline(read, readline);
				// Store information in variables of leave class
				stringstream ss(readline);
				ss >> temp;
				object->id = stoi(temp);
				ss >> object->empid >> object->from >> object->till >> object->applied >> object->address;
				while (ss >> temp)
					object->reason += " " + temp;
				object->setLeaveStatus(new rejected());
				// Check if empid is passed than only store leave record of that employee
				// If not passed than store all the leave records
				if(object->empid == empid||empid == "default")
					record.push_back(object);
			}
		}
		else
			cout << "Approved leave records file not found!" << endl;
	}

	// Function to check if leave application of employee for a date exist or not
	bool checkLeaveRecord(leave* object) {
		vector<leave*> record;
		// Check if leave is in approved records
		readApprovedLeaveRecord(record, object->empid);
		for (auto ite : record) {
			if (object->from.between(ite->from,ite->till)) {
				cout << "You already have approved leave for this date" << endl;
				return true;
			}
		}
		record.clear();
		// Check if leave is in under review by supervisor records
		readSuperLeaveRecord(record, object->empid);
		for (auto ite : record) {	
			if (object->from.between(ite->from, ite->till)) {
				cout << "You already have under review by supervisor leave for this date" << endl;
				return true;
			}
		}
		record.clear();
		// Check if leave is in under review by director records
		readDirecLeaveRecord(record, object->empid);
		for (auto ite : record) {
			if (object->from.between(ite->from, ite->till)) {
				cout << "You already have under review by director leave for this date" << endl;
				return true;
			}
		}
		record.clear();
		// Check if leave is in cancelled records
		readRejectedLeaveRecord(record, object->empid);
		for (auto ite : record) {
			if (object->from.between(ite->from, ite->till)) {
				cout << "You already have rejected leave for this date" << endl;
				return true;
			}
		}
		return false;
	}

	// Function to update cancel of leave application
	void cancelLeaveRecord(leave* object) {
		vector<leave*> record;
		if (object->current->getStatus() == "Under Review By Supervisor") {
			readSuperLeaveRecord(record);
			for (auto ite = record.begin(); ite != record.end(); ite++) {
				if ((*ite)->getLeaveID() == object->getLeaveID()) {
					record.erase(ite);
					break;
				}
			}
			overWriteLeaveRecordSuper(record);
		}
		else if (object->current->getStatus() == "Under Review By Director") {
			readDirecLeaveRecord(record);
			for (auto ite = record.begin(); ite != record.end(); ite++) {
				if ((*ite)->getLeaveID() == object->getLeaveID()) {
					record.erase(ite);
					break;
				}
			}
			overWriteLeaveRecordDirec(record);
		}
	}
};

// Interface of leave service for employee
class leaveInterfaceEmployee {
public:
	virtual void applyLeave(user*) = 0;
	virtual void cancelLeave(user*) = 0;
};

// Interface of leave service for supervisor
class leaveInterfaceSupervisor {
public:
	virtual void processLeaveSupervisor() = 0;
};

// Interface of leave service for director
class leaveInterfaceDirector {
public:
	virtual void processLeaveDirector() = 0;
};

// Class which provide all the leave realted services like apply for leave, cancel leave, approve leave, reject leave
class leaveService : public leaveInterfaceEmployee, public leaveInterfaceSupervisor, public leaveInterfaceDirector {
	vector<leave*> record;					
	leaveConsoleHandling* consoleHandling;
	leaveFileHandling* fileHandling;
	AttendanceServiceLeaveI* attendanceInterface;
	
public:
	// Default constructor of class
	leaveService() {
		consoleHandling = new leaveConsoleHandling();
		fileHandling = new leaveFileHandling();
		attendanceInterface = new AttendanceService();
	}

	// Function to apply for leave i.e. create specified leave object
	void applyLeave(user* current) {
		system("CLS");

		leave* object = nullptr;
		cout << "Apply Leave Menu" << endl
			<< "1- Casual Leave" << endl
			<< "2- Earned Leave" << endl
			<< "3- Unpaid Leave" << endl
			<< "4- Official Leave" << endl
			<< "5- Exit" << endl;
		int choice = validInput(1, 5);
		int balance = 0;
		if (choice == 1) {
			object = new casual(current->getID());
			balance = current->getBalance(1);
		}
		else if (choice == 2) {
			object = new earned(current->getID());
			balance = current->getBalance(2);
		}
		else if (choice == 3) {
			object = new unpaid(current->getID());
			balance = INT_MAX;
		}
		else if (choice == 4) {
			object = new official(current->getID());
			balance = INT_MAX;
		}
		else
			return;

		// Get leave deatils from the user
		consoleHandling->readLeaveApplication(object);

		int days = object->getTillDate().getDiff(object->getFromDate());
		if (days <= balance) {


			bool flag = false;
			for (Date i = object->getFromDate(); i < object->getTillDate() || i == object->getTillDate(); i = i + 1) {
				if (attendanceInterface->checkAttendanceRecord(object->getEmpID(), i))
					flag = true;
			}

			if (flag) {
				cout << "Your attendance records for leave dates exist" << endl;
				cout << "Cannot apply leave for these dates" << endl;
			}
			else
			{
				//Record leave details in the file if there is no leave record against application date
				if (!fileHandling->checkLeaveRecord(object)) {
					int leaveid = 0;
					ifstream read("leaveid.txt");
					read >> leaveid;
					object->setleaveID(++leaveid);
					read.close();
					ofstream write("Leaveid.txt");
					write << leaveid;
					object->setLeaveStatus(new pendingSuper());
					object->notify();
					fileHandling->writeLeaveRecord(object);
					cout << "Leave applied successfully" << endl;
					if (object->getType() == "casual")
						current->decBalance(days,1);
					else if (object->getType() == "earned")
						current->decBalance(days,2);
					else if (object->getType() == "official")
						current->decBalance(days,3);

				}
				else
					cout << "Cannot apply leave for these dates" << endl;
			}
		}
		else
			cout << "You cannot apply for leave due to insufficient balance." << endl;
		system("pause");
	}

	void cancelLeave(user* current) {
		fileHandling->readSuperLeaveRecord(record, current->getID());
		fileHandling->readDirecLeaveRecord(record, current->getID());
		int choice = 0;
		do {
			consoleHandling->printLeaveConsole(record);
			if (!record.empty()) {
				int leaveid = 0;
				cout << "Enter leave id to cancel: ";
				cin >> leaveid;
				bool flag = false;
				for (auto ite = record.begin(); ite != record.end(); ite++) {
					if ((*ite)->getLeaveID() == leaveid) {
						flag = true;
						fileHandling->cancelLeaveRecord(*ite);
						(*ite)->setLeaveStatus(new cancelled());
						fileHandling->writeLeaveRecord(*ite);
						(*ite)->notify();
						int days = (*ite)->getTillDate().getDiff((*ite)->getFromDate());
						if ((*ite)->getType() == "casual")
							current->incBalance(days, 1);
						else if ((*ite)->getType() == "earned")
							current->incBalance(days, 2);
						else if ((*ite)->getType() == "official")
							current->incBalance(days, 3);
						record.erase(ite);

						break;
					}
				}
				if (!flag)
					cout << "Invalid leave ID" << endl;
			}
			cout << "1- Cancel another leave" << endl
				<< "2- Return" << endl;
			choice = validInput(1, 2);
		} while (choice != 2);
	}

	// Function for supervisor to approve or reject leave
	void processLeaveSupervisor() {
		record.clear();
		// Function to read leave records of pending supervisor approval
		fileHandling->readSuperLeaveRecord(record);	
		int choice = 0;

		// Loop untill supervisor want to process leave applications
		do {
			if (!record.empty()) {
				system("CLS");
				bool flag = false;
				// Print all the leave reocrds on console
				consoleHandling->printLeaveConsole(record);

				// Input leave id of leave which supervisor want to process
				int leaveid;
				cout << "Enter leave id to process: ";
				cin >> leaveid;

				if (cin.fail()) {
					cin.clear();
					cin.ignore();
					cout << "Invalid leave id!" << endl;
				}

				// Iterate through all the leave records
				for (auto ite = record.begin(); ite != record.end(); ite++) {
					// If reocrd id matches the leave id entered by superviosr
					if ((*ite)->getLeaveID() == leaveid) {
						flag = true;
						cout << "1- Approve Leave" << endl
							<< "2- Reject Leave" << endl
							<< "3- Return" << endl;
						choice = validInput(1, 3);

						// If supervisor approves the leave application
						if (choice == 1) {
							// Check if director approval is required or not and set status according to leave type
							if ((*ite)->getType() == "casual" || (*ite)->getType() == "official") {
								(*ite)->setLeaveStatus(new approved());
								fileHandling->writeLeaveRecord(*ite);
								attendanceInterface->markAttendanceLeave((*ite)->getEmpID(), (*ite)->getFromDate(), (*ite)->getTillDate());
							}
							else {
								(*ite)->setLeaveStatus(new pendingDirec());
								fileHandling->writeLeaveRecord(*ite);
							}
							(*ite)->notify();
							// Remove that leave from the pending supervisor approval since it it processed
							record.erase(ite);
						}
						// If supervisor rejects the leave application
						else if (choice == 2) {
							// Set status to rejected
							(*ite)->setLeaveStatus(new rejected());
							userFileHandling::updateBalance((*ite)->getEmpID(), (*ite)->getType(), (*ite)->getTillDate().getDiff((*ite)->getFromDate()));
							fileHandling->writeLeaveRecord(*ite);
							(*ite)->notify();
							// Remove that leave from the pending supervisor approval since it it processed
							record.erase(ite);
						}
						break;
					}
				}
				// Ask if supervisor want to process more leave records
				if (!flag)
					cout << "No leave with eneterd id" << endl;
				cout << "1- Process more leave" << endl
					<< "2- Return" << endl;
				choice = validInput(1, 2);
			}
			else {
				cout << "No more leave applications to process" << endl;
				choice = 2;
			}
		} while (choice == 1);

		// Write all the remaining records in under supervisor approval file
		fileHandling->overWriteLeaveRecordSuper(record);

		system("pause");
	}

	// Function for director to approve or reject leave
	void processLeaveDirector() {
		record.clear();
		// Function to read leave records of pending director approval
		fileHandling->readDirecLeaveRecord(record);
		int choice = 0;

		// Loop untill director want to process leave applications
		do {

			if (!record.empty()) {
				system("CLS");
				bool flag = false;
				// Print all the leave reocrds on console
				consoleHandling->printLeaveConsole(record);

				// Input leave id of leave which director want to process
				int leaveid;
				cout << "Enter leave id to process: ";
				cin >> leaveid;

				if (cin.fail()) {
					cin.clear();
					cin.ignore();
					cout << "Invalid leave id!" << endl;
				}

				// Iterate through all the leave records
				for (auto ite = record.begin(); ite != record.end(); ite++) {
					// If reocrd id matches the leave id entered by director
					if ((*ite)->getLeaveID() == leaveid) {
						flag = true;
						cout << "1- Approve Leave" << endl
							<< "2- Reject Leave" << endl
							<< "3- Return" << endl;
						choice = validInput(1, 3);

						// If director approves the leave application
						if (choice == 1) {
							// Set leave status to approved
							(*ite)->setLeaveStatus(new approved());
							attendanceInterface->markAttendanceLeave((*ite)->getEmpID(), (*ite)->getFromDate(), (*ite)->getTillDate());
							fileHandling->writeLeaveRecord(*ite);
							(*ite)->notify();
							// Remove that leave from the pending supervisor approval since it it processed
							record.erase(ite);
						}
						// If supervisor rejects the leave application
						else if (choice == 2) {
							// Set status to rejected
							(*ite)->setLeaveStatus(new rejected());
							userFileHandling::updateBalance((*ite)->getEmpID(), (*ite)->getType(), (*ite)->getTillDate().getDiff((*ite)->getFromDate()));
							fileHandling->writeLeaveRecord(*ite);
							(*ite)->notify();
							// Remove that leave from the pending supervisor approval since it it processed
							record.erase(ite);
						}
						break;
					}
				}
				// Ask if director want to process more leave records
				if (!flag)
					cout << "No leave with eneterd id" << endl;
				cout << "1- Process more leave" << endl
					<< "2- Return" << endl;
				choice = validInput(1, 2);
			}
			else {
				cout << "No leave applications to process" << endl;
				choice = 2;
			}
		} while (choice == 1);

		// Write all the remaining records in under director approval file
		fileHandling->overWriteLeaveRecordDirec(record);

		system("pause");
	}

	// Destructor of class
	~leaveService() {
		for (auto ite : record) {
			delete ite;
			ite = nullptr;
		}
		delete consoleHandling;
		consoleHandling = nullptr;
		delete fileHandling;
		fileHandling = nullptr;
	}
};

// Leave report interface for director
class leaveReportsDirectorI {
public:
	virtual void viewOutstandingLeavesDirector() = 0;
	virtual void viewLeaveRecord() = 0;
};

// leave report interface for supervisor
class leaveReportsSupervisorI {
public:
	virtual void viewOutstandingLeavesSupervisor() = 0;
	virtual void viewLeaveRecord() = 0;
};

// Leave report interface for employee
class leaveReportsEmployeeI {
public:
	virtual void viewLeaveRecord(string) = 0;
};

// CLass to generate all leave related reports
class leaveReports: public leaveReportsDirectorI, public leaveReportsSupervisorI, public leaveReportsEmployeeI {
	vector<leave*> record;
	leaveFileHandling* fileHandling;
	leaveConsoleHandling* consoleHandling;
public:
	// Default constructor
	leaveReports() {
		fileHandling = new leaveFileHandling();
		consoleHandling = new leaveConsoleHandling();
	}

	// Function for director to view his outstanding leaves
	void viewOutstandingLeavesDirector() {
		// Clear previous records if any
		record.clear();
		cout << "Leaves Pending for Director Approval" << endl << endl;
		// Read all leaves that are under director review
		fileHandling->readDirecLeaveRecord(record);
		// Print all leaves that are under director review
		consoleHandling->printLeaveConsole(record);
		system("pause");
	}

	// Function for supervisor to view his outstanding leaves
	void viewOutstandingLeavesSupervisor() {
		// Clear previous records if any
		record.clear();
		cout << "Leaves Pending for Supervisor Approval" << endl << endl;
		// Read all leaves that are under supervisor review
		fileHandling->readSuperLeaveRecord(record);
		// Print all leaves that are under supervisor review
		consoleHandling->printLeaveConsole(record);
		system("pause");
	}

	// Function for employee to view leave records
	void viewLeaveRecord(string empid) {
		cout << "Employee Leave Records" << endl;

		// Read and print approved leaves of employee
		cout << endl << "Approved leaves:" << endl;
		fileHandling->readApprovedLeaveRecord(record,empid);
		consoleHandling->printLeaveConsole(record);
		// Clear records
		record.clear();
		
		// Read and print rejected leaves of employee
		cout << endl << "Rejected leaves:" << endl;
		fileHandling->readRejectedLeaveRecord(record,empid);
		consoleHandling->printLeaveConsole(record);
		// Clear records
		record.clear();

		// Read and print leaves of employee that are undr review by supervisor
		cout << endl << "Super approval pending leaves:" << endl;
		fileHandling->readSuperLeaveRecord(record,empid);
		consoleHandling->printLeaveConsole(record);
		// Clear records
		record.clear();

		// Read and print leaves of employee that are undr review by director
		cout << endl << "Director approval pending leaves:" << endl;
		fileHandling->readDirecLeaveRecord(record,empid);
		consoleHandling->printLeaveConsole(record);
		// Clear records
		record.clear();
		
		// Read and print cancelled leaves of employee
		cout << endl << "Cancelled leaves:" << endl;
		fileHandling->readCancelledLeaveRecord(record, empid);
		consoleHandling->printLeaveConsole(record);
		// Clear records
		record.clear();
	}

	// Function for supervisor and director to view any employee leave records
	void viewLeaveRecord() {
		int choice = 0;
		do {
			system("cls");
			string empid;
			cout << "Enter employee ID: ";
			cin >> empid;
			bool flag = false;
			leaveRecordI* balance = nullptr;
			{
				string readRecord = "";
				// Open employee data file in read mode
				ifstream read("Employee.txt");
				if (read.is_open()) {
					string fileuserid;
					// Read data from file
					while (getline(read, readRecord) && !flag) {
						stringstream ss(readRecord);
						ss >> fileuserid;
						// If employee id if found than set flag true
						if (fileuserid == empid) {
							flag = true;
							int casual, earned, official, year;
							ss >> casual >> earned >> official >> year;
							balance = new leaveRecord(casual, earned, official, year);
						}
					}
					read.close();
				}
				else
					cout << "Employee file not found!" << endl;
			}
			if (flag) {
				balance->print();
				viewLeaveRecord(empid);
				system("pause");
			}
			else
				cout << "No employee found" << endl;
			cout << "1- View another employee records" << endl
				<< "2- Return" << endl;
			choice = validInput(1, 2);
		} while (choice == 1);
	}
};
