#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

inf_int::inf_int() {// assign 0 as a default value
	this->digits = new char[2];
	this->digits[0] = '0';
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}
inf_int::inf_int(int n) {
	this->length = 0;
	int copy = n;
	if (n >= 0)
		this->thesign = true;
	else {
		this->thesign = false;
		n = -n;
	}
	if (n == 0)
	{
		this->digits = new char[2];
		this->digits[0] = '0';
		this->digits[1] = '\0';
		this->length = 1;
		this->thesign = true;
	}
	else {
		while (copy != 0)
		{
			copy = copy / 10;
			this->length++;
		}
		this->digits = new char[length + 1];
		int i = 0;
		while (n != 0)
		{
			this->digits[i] = (n % 10) + 48;
			n = n / 10;
			i++;
		}
		this->digits[length] = '\0';
	}
}
inf_int::inf_int(const char* n) {
	if (n[0] == '-') {
		this->thesign = false;
		this->length = strlen(n) - 1;
		this->digits = new char[length + 1];
		for (int i = 0; i < length; i++)
		{
			this->digits[i] = n[length - i];
		}
		this->digits[length] = '\0';
	}
	else {
		this->thesign = true;
		this->length = strlen(n);
		this->digits = new char[length + 1];
		for (int i = 0; i < length; i++)
		{
			this->digits[i] = n[length - 1 - i];
		}
		this->digits[length] = '\0';
	}
}
inf_int::inf_int(const inf_int& copy) {// copy constructor
	this->thesign = copy.thesign;
	this->length = copy.length;
	this->digits = new char[copy.length + 1];
	strcpy(this->digits, copy.digits);
}
inf_int::~inf_int() { // destructor
	delete[] this->digits;

}

inf_int& inf_int::operator=(const inf_int& copy) // assignment operator
{
	if (this->digits)
	{
		delete[] this->digits;
	}
	this->thesign = copy.thesign;
	this->length = copy.length;
	this->digits = new char[copy.length + 1];
	strcpy(this->digits, copy.digits);
	return *this;

}

bool operator==(const inf_int& a, const inf_int& b) {
	if (a.thesign == b.thesign && strcmp(a.digits, b.digits) == 0)
		return true;
	else
		return false;
}
bool operator!=(const inf_int& a, const inf_int& b) {
	return !operator==(a, b);
}
bool operator>(const inf_int& a, const inf_int& b) {
	bool checker = false;

	if (a.thesign == b.thesign) {
		if (a.length > b.length)
			checker = true;
		else if (a.length < b.length)
			checker = false;
		else
		{
			for (int i = 0; i < a.length; i++)
			{
				if (a.digits[i] > b.digits[i])
				{
					checker = true;
					break;
				}
				else if (a.digits[i] < b.digits[i])
				{
					checker = false;
					break;
				}
			}
		}
		if (a.thesign == true)
			return checker;
		else
			return !checker;
	}
	else if (a.thesign == true)
		return true;
	else
		return false;
}
bool operator<(const inf_int& a, const inf_int& b) {
	if (operator>(a, b) || operator==(a, b))
		return false;
	else
		return true;
}

