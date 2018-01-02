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

struct param_struct {
	string param_name;
	string param_descript;
};

struct funct_struct {
	int num_of_param;
	bool isConstant;
	long get_index;
	string ret_str; //look for type after return found. If void ...
	string summary;
	string func_name;
	string ret_type;
	string func_decl;
	vector<param_struct> param_vect;

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

	look for beginning of the block comment and find the end. mark it and ignore the lines from there when parsing.
	*/
public:

	XmlParse();


	///<summary> 
	///Generates an XmlParser object with the specified file name.
	///</summary>
	///<param name = "fn"> The file name of the source file as a string. </param>
	

	XmlParse(string fn);

	~XmlParse();

	///<summary>
	///Initializes the funct struct.
	///</summary>
	///<return> Returns an initialized funct structure.</return>

	funct_struct initFuncStruct();

	///<summary>
	///Parses the function names from the source file and creates vectors for each one.
	///The lines between the functions are then parsed.
	///</summary>
	
	void parseFuncName();
	

	///<summary>
	///Parses a source file and generates an html document.
	///</summary>
	///<return>
	///Returns true if the operation is succesful. 
	///</return>

	bool parse();

	///<summary>
	///Parses a source file and stores the xml comment's in a vector format.
	///</summary>
	///<param name = "index"> The index of the function whose comments we are parsing. </param>
	///<return> Returns true if the operation was succesful. </return>

	bool parse(int index);



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
	///Accumulates the comments between the xml start and end tag.
	///</summary>
	///<param name = "start_tag"> The format for the starting tag. </param>
	///<param name = "end_tag"> The format for the ending tag. </param>
	///<param name = "beg_line"> The beginning line contaning the starting tag. </param>
	///<param name = "cur_tag"> The current type of xml_tag we are parsing. </param>
	///<param name = "get_index"> The current position of the get pointer in the stream. </param>
	///<return> Returns the comments between the starting and ending tags. </return>
	string accumulateComments(string start_tag, string end_tag, string cur_line, XML_TAGS cur_tag, int get_index);

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
	///Removes a specified pattern from the given string.
	///Only removes patterns at the beginning of files.
	///Further support includes remove beginning or replace.
	///</summary>
	///<param name = "comm_line"> The comment line passed as a string. </param>
	///<param name  = "patt"> The string pattern which we wish to remove from the beginning of the file. </param>
	///<return> Returns the altered string. Without the slashes. </return>

	string cleanString(string cur_line, string patt);

	string removeWhiteSpace(string cur_line);


	//int indexOfSlash(string cur_line, int AMOUNT_OF_SLASH);


	

};
#endif // ! XML_PARSE_H
