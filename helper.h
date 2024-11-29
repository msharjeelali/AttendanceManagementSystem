#pragma once

#include<iostream>
#include<string>
#include<string>
using namespace std;

// Function to validate menu input
int validInput(int start, int end) {
	int choice;
	do {
		cout << "Enter your choice: ";
		cin >> choice;					// Get user choice
		if (cin.fail()) {				// If not an interger
			cin.clear();
			cin.ignore();
		}
	} while (choice< start || choice > end);	// Loop untill input is not in range
	return choice;

}
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
		if (date.length() >= 3 && date.find('/') != string::npos)		// Condition to check if string is in proper format i.e. ( DD/MM )
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

	// Function to check if calling date lies between the given two obj or not
	bool between(Date& start, Date& end) const{
		if (start == *this || end == *this || (start < *this && *this < end))
			return true;
		return false;
	}

	// Function to get week day from date object
	string getDay() const {
		int t_year = year, t_month = month, t_day = day;
		string weekday[7] = { "Sun","Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		if (t_month < 3) {
			t_year -= 1; // Previous year for Jan and Feb
			t_month += 12; // Treat Jan and Feb as 13 and 14

		}
		int index = (t_year + t_year / 4 - t_year / 100 + t_year / 400 + t[t_month - 1] + t_day) % 7;

		return weekday[index];
	}

	// Function to get month of date
	int getMonth()const {
		return month;
	}

	// Function to determine week range from Date provided
	static Date getWeekRange(Date date, string day) {
		vector<string>weekday{ "Mon", "Tues", "Wed", "Thur", "Fri" };
		int index = find(weekday.begin(), weekday.end(), day) - weekday.begin() + 1;

		// Calculate week start and end dates to group them weekly
		Date rangefrom = date - index;
		return rangefrom;
	}

	// Overloading plus operator to calcualte ranges of week in weekly report
	Date operator + (int day) const {
		Date result = *this;
		int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		
		result.day += day;
		if (result.day > daysInMonth[result.month] ) {
			result.day = result.day % daysInMonth[result.month];
			result.month++;
			if (result.month > 12) {
				result.month = 1;
				result.year++;
			}
		}

		return result;
	}

	// Overloading minus operator to calculate ranges of week in weekly report
	Date operator - (int day) const {
		Date result = *this;
		int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		result.day -= day;
		if (result.day < 1) {
			result.day = daysInMonth[result.month] - result.day;
			result.month--;
			if (result.month < 1) {
				result.month = 12;
				result.year--;
			}
		}

		return result;
	}

	// Function to get number of days between two dates
	int getDiff(Date obj) {
		int count = 0;
		for (Date i = *this; i > obj || i == obj; i = i - 1)
			count++;
		return count;
	}

	// Overloading greater than operator to check if calling object is greater (ahead date) than passed object
	bool operator > (Date obj) const {
		if (year != obj.year)
			return year > obj.year;
		if (month != obj.month)
			return month > obj.month;
		return day > obj.day;
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
		if (today.day == 0 && today.month == 0)
			out << "NotAvailable";
		else
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
		
		if (date.length() >= 3 && date.find('/') != string::npos) {
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
		string temp = "";
		while (date[index] != '/' && index < date.length())
			temp += date[index++];
		today.day = stoi(temp);

		index++;
		temp.clear();
		while (date[index] != '/' && index < date.length())
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
		return !(hour < 0 || hour > 23 || min < 0 || min > 59);
	}

	// Function to comapre to time objects
	bool compare(Time obj) const{
		return (min == obj.min) && (hour == obj.hour);
	}

	// Function to get hours from time to calculate weekly report
	int getHours()const {
		float sum = 0;
		sum += this->hour;
		sum += float(this->min / 60);
		return sum;
	}

	// Function to check validity of time i.e. whether entry time entered is earlier than exit time
	static bool checktime(Time entry, Time exit) {
		if (entry.hour > exit.hour)
			return false;
		else if (entry.hour == exit.hour && entry.min > exit.min)
			return false;
		return true;
	}

	// Overloading minus operator to subtract two Time objects and get thier difference in form of Time object
	Time operator - (const Time& obj) const{
		int first = this->hour * 60 + this->min;
		int second = obj.hour * 60 + obj.min;
		
		int res = first - second;
		if (res < 0)
			return Time(0, 0);
		return Time(res / 60, res % 60);
	}

	// Overloading plus operator to add working hours for weekly report
	Time operator + (const Time& obj) const {
		int first = this->hour * 60 + this->min;
		int second = obj.hour * 60 + obj.min;

		int res = first + second;
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

		if (intime.length() >= 3 && intime.find(':') != string::npos) {
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