inf_int operator+(const inf_int& a, const inf_int& b) {
	inf_int a_copy = a;//절댓값의 크기 비교용
	inf_int b_copy = b;//절댓값의 크기 비교용
	inf_int c;//계산용
	int ai = 0;
	int bi = 0;

	int i;

	if (a_copy.thesign == b_copy.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		c.thesign = a.thesign;
		a_copy.thesign = true;//절댓값 크기 비교
		b_copy.thesign = true;
		if (a_copy > b_copy) {
			c.digits = new char[a.length + 2];
			c.digits[a.length + 1] = '\0';
			for (i = 0; i < a.length + 1; i++)
			{
				c.digits[i] = '0';
			}
			for (i = 0; i < a.length; i++) {
				ai = a.digits[i] - '0';
				if (i > b.length - 1)
					bi = 0;
				else
					bi = b.digits[i] - '0';
				c.adjust(ai + bi, i);

			}
			if (c.digits[a.length] == '0')
			{
				c.length = a.length;
				c.digits[a.length] = '\0';
			}
			else
				c.length = a.length + 1;
		}
		else {
			c.digits = new char[b.length + 2];
			c.digits[b.length + 1] = '\0';
			for (i = 0; i < b.length + 1; i++)
			{
				c.digits[i] = '0';
			}
			for (i = 0; i < b.length; i++) {
				bi = b.digits[i] - '0';
				if (i > a.length - 1)
					ai = 0;
				else
					ai = a.digits[i] - '0';
				c.adjust(ai + bi, i);

			}
			if (c.digits[b.length] == '0')
			{
				c.length = b.length;
				c.digits[b.length] = '\0';
			}
			else
				c.length = b.length + 1;
		}
		inf_int d = c;
		return d;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b) {
	inf_int c;//계산용
	inf_int a_copy = a;//절댓값 크기 비교용
	inf_int b_copy = b;//절댓값 크기 비교용
	if (a == b)//같은 두 수를 뺴면 0(초기화되기 않으면 0이 저장됨)
		return c;

	int i;
	int ai, bi;
	if (a.thesign == b.thesign) {
		a_copy.thesign = true;
		b_copy.thesign = true;
		if (a_copy > b_copy) {
			c.thesign = a.thesign;
			c.digits = new char[a.length + 2];
			c.digits[a.length + 1] = '\0';
			for (i = 0; i < a.length + 1; i++)
			{
				c.digits[i] = '0';
			}
			for (i = 0; i < a.length; i++) {
				ai = a.digits[i] - '0';
				if (i > b.length - 1)
					bi = 0;
				else
					bi = b.digits[i] - '0';
				c.adjust(ai - bi, i);
			}
			for (i = a.length; i > 0; i--)
			{
				if (c.digits[i] == '0')
					c.digits[i] = '\0';
				else {
					c.length = i + 1;
					break;
				}
			}
		}
		else {
			c.thesign = !a.thesign;
			c.digits = new char[b.length + 2];
			c.digits[b.length + 1] = '\0';
			for (i = 0; i < b.length + 1; i++)
			{
				c.digits[i] = '0';
			}
			for (i = 0; i < b.length; i++) {
				bi = b.digits[i] - '0';
				if (i > a.length - 1)
					ai = 0;
				else
					ai = a.digits[i] - '0';
				c.adjust(-ai + bi, i);
			}
			for (i = b.length; i > 0; i--)
			{
				if (c.digits[i] == '0')
					c.digits[i] = '\0';
				else {
					c.length = i + 1;
					break;
				}
			}
		}
		return c;
	}
	else {
		c = b;
		c.thesign = a.thesign;
		return a + c;
	}
}

inf_int operator*(const inf_int& a, const inf_int& b) {

	inf_int c;
	inf_int d;
	int ai = 0;
	int bi = 0;
	int i;
	int j;
	int ij;
	if (a == c || b == c)//0일시 무조건 0 리턴(초기화 하지 않으면 0이 저장됨)
	{
		return c;
	}

	if (a.thesign == b.thesign) { 	// 이항의 부호가 같을 경우 +
		c.thesign = true;
		d.thesign = true;
	}
	else {//부호가 다를 경우 -
		c.thesign = false;
		d.thesign = false;
	}
	c.digits = new char[a.length + b.length + 1];
	c.digits[a.length + b.length] = '\0';
	d.digits = new char[a.length + b.length + 1];
	d.digits[a.length + b.length] = '\0';
	c.length = a.length + b.length;
	d.length = c.length;

	for (i = 0; i < a.length + b.length; i++)
	{
		c.digits[i] = '0';
		d.digits[i] = '0';
	}
	for (i = 0; i < a.length; i++) {
		ai = a.digits[i] - '0';
		for (j = 0; j < b.length; j++)
		{
			bi = b.digits[j] - '0';
			c.adjust(ai * bi, i + j);
		}
		d = c + d;
		for (int k = 0; k < a.length + b.length - i; k++)
		{
			c.digits[i + k] = '0';
		}
	}
	if (d.digits[a.length + b.length - 1] == '0')
	{
		d.digits[a.length + b.length - 1] = '\0';
		d.length = a.length + b.length - 1;
	}
	else
		d.length = a.length + b.length;


	return d;
}

// inf_int operator/(const inf_int& , const inf_int&); // not required

ostream& operator<<(ostream& out, const inf_int& a)
{
	if (a.thesign == false) {
		out << '-';
	}
	for (int i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::adjust(int num, int index) {
	int number = 0;
	number += this->digits[index] - '0';
	number += num;
	if (number >= 0) {
		int divided = number / 10;
		int mod = number % 10;
		this->digits[index] = mod + '0';
		this->digits[index + 1] = divided + '0';
	}
	else
	{
		number += 10;
		this->digits[index] = number + '0';
		this->digits[index + 1] = '0' - 1;
	}
}



// friend istream& operator>>(istream& , inf_int&);    // not required
// 
// 
// 
// to be filled by students
//
// example :
//
// bool operator==(const inf_int& a , const inf_int& b)
// {
//     // we assume 0 is always positive.
//     if ( (strcmp(a.digits , b.digits)==0) && a.thesign==b.thesign )
//         return true;
//     return false;
// }
//
