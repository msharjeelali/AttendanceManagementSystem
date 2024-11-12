#include<iostream>
#include<string>
#include<string>
using namespace std;

#pragma once
class Date
{
	int day;
	int month;
	int year;
public:
	Date(int day = 0, int month = 0, int year = 0 ) {
		this->day = day;
		this->month = month;
		this->year = year;
	}
	Date(const Date& obj) {
		this->day = obj.day;
		this->month = obj.month;
		this->year = obj.year;
	}

	bool valid() const {
		if (day < 1 or day > 31 or month < 1 or month > 12)
			return false;
		return true;
	}
	bool compare(string date) {
		if (date.length() >= 3 and date.find('/') != string::npos) {
			int index = 0;
			string temp;
			while (date[index] != '/')
				temp += date[index++];
			if (this->day != stoi(temp))
				return false;

			index++;
			temp.clear();
			while (index < date.length())
				temp += date[index++];
			if (this->month != stoi(temp))
				return false;
			return true;
		}
		else
			return false;
	}
	static Date convert(string date) {
		int day, month, year;
		int index = 0;
		string temp;
		while (date[index] != '/')
			temp += date[index++];
		day = stoi(temp);

		index++;
		temp.clear();
		while (date[index] != '/')
			temp += date[index++];
		month = stoi(temp);

		index++;
		temp.clear();
		while (index < date.length())
			temp += date[index++];
		year = stoi(temp);

		return Date(day, month, year);
	}

	// Required Operators Overloaded
	bool operator < (Date obj) const {
		return this->day < obj.day and this->month < obj.month and this->year < obj.year;
	}
	Date& operator = (const Date& obj) {
		this->day = obj.day;
		this->month = obj.month;
		this->year = obj.year;
		return *this;
	}
	bool operator == (Date obj) const {
		if (this->day != obj.day)
			return false;
		if (this->month != obj.month)
			return false;
		if (this->year != obj.year)
			return false;
		return true;
	}
	friend ostream& operator << (ostream& out, const Date& today) {
		out << today.day << "/" << today.month << "/" << today.year;
		return out;
	}
	friend ofstream& operator << (ofstream& out, const Date& today) {
		out << today.day << "/" << today.month << "/" << today.year;
		return out;
	}
	friend istream& operator >> (istream& in, Date& today) {
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
	friend ifstream& operator >> (ifstream& in, Date& today) {
		string date;
		in >> date;
		
		int index = 0;
		string temp;
		while (date[index] != '/')
			temp += date[index++];
		today.day = stoi(temp);

		index++;
		temp.clear();
		while (date[index] != '/')
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
	int hour;
	int min;
public:
	Time(int hour = 0, int min = 0) {
		this->hour = hour;
		this->min = min;
	}
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
	// Function to cenvert string to time
	static Time convert(string time) {
		if (time.find(':') == string::npos)
			cout << "Invalid time format" << endl;
		
		int hour = stoi(time.substr(0, time.find(':')));
		int min = stoi(time.substr(time.find(':')+1));

		return Time(hour, min);
	}
	// Function to check validity of date
	static bool checktime(Time entry, Time exit) {
		if (entry.hour > exit.hour)
			return false;
		else if (entry.hour == exit.hour and entry.min > exit.min)
			return false;
		return true;
	}


	// Required Operators Overloaded
	Time operator - (const Time& obj) const{
		int first = this->hour * 60 + this->min;
		int second = obj.hour * 60 + obj.min;
		
		int res = first - second;
		if (res < 0)
			return Time(0, 0);
		return Time(res / 60, res % 60);
	}
	Time& operator = (const Time& obj) {
		if(!this->compare(obj))
			this->hour = obj.hour;
			this->min = obj.min;
		return *this;
	}
	friend ostream& operator << (ostream& out, const Time& time) {
		out << time.hour << ":" << time.min;
		return out;
	}
	friend ofstream& operator << (ofstream& out,const Time& time) {
		out << time.hour << ":" << time.min;
		return out;
	}
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
