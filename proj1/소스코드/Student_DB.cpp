#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>
#include "Student.h"
#include "Student_DB.h"
using namespace std;

DB::DB() { //DB생성자, index 현재 배열에 들어있는 데이터 크기. 파일 읽어오면 그 자리로 다시 이동 해야 함
	fin.open("file1.txt");
	if (!fin) {
		fout.open("file1.txt", ios::app);
	}
	else {
		fout.open("file1.txt", ios::out | ios::in);
	}
	//이유는 모르겠지만 flag를 이렇게 써야만 전에거 읽으면서 수정도 가능함
	//report 화이팅!
}

void DB::init() { //student배열 초기화, txt파일에서 읽어옴

	string line, name, department, tel, stu_id, bir_year_str, total_cred_str, total_avg_str;
	int student_id, birth_year, total_cred;
	double total_avg;

	index = 0;
	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			name = line.substr(0, 15);//공백제거 완료
			for (int i = 0; i < name.size(); i++) {
				if (name[i] == ' ' && name[i + 1] == ' ') {
					name = name.substr(0, i);
					break;
				}
			}
			
			stu_id = line.substr(17, 10);//형변환 완료
			

			department = line.substr(29, 18);//공백제거 완료
			for (int i = 0; i < department.size(); i++) {
				if (department[i] == ' ' && department[i + 1] == ' ') {
					department = department.substr(0, i);
					break;
				}
			}

			bir_year_str = line.substr(49, 4);//형변환 완료
			int bir_year = stoi(bir_year_str);

			tel = line.substr(61, 12);//변화시킬 없음

			total_cred_str = line.substr(74, 8);//공백 제거 및 형변환 완료
			for (int i = 0; i < total_cred_str.size(); i++) {
				if (total_cred_str[i] == ' ' && total_cred_str[i + 1] == ' ') {
					total_cred_str = total_cred_str.substr(0, i);
					break;
				}
			}
			total_cred = stoi(total_cred_str);

			total_avg_str = line.substr(90, 8);//공백 제거 및 형변환 완료
			for (int i = 0; i < total_avg_str.size(); i++) {
				if (total_avg_str[i] == ' ' && total_avg_str[i + 1] == ' ') {
					total_avg_str = total_avg_str.substr(0, i);
					break;
				}
			}
			total_avg = stod(total_avg_str);

			arr[index++] = Student(name, stu_id, bir_year, department, tel, total_cred, total_avg);
		}
	}
}

void DB::insert_DB() { //student 배열에 입력
	string name, department, tel, student_id;
	int birth_year, total_credit;
	int checker = 0;//중복을 확인하기 위한 변수
	double total_avg;
	cout << "Name? ";
	getline(cin, name);

	do {
		checker = 0;
		cout << "Student ID(10digits)? ";//중복확인
		cin >> student_id;
		for (int i = 0; i < index; i++)
		{
			if (student_id == arr[i].get_student_id())
			{
				cout << "Error: Already inserted" << endl;
				checker = 1;
				break;
			}
		}
	} while (checker != 0 || student_id.length() != 10);
	do {
		cout << "Birth Year(4digits)? ";
		cin >> birth_year;
		cin.ignore();
	} while (birth_year < 1000 || birth_year > 9999);
	cout << "Department? ";
	getline(cin, department);
	do {
		checker = 0;
		cout << "Tel (12digits)? ";//중복확인
		getline(cin, tel);
		for (int i = 0; i < index; i++) {
			if (tel.compare(arr[i].get_tel()) == 0)
			{
				cout << "Error: Already inserted" << endl;
				checker = 1;
				break;
			}
		}
	} while (checker != 0 || tel.length() != 12);
	do {
		cout << "Total Credit(0 ~ 120)? ";
		cin >> total_credit;
		if (total_credit < 0 || total_credit>120)
			cout << "Wrong Range!" << endl;
	} while (total_credit < 0 || total_credit>120);
	do {
		cout << "Total Credit Average(0.0 ~ 4.5)? ";
		cin >> total_avg;
		if (total_avg < 0 || total_avg>4.5)
			cout << "Wrong Range!" << endl;
	} while (total_avg < 0.0 || total_avg > 4.5);
	cin.ignore();

	arr[index++] = Student(name, student_id, birth_year, department, tel, total_credit, total_avg);
	fout << arr[index - 1] << endl;
}

