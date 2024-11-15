#include<iostream>
#include<string>
#include<string>
using namespace std;

#pragma once
class Date
{
	int day;	// Store numeric date
	int month;	// Store numeric month
	int year;	// Store numeric year
public:
	// Deafault and overloaded constructor
	Date(int day = 0, int month = 0, int year = 0 ) {
		this->day = day;
		this->month = month;
		this->year = year;
	}

	// Copy constructor using deeo copy
	Date(const Date& obj) {
		this->day = obj.day;
		this->month = obj.month;
		this->year = obj.year;
	}

	// Function to check wether date stored as object is valid or not (i.e. True means valid date)
	bool valid() const {
		if (month < 1 || month > 12)
			return false;

		int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		if (day < 1 || day > daysInMonth[month])
			return false;

		return true;
	}

	// Function which takes string as argument and check if date stored in calling object is same as date passsed as string
	bool compare(string date) {
		if (date.length() >= 3 and date.find('/') != string::npos)		// Condition to check if string is in proper format i.e. ( DD/MM )
		{
			int index = 0;
			string temp;
			while (date[index] != '/')									// Loop till we get first slash i.e. delimeter
				temp += date[index++];
			if (this->day != stoi(temp))								// Stoi function which convert string to integer (date)
				return false;

			index++;
			temp.clear();
			while (index < date.length())								// Loop till we get second slash i.e. delimeter
				temp += date[index++];
			if (this->month != stoi(temp))								// Stoi function which convert string to integer (month)
				return false;
			return true;
		}
		else
			return false;
	}

	// Overloading less than operator to check if calling object is less (prev date) than passed object
	bool operator < (Date obj) const {
		if (year != obj.year)
			return year < obj.year;
		if (month != obj.month)
			return month < obj.month;
		return day < obj.day;
	}
	
	// Overloading assignment operator to assign one object vaue to another object
	Date& operator = (const Date& obj) {
		this->day = obj.day;
		this->month = obj.month;
		this->year = obj.year;
		return *this;
	}
	
	// Overloading comparison operator to check if two dates are equal or not
	bool operator == (Date obj) const {
		if (this->day != obj.day)
			return false;
		if (this->month != obj.month)
			return false;
		if (this->year != obj.year)
			return false;
		return true;
	}
	
	// Overloading cout operaotor to print date in specific format on console
	friend ostream& operator << (ostream& out, const Date& today) {
		out << today.day << "/" << today.month << "/" << today.year;
		return out;
	}
	
	// Overloading ifstream operaotor to write date in specific format in file
	friend ofstream& operator << (ofstream& out, const Date& today) {
		out << today.day << "/" << today.month << "/" << today.year;
		return out;
	}
	
	// Overloading cin operator which takes input in string and convert it into Date object
	friend istream& operator >> (istream& in, Date& today)
	{
		string date;
		in >> date;
		
		if (date.length() >= 3 and date.find('/') != string::npos) {
			int index = 0;
			string temp;
			while (date[index] != '/')
				temp += date[index++];
			today.day = stoi(temp);

			index++;
			temp.clear();
			while (index < date.length())
				temp += date[index++];
			today.month = stoi(temp);
		
			today.year = 2024;
		}
		return in;
	}
	
	// Overloading ofstream operator which read data from file and convert it into Date object
	friend ifstream& operator >> (ifstream& in, Date& today) {
		string date;
		in >> date;
		
		int index = 0;
		string temp;
		while (date[index] != '/' and index < date.length())
			temp += date[index++];
		today.day = stoi(temp);

		index++;
		temp.clear();
		while (date[index] != '/' and index < date.length())
			temp += date[index++];
		today.month = stoi(temp);

		index++;
		temp.clear();
		while (index < date.length())
			temp += date[index++];
		today.year = stoi(temp);
		
		return in;
	}
};

class Time {
	int hour;		// Stores hour in integer
	int min;		// Stores min in integer
public:
	// Default constructor and overloaded constructor
	Time(int hour = 0, int min = 0) {
		this->hour = hour;
		this->min = min;
	}
	// Copy construtor using deep copy
	Time(const Time& obj) {
		this->hour = obj.hour;
		this->min = obj.min;
	}

	// Function to check whether time is valid or not
	bool valid() const {
		return !(hour < 0 or hour > 23 or min < 0 or min > 59);
	}

	// Function to comapre to time objects
	bool compare(Time obj) const{
		return (min == obj.min) and (hour == obj.hour);
	}

	// Function to check validity of time i.e. whether entry time entered is earlier than exit time
	static bool checktime(Time entry, Time exit) {
		if (entry.hour > exit.hour)
			return false;
		else if (entry.hour == exit.hour and entry.min > exit.min)
			return false;
		return true;
	}

	// Overloading minus operator to subtract to Time objects and get thier difference in form of Time object
	Time operator - (const Time& obj) const{
		int first = this->hour * 60 + this->min;
		int second = obj.hour * 60 + obj.min;
		
		int res = first - second;
		if (res < 0)
			return Time(0, 0);
		return Time(res / 60, res % 60);
	}

	// Overloading assignment operator to assign one object vaue to another object
	Time& operator = (const Time& obj) {
		if(!this->compare(obj))
			this->hour = obj.hour;
			this->min = obj.min;
		return *this;
	}

	// Overloading cout operaotor to print time in specific format on console
	friend ostream& operator << (ostream& out, const Time& time) {
		out << time.hour << ":" << time.min;
		return out;
	}
	
	// Overloading ofstream operaotor to write date in specific format in file
	friend ofstream& operator << (ofstream& out,const Time& time) {
		out << time.hour << ":" << time.min;
		return out;
	}
	
	// Overloading cin operator which takes input in string and convert it into Time object
	friend istream& operator >> (istream& in, Time& time) {
		string intime;
		in >> intime;

		if (intime.length() >= 3 and intime.find(':') != string::npos) {
			int index = 0;
			string temp;
			while (intime[index] != ':')
				temp += intime[index++];
			time.hour = stoi(temp);

			index++;
			temp.clear();
			while (index < intime.length())
				temp += intime[index++];
			time.min = stoi(temp);
		}
		return in;
	}

	// Overloading ofstream operator which read data from file and convert it into Date object
	friend ifstream& operator >> (ifstream& in, Time& time) {
		string intime;
		in >> intime;

		int index = 0;
		string temp;
		while (intime[index] != ':')
			temp += intime[index++];
		time.hour = stoi(temp);

		index++;
		temp.clear();
		while (index < intime.length())
			temp += intime[index++];
		time.min = stoi(temp);

		return in;
	}
};
