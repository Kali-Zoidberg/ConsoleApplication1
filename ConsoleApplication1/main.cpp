// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "XmlParse.h"
using namespace std;

int main()
{
	XmlParse parser;
	string file_name = " ";
	cout << "Please enter the source file. \n";
	getline(cin, file_name);
	parser.setFileName(file_name);
	parser.openFile();

	return 0;
}

