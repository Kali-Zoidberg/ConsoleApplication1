#pragma once
#ifndef XML_PARSE_H
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

struct function_struct {
	string func_decl;
	string func_name;
	string summary;
	int num_of_param;
	vector<param_struct> param_vect;
	bool isConstant;
	string ret_type;
	string ret_str; //look for type after return found. If void ...
	long get_index;

};

class XmlParse
{
	/*

	Parsing file. Do get line until eof. Check each line and stop loop if /// is not found.
	If /// is found, look for <summary>, <param name> etc.
	Then look for  func name.

	

	overload function
	Start looking for string::find

	look for beginning of the block comment and find the end. mark it and ignore the lines from there when parsing.
	*/
public:

	///<summary>
	///hello!
	///</summary>
	XmlParse();


	///<summary> 
	///Generates an XmlParser object with the specified file name.
	///</summary>
	///<param name = "fn"> The file name of the source file as a string. </param>
	

	XmlParse(string fn);

	~XmlParse();

	///<summary>
	///Prints all data members of the function_structure vector.
	///</summary>
	
	void print();

	///<summary>
	///Parses a source file and generates an html document.
	///</summary>
	///<return>
	///Returns true if the operation is succesful. 
	///</return>


	bool parse(int index);

	///<summary>
	///Parses the function names from the source file and creates vectors for each one.
	///The lines between the functions are then parsed.
	///</summary>
	
	void parseFuncName();


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
	///Returns a copy of the function_structure vector that was parsed from the source file.
	///</summary>
	///<return> Returns a copy of the function_structure vector that was parsed from the source file.
	///</return>
	vector<function_struct> getFuncStruct();

private:
	vector<function_struct> xml_vect;
	const string format = "///";
	string file_name;
	ifstream file_stream;
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

	///<summary>
	///Initializes the funct struct.
	///</summary>
	///<return> Returns an initialized funct structure.</return>

	function_struct initFuncStruct();

	
	
	///<summary>
	///Retrieves the return type from the function declaration.
	///</summary>
	///<param name = "cur_line"> The function declaration as a string. </param>
	///<return>
	///Returns the return type as a string.
	///</return>
	
	string retrieveRetType(string cur_line);

	///<summary>
	///Parses the function declaration of the vector and modifies the vector specified by vect_index.
	///</summary>
	///<param name = "vect_index"> The vector whose function declaration we wish to parse. </param>
	void parseFunctionDeclaration(int vect_index);


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
	///Parses all the comments belonging to the function at vect_index in the xml_vect vector.
	///</summary>
	///<param name = "comments"> The XML comments belonging to the function. </param>
	///<param name = "vect_index"> The xml_vect index at which the function exists in the vector. </param>


	void parseCommentBlocks(string comments, int vect_index);

	
};
#endif // ! XML_PARSE_H


