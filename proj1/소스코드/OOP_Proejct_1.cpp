#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include "Student_DB.h"
using namespace std;
int main(void) {
	DB data_base;
	int menu = 0, search = 0, sort_type = 1, direction = 1, index;

	data_base.init();

	while (menu != 4) {
		data_base.sort_DB(sort_type, direction);

		cout << "1. Insertion" << endl << "2. Search" << endl << "3. Sorting Oprtion" << endl << "4. Exit" << endl << "> ";
		cin >> menu;
		cin.ignore();

		if (menu == 1) { //Insertion
			data_base.insert_DB();
		}
		else if (menu == 2) {
			index = data_base.get_index();
			if (index == 0)
			{
				cout << "There is no data, please enter the data" << endl;
			}
			else {
				//Searching   admission year은 id에서 확인 가능할것이라 판단하여 birth year로 바꾸어놓겠습니다. 혹시 admission year가 필요하다면 연락주세요-용한
				cout << "1. Search by name" << endl << "2. Search by student ID(to modify the data, enter here)" << endl << "3. Search by birth year" << endl <<
					"4. Search by department name" << endl << "5. Search by total creadit" << endl << "6. Search by total grade" << endl << "7. List All" << endl << "> ";
				cin >> search;
				data_base.search_DB(search);
			}
		}
		else if (menu == 3) { //Sorting
			index = data_base.get_index();
			if (index == 0)
			{
				cout << "There is no data, please enter the data" << endl;
			}
			else {
				cout << "1. Sort by name" << endl << "2. Sort by student ID" << endl << "3. Sort by birth year" << endl << "4. Sort by department name" << endl <<
					"5. Sort by total credit" << endl << "6. Sort by total average" << endl << "> ";
				cin >> sort_type;
				cout << "1.오름차순" << endl << "2.내림차순" << endl;
				cin >> direction;
			}
		}
		else if (menu == 4)
		{
			data_base.exit_DB();
			break;
		}
	}
}