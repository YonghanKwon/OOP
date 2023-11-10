#pragma once
#include <fstream>
#include "Student.h"
#ifndef STUDENT_DB_H
#define STUDENT_DB_H
class DB {
	int index;
	Student arr[100];
public:
	DB();
	ifstream fin;
	ofstream fout;
	void init(); // 초기화 함수
	void insert_DB();
	void search_DB(int src);
	void sort_DB(int srt_col, int srt_da);
	void exit_DB();
	int get_index()
	{
		return index;
	}
};
#endif