#pragma once

// This file conatain all the user related functionalities
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

// User class implementation
class user
{
protected:
	// Store user id
	string id;				
	// Store frist name
	string firstName;		
	// Store last name
	string lastName;		
public:
	// Default and overloaded constructor
	user(string id = "NA", string firstname = "NA", string lastname = "NA") {
		this->id = id;
		this->firstName = firstname;
		this->lastName = lastname;
	}

	// Function to return id of user
	string getID() {
		return id;
	}

	// Function to return name of user
	string getName() {
		return firstName + lastName;
	}

	// Function to print details of use on console
	virtual void getDetails() {
		cout << "User id: " << id << endl << "Name: " << firstName << " " << lastName << endl;
	}

	// Pure virtual function of menu
	// Different menu for different type of user
	virtual void menu() = 0;

	// Pure virtual function for balance related functionalities
	// Only useful for employee user type
	virtual int getBalance(int) = 0;
	virtual void decBalance(int,int) = 0;
	virtual void incBalance(int,int) = 0;
};

// Class to deal users file handling
// Mainly it is going to check whether a user exist or not
class userFileHandling {
public:
	// Store employee file name
	static string employee;				
	// Store Guard file name
	static string guard;		
	// Store Director file name
	static string director;	
	// Store Supervisor file name
	static string supervisor;	

	// Function to check if employee passed exist or not
	static bool checkEmployeeExistance(string empid) {
		bool flag = false;
		string readRecord = "";
		// Open employee data file in read mode
		ifstream read(employee);
		if (read.is_open()) {
			string fileuserid;
			// Read data from file
			while (getline(read, readRecord) && !flag) {
				stringstream ss(readRecord);
				ss >> fileuserid;
				// If employee id if found than set flag true
				if (fileuserid == empid)
					flag = true;
			}
			read.close();
		}
		else
			cout << "Employee file not found!" << endl;
		// Return true if employee is found
		// Return false if employee is not found
		return flag;
	}

	// Function that return all the employee to generate reports
	static vector<string> getAllEmployee() {
		// Vector to store all employes id
		vector<string> userList;
		// Open employee file in read mode
		ifstream read(employee);
		if (read.is_open()) {
			string fileUserID, temp;
			// Read file data in temp vairable
			while (getline(read, temp)) {
				// Extract employee id from line 
				stringstream ss(temp);
				ss >> fileUserID;
				// Puch employee id in vector
				userList.push_back(fileUserID);
			}
			read.close();
		}
		else
			cout << "Employee file not found!" << endl;
		// Return list if employee id
		return userList;
	}
	
	// Function to update information of employee
	static void updateBalance(user* current) {
		// Temporary structure to store information of employee
		// Instead of creating employee objects temporarily store information in struct
		struct Employee {
			string empid = "";
			string pass = "";
			string first = "";
			string last = "";
			int casual = 0;
			int earned = 0;
			int official = 0;
		};
		
		// Open employee data file in read mode
		ifstream read(employee);
		// Vector to store information of all employees
		vector<Employee> list;
		if (read.is_open()) {
			string temp;
			// Read information from the file
			while (getline(read, temp)) {
				string empid, pass, first, last;
				int casual = 0, earned = 0, official= 0;
				stringstream ss(temp);

				// Create struct object and store information in it
				Employee object;
				ss >> object.empid >> object.pass >> object.first >> object.last >> object.casual >> object.earned >> object.official;
				// Push object in vector
				list.push_back(object);
			}
			read.close();
		}
		else
			cout << "Cannot update leave balance" << endl;

		// Iterate through list of objects
		for (auto& ite : list) {
			// Update record of specific employee
			if (ite.empid == current->getID()) {
				ite.casual = current->getBalance(1);
				ite.earned = current->getBalance(2);
				ite.official = current->getBalance(3);
			}
		}
		
		// Open employee file in writing mode
		ofstream write(employee);
		if (write.is_open()) {
			// Write back all the record in file
			for (auto& ite : list) {
				write << ite.empid << " " << ite.pass << " " << ite.first << " " << ite.last << " "
					<< ite.casual << " " << ite.earned << " " << ite.official << endl;
			}
			write.close();
		}
		else
			cout << "Cannot update leave balance" << endl;
	}

	// Function to update information of employee
	static void updateBalance(string empid,string type,int count) {
		// Instead of creating employee objects temporarily store information in struct
		// Temporary structure to store information of employee
		struct Employee {
			string empid = "";
			string pass = "";
			string first = "";
			string last = "";
			int casual = 0;
			int earned = 0;
			int official = 0;
		};

		// Vector to store information of all employees
		vector<Employee> list;
		// Open employee data file in read mode
		ifstream read(employee);
		if (read.is_open()) {
			string temp;
			// Read information from the file
			while (getline(read, temp)) {
				string empid, pass, first, last;
				int casual = 0, earned = 0, official = 0;
				stringstream ss(temp);

				// Create struct object and store information in it
				Employee object;
				ss >> object.empid >> object.pass >> object.first >> object.last >> object.casual >> object.earned >> object.official;
				// Push object in vector
				list.push_back(object);
			}
			read.close();
		}
		else
			cout << "Cannot update leave balance" << endl;

		// Iterate through list of objects
		for (auto& ite : list) {
			// Update record of specific employee
			if (ite.empid == empid) {
				if (type == "casual") {
					ite.casual += count;
				} else if(type == "earned") {
					ite.earned += count;
				} else if (type == "official") {
					ite.official -= count;
				}
			}
		}

		// Open employee file in writing mode
		ofstream write(employee);
		if (write.is_open()) {
			// Write back all the record in file
			for (auto& ite : list) {
				write << ite.empid << " " << ite.pass << " " << ite.first << " " << ite.last << " "
					<< ite.casual << " " << ite.earned << " " << ite.official << endl;
			}
			write.close();
		}
		else
			cout << "Cannot update leave balance" << endl;
	}
};

// Initialization of all static members of userFileHandling class
string userFileHandling::employee = "Employee.txt";
string userFileHandling::guard = "Guard.txt";
string userFileHandling::supervisor = "Supervisor.txt";
string userFileHandling::director = "Director.txt";