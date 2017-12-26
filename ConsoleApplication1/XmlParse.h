#pragma once
#ifndef  XML_PARSE_H
#define XML_PARSE_H

#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

enum XML_TAGS
{
	SUMMARY, PARAM, RETURN
};
struct funct_struct {
	int num_of_param;
	string ret_str; //look for type after return found. If void ...
	vector<string> param_strings;
	string summary;
	string func_name;
	string ret_type;
};

class XmlParse
{
	/*

	Parsing file. Do get line until eof. Check each line and stop loop if /// is not found.
	If /// is found, look for <summary>, <param name> etc.
	Then look for  func name.

	Parallel vectors
	-make enum for types

	overload function
	Start looking for string::find


	*/
public:

	XmlParse();


	///<summary> Generates an XmlParser object with the specified file name.
	///</summary>
	///<param name = "fn"> The file name of the source file as a string. </param>

	XmlParse(string fn);

	~XmlParse();


	///<summary>
	///Parses a cpp file and generates an html document.
	///line 3 test
	///</summary>
	///<return> 
	///Returns true if the operation is succesful. 
	///</return>
	bool parse();


	///<summary>
	///Opens a file with a new name and determines whether or not it is valid.
	///</summary>
	///<param name = "fn"> The file name to open </param>
	///<return> Returns true if the operation is succesul. False otherwise. 
	///</return>

	bool openFile(string fn);


	///<summary>
	///Opens the file and determines whether or not it is valid.
	///</summary>
	///<return> Returns true if the operation is succesul. False otherwise. </return>

	bool openFile();

	///<summary>
	///Specifies a new file name.
	///</summary>
	///<return> Returns true if the name is valid. </return>


	bool setFileName(string fn);
	///<summary>
	///Parses a valid xml comment.
	///</summary>
	///<param name = "line"> Pass the xml formmated line as a string. </param>
	///<param name = "tag"> The type of xml tag </param> 
	///<param name = "index"> The beginning point in the line where the comment begins. </param>
	void parseString(string line, XML_TAGS tags, int index);

private:
	const string format = "///";
	string file_name;
	fstream file_stream;
	vector<funct_struct> xml_vect;
	int vect_index;

	///<summary>
	///Removes the Forward slashes from the comments.
	///</summary>
	///<param name = "comm_line"> The comment line passed as a string. </param>
	///<param name  = "
	///<return> Returns the altered string. Without the slashes </return>

	string cleanString(string cur_line, string patt);


	//int indexOfSlash(string cur_line, int AMOUNT_OF_SLASH);

	bool endOfTag(string cur_line, XML_TAGS tags);

	

};
#endif // ! XML_PARSE_H