void DB::search_DB(int src) { //student 배열에서 검색, 1 ~ 5는 pdf에 있는 거 보고 만든거라 수정하셔도 됩니다
	string input_str;//name,department,tel등의 string을 입력받음
	char mod;
	int check_mod = 0, correct_val, f_now = 0, i_m;
	int check = 0;// 중복 확인을 위한 변수(student_id, tel)
	int check_i = 0;// 중복 확인용 int(student_id)
	string str_m;
	string check_str;//확인용 str(tel)
	int input_i;//birth_year,student_id등의 integer값을 입력받음
	int lower_i, higher_i;//total_credit의 범위값을 받기 위한 변수.
	double input_d;//큰 숫자를 받기 위한 변수
	double d_m;
	double lower_d, higher_d;///total_avg의 범위값을 받기위한 변수
	int checker = 0;//입력 값이 student배열의 요소에 존재하는지 check함
	switch (src) {
	case(1):
		cout << "Name?";
		cin >> input_str;
		for (int i = 0; i < index; i++)
		{
			if (input_str.compare(arr[i].get_name()) == 0) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
				cout << arr[i] << endl;
			}
		}
		if (checker == 0)
			cout << "There is No one whose name is " << input_str << endl;
		break;
	case(2):
		cout << "Student ID(10digits)?";
		cin >> input_str;
		for (int i = 0; i < index; i++)
		{
			if (input_str == arr[i].get_student_id()) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
				cout << arr[i] << endl;

				do {// 수정하기 위해선 txt 파일에 접근해야할 필요가 있음.
					cout << "Do you want to change the data?(Y/N)";
					cin >> mod;
					if ((mod == 'y') | (mod == 'Y')) {
						check_mod = 1;
						cout << "1. Name" << endl << "2. Student ID" << endl << "3. Dept." << endl << "4. Birth_Year" << endl << "5.Tel" <<
							endl << "6. Total Credit" << endl << "7.Total Average" << endl << "8. all" << endl;
						cout << "> " << endl;
						cin >> correct_val;
						switch (correct_val) {
						case 1:
							cout << "name?";
							cin >> str_m;
							arr[i].set_name(str_m);
							break;
						case 2:
							do {
								cout << "Student ID(10digits)? ";
								cin >> str_m;
								cin.ignore();
								for (int i = 0; i < index; i++) {
									if (str_m == arr[i].get_student_id()) {
										cout << "same ID is already in data" << endl;
										str_m = "";
										break;
									}
								}
							} while (str_m.length() != 10); break;
						case 3:
							cout << "Department? ";
							cin >> str_m;
							arr[i].set_department(str_m); break;
						case 4:
							do {
								cout << "Birth Year(4digits)? ";
								cin >> i_m;
								arr[i].set_birth_year(i_m);
							} while (i_m < 1000 || i_m>9999); break;
						case 5:
							do {
								cout << "tel(12digit)";
								cin >> str_m;
								cin.ignore();
								for (int i = 0; i < index; i++) {
									if (str_m == arr[i].get_tel()) {
										cout << "same tel is already in data" << endl;
										str_m = "";
										break;
									}
								}
								arr[i].set_tel(str_m);
							} while (str_m.length() != 12); break;
						case 6:
							do {
								cout << "Total Credit(0 ~ 120)? ";
								cin >> i_m;
								arr[i].set_total_credit(i_m);
								cin.ignore();
							} while (i_m > 120 || i_m < 0); break;
						case 7:
							do {
								cout << "Total Credit Average(0.0 ~ 4.5)? ";
								cin >> d_m;
								arr[i].set_total_avg(d_m);
								cin.ignore();
							} while (d_m < 0 || d_m>4.5); break;
						case 8:
							insert_DB();
						}
					}
					else if ((mod == 'n') | (mod == 'N'))
						check_mod = 1;
				} while (check_mod == 0);
			}
		}
		if (checker == 0)
			cout << "There is NO one whose ID is " << input_str << endl;
		break;
	case(3):
		cout << "Birth year?";
		cin >> input_i;
		for (int i = 0; i < index; i++)
		{
			if (input_i == arr[i].get_birth_year()) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시력 전 표시
				cout << arr[i] << endl;

			}
		}
		if (checker == 0)
			cout << "There is No one whose Birth year is " << input_i << endl;
		break;
	case(4):
		cout << "Department?";
		cin >> input_str;
		for (int i = 0; i < index; i++)
		{
			if (input_str.compare(arr[i].get_department()) == 0) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
				cout << arr[i] << endl;
				checker = 1;
			}
		}
		if (checker == 0)
			cout << "There is No one whose department is " << input_str << endl;
		break;
	case(5):
		do {
			cout << "Enter the searching total credit range(two integer) (ex 90 100) ";
			cin >> lower_i >> higher_i;
			if (lower_i > higher_i)
				cout << "Wrong range! enter correct range." << endl;
		} while (lower_i > higher_i);
		for (int i = 0; i < index; i++)
		{
			if (lower_i <= arr[i].get_total_credit() && arr[i].get_total_credit() <= higher_i) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
				cout << arr[i] << endl;
			}
		}
		if (checker == 0)
			cout << "There is No one whose total credit is range in " << lower_i << " and " << higher_i << endl;
		break;
	case(6):
		do {
			cout << "Enter the searching total average grade range(two floating point) (ex 0.0 4.5) ";
			cin >> lower_d >> higher_d;
			if (lower_d > higher_d)
				cout << "Wrong range! enter correct range." << endl;
		} while (lower_d > higher_d);
		for (int i = 0; i < index; i++)
		{
			if (lower_d <= arr[i].get_total_avg() && arr[i].get_total_avg() <= higher_d) {
				checker++;
				if (checker == 1)
					cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
					setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
				cout << arr[i] << endl;
			}
		}
		if (checker == 0)
			cout << "There is No one whose total average grade is range in " << lower_d << " and " << higher_d << endl;
		break;
	case(7):
		cout << left << setw(17) << "Name" << setw(12) << "Student ID" << setw(20) << "Dept." << setw(12) << "Birth_Year" << setw(13) << "Tel" <<
			setw(16) << "Total Credit" << setw(16) << "Total Average" << endl; //student 출력 전 표시
		for (int i = 0; i < index; i++) {
			cout << arr[i] << endl;
		}
		break;
	default:
		cout << "Wrong Input!" << endl;

	}
}

void DB::sort_DB(int sort_type, int direction) { //student 배열에서 정렬, 1 ~ 5는 pdf에 있는 거 보고 만든거라 수정하셔도 됩니다
	for (int i = 0; i < index; i++) {
		arr[i].set_sort_type(sort_type);
	}
	sort(arr, arr + index);
	if (direction == 2) {
		reverse(arr, arr + index);
	}

	fout.close();
	fout.open("file1.txt", ios::out | ios::in);
	fout.seekp(0, ios::beg);
	for (int i = 0; i < index; i++) {
		fout << arr[i] << endl;
	}
}

void DB::exit_DB() {
	fin.close();
	fout.close();
}