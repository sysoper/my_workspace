// Testing_Console_Params.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;
enum InputCase {DefaultPath = 1, DefaultInput=2, DefaultOutput=3, IOPathsGiven=4,HelpParam=5,WrongParams=6};

int main(int argc, char *argv[])
{

	ifstream file;

	file.open(argv[1]);
	int a;
	file >> a;
	file.ignore(2);
	int b;
	file >> b;
	int c;
	file.ignore(1);
	file >> c;

	cout << a << endl;
	cout << b << endl;
	cout << c;
	
	file.close();

    return 0;
}

