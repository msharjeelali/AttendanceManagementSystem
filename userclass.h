#pragma once

// This file conatain all the user related functionalities
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

// User class implementation
// User class
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
	virtual int getBalance(int) = 0;
	virtual void decBalance(int,int) = 0;
	virtual void incBalance(int,int) = 0;
};

// Class to deal users file handling
// Mainly it is going to check whether a user exist or not
class userFileHandling {
public:
	static string employee;				// Store employee file
	static string guard;				// Store Guard file
	static string director;				// Store Director file
	static string supervisor;			// Store Supervisor file

	// Function to check if employee passed exist or not
	static bool checkEmployeeExistance(string empid) {
		bool flag = false;
		string readRecord = "";
		ifstream read(employee);
		if (read.is_open()) {
			string fileuserid;
			while (getline(read, readRecord) && !flag) {
				stringstream ss(readRecord);
				ss >> fileuserid;
				if (fileuserid == empid)
					flag = true;
			}
		}
		else
			cout << "Employee file not found!" << endl;
		return flag;
	}

	// Function that return all the employee to generate reports
	static vector<string> getAllEmployee() {
		vector<string> userList;
		ifstream read(employee);
		if (read.is_open()) {
			string fileUserID, temp;
			while (getline(read, temp)) {
				stringstream ss(temp);
				ss >> fileUserID;
				userList.push_back(fileUserID);
			}
		}
		else
			cout << "Employee file not found!" << endl;
		return userList;
	}

	static void updateBalance(user* current) {
		struct Employee {
			string empid = "";
			string pass = "";
			string first = "";
			string last = "";
			int casual = 0;
			int earned = 0;
			int official = 0;
		};
		
		ifstream read(employee);
		vector<Employee> list;
		if (read.is_open()) {
			string temp;
			while (getline(read, temp)) {
				string empid, pass, first, last;
				int casual = 0, earned = 0, official= 0;
				stringstream ss(temp);
				
				Employee object;
				ss >> object.empid >> object.pass >> object.first >> object.last >> object.casual >> object.earned >> object.official;
				list.push_back(object);
			}
		}
		else
			cout << "Cannot update leave balance" << endl;

		for (auto& ite : list) {
			if (ite.empid == current->getID()) {
				ite.casual = current->getBalance(1);
				ite.earned = current->getBalance(2);
				ite.official = current->getBalance(3);
			}
		}

		ofstream write(employee);
		if (write.is_open()) {
			for (auto& ite : list) {
				write << ite.empid << " " << ite.pass << " " << ite.first << " " << ite.last << " "
					<< ite.casual << " " << ite.earned << " " << ite.official << endl;
			}
		}
		else
			cout << "Cannot update leave balance" << endl;
	}

	static void updateBalance(string empid,string type,int count) {
		struct Employee {
			string empid = "";
			string pass = "";
			string first = "";
			string last = "";
			int casual = 0;
			int earned = 0;
			int official = 0;
		};

		ifstream read(employee);
		vector<Employee> list;
		if (read.is_open()) {
			string temp;
			while (getline(read, temp)) {
				string empid, pass, first, last;
				int casual = 0, earned = 0, official = 0;
				stringstream ss(temp);

				Employee object;
				ss >> object.empid >> object.pass >> object.first >> object.last >> object.casual >> object.earned >> object.official;
				list.push_back(object);
			}
		}
		else
			cout << "Cannot update leave balance" << endl;

		for (auto& ite : list) {
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

		ofstream write(employee);
		if (write.is_open()) {
			for (auto& ite : list) {
				write << ite.empid << " " << ite.pass << " " << ite.first << " " << ite.last << " "
					<< ite.casual << " " << ite.earned << " " << ite.official << endl;
			}
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