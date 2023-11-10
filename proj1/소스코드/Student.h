#pragma once
#include <string>

#ifndef STUDENT_H__
#define STUDENT_H__
using namespace std;
class Student {
	string name, department, tel, student_id;
	int birth_year, total_credit, sort_type; //total_credit = ÃÑ ÇÐÁ¡
	double total_avg; //total_avg = ÀüÃ¼ Æò±Õ
public:
	Student() { }
	Student(string n, string s_id, int b_y, string d, string t, int t_c, double t_a) :
		name(n), student_id(s_id), birth_year(b_y), department(d), tel(t), total_credit(t_c), total_avg(t_a){
	}

	string get_name() {
		return name;
	}
	string get_department() {
		return department;
	}
	string get_tel() {
		return tel;
	}
	string get_student_id() {
		return student_id;
	}
	int get_birth_year() {
		return birth_year;
	}
	int get_total_credit() {
		return total_credit;
	}
	double get_total_avg() {
		return total_avg;
	}

	int get_sort_type() {
		return sort_type;
	}
	void set_sort_type(int sort_type) {
		this->sort_type = sort_type;
	}
	void set_name(string name) {
		this->name = name;
	}
	void set_department(string department) {
		this->department = department;
	}
	void set_tel(string tel) {
		this->tel = tel;
	}
	void set_student_id(string student_id) {
		this->student_id = student_id;
	}
	void set_birth_year(int birth_year) {
		this->birth_year = birth_year;
	}
	void set_total_credit(int total_credit) {
		this->total_credit = total_credit;
	}
	void set_total_avg(double total_avg) {
		this->total_avg = total_avg;
	}
	friend ostream& operator<< (ostream& os, Student st);
	bool operator< (Student& st);
};
#endif