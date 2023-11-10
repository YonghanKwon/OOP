#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdlib.h>

using namespace std;

int main()
{
	char input[100];
	char ope;//¿¬»êÀÚ
	inf_int output;
	do {
		cout << "Input: ";
		cin.getline(input, 100, '\n');
		if (strcmp(input, "0") == 0)
			break;

		char* ptr = strtok(input, " ");
		inf_int a(ptr);
		ptr = strtok(NULL, " ");
		ope = ptr[0];
		ptr = strtok(NULL, " ");
		inf_int b(ptr);

		switch (ope) {
		case '+':
			output = a + b;
			break;
		case '-':
			output = a - b;
			break;
		case '*':
			output = a * b;
			break;
		default:
			cout << "Wrong Input.";
		}

		cout << "Output: " << output << endl;

	} while (1);

	return 0;
}
