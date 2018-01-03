/*

Notes: Runtime is o(n) where n is the number of lines.

TO do: support for constructors.
	Remove the /// from strings.
	Tagging from function declaration. Need to get content from between the () to determine # of parameters and their types.
	block comment ignorance.

Changelog: Added support for pushing back summary names.
Fixed issue where the file would be opened twice and would cause every other function's comments to be skipped and not stored.
Added support for return types, function names, return summaries, parameter names and descriptions.
Added a print function that prints the contents of the function_structure.

Issues: First function's comments not stored properly.
logical issue with parseCommentBlocks function.
Return type is a problem with the function names.

Parameter summary for multiple parameters.

Paramater Description:

///<param name  = "patt"> The string pattern which we wish to remove from the beginning of the file.
*/

#include "stdafx.h"
#include "XmlParse.h"



XmlParse::XmlParse()
{
	vect_index = 0;
	file_name = ""; 
}
XmlParse::XmlParse(string fn) {
	vect_index = 0;
	file_name = fn;

}

void XmlParse::print() {
	
	int vect_size = xml_vect.size();
	int amount_of_param = 0;
	for (int i = 0; i < vect_size; ++i) {
		cout << endl;
		cout << "Function Declaration: " << xml_vect.at(i).func_decl << endl;
		cout << "Function Name: " << xml_vect.at(i).func_name << endl << endl;
		cout << "Summary: " << xml_vect.at(i).summary << endl << endl;
		amount_of_param = xml_vect.at(i).num_of_param;

		cout << "Number of Parameters: " << amount_of_param << endl;

		for (int j = 0; j < amount_of_param; ++j) {
			cout << "Paramater Name: " << xml_vect.at(i).param_vect.at(j).param_name << endl;
			cout << "Paramater Description: " << xml_vect.at(i).param_vect.at(j).param_descript << endl << endl;
		}

		cout << "Constant: ";

		if (xml_vect.at(i).isConstant)
			cout << "True. \n";
		else
			cout << "False. \n";
		
		cout << "Return Type: " << xml_vect.at(i).ret_type << endl;
		cout << "Return Summary: " << xml_vect.at(i).ret_str << endl;

	}
}


void XmlParse::parseFuncName() {

	function_struct func = initFuncStruct();

	string cur_line = " ";
	long get_count = 0;

	if (!file_stream.is_open()) //file hasn't been opened yet.
		file_stream.open(file_name);

	do {

		getline(file_stream, cur_line);
		get_count = file_stream.tellg();

		if (cur_line.find("//", 0) == string::npos && cur_line.find('(', 0) != string::npos && cur_line.find(')', 0) != string::npos) { //found ( and ) somewhere in the string. Assume the programmer has the correct file format.
		
			func.func_decl = cur_line;
			func.get_index = get_count;

			xml_vect.push_back(func);
		}

	} while (!file_stream.eof());
	
	//for (unsigned int i = 0; i < xml_vect.size(); ++i) 
	//	cout <<"get pointer: " << xml_vect.at(i).get_index << xml_vect.at(i).func_decl << endl;
	
	for (unsigned int i = 0; i < xml_vect.size(); ++i)
		parse(i);
//	cout << "size of xml_vect: " << xml_vect.size() << endl;

	//print();

	//file_stream.close();
}

function_struct XmlParse::initFuncStruct() {

	function_struct func;
	func.get_index = 0;
	func.func_name = "Not provided.";
	func.isConstant = false;
	func.num_of_param = 0;
	func.ret_str = "Not provided.";
	func.ret_type = "Constructor";
	func.func_decl = "Not provided.";
	func.summary = "Not provided.";

	return func;
}

bool XmlParse::parse(int index) {
	
	int size_of_vect = xml_vect.size();
	long beg_get_index = 0;
	long end_get_index = 0;
	long cur_get_index = 0;
	char c = ' ';
	string cur_line = " ";

	if (!file_stream.is_open()) {//file hasn't been opened yet.
		file_stream.open(file_name);
	}

	if (file_stream.fail()) {
	//	cout << "we are in error state in parse \n";
	}	

	if (index >= size_of_vect)
		return false;
	
	
		if (index == 0)
			beg_get_index = 0;
		else 
			beg_get_index = xml_vect.at(index - 1).get_index;

		end_get_index = xml_vect.at(index).get_index - xml_vect.at(index).func_decl.size() - 1;
		file_stream.seekg(beg_get_index); //put's the get and put pointers at the starting point of the function declaration.
		cur_get_index = beg_get_index;

	//	cout << "func: " << xml_vect.at(index).func_decl << endl;
			
		while (cur_get_index < end_get_index && !file_stream.eof() && !file_stream.fail()) { //Loops as long as we do not reach the next function declaration or have not reached the end of the file.
				
			file_stream.get(c);
			cur_line += c;
				
			cur_get_index = file_stream.tellg();
			if (cur_get_index == -1) {
				cout << "Didn't work. \n";
			} 

		}
		parseCommentBlocks(cur_line, index);
			//cout << cur_line << endl;

	file_stream.close();

	return true;
}

bool XmlParse::openFile() {

	file_stream.open(file_name, fstream::in);

	if (file_stream.fail()) {
		cout << "Error openning file. \n";
		return false;
	} else {

		parseFuncName();
		file_stream.close();

		return true;
	}
}

bool XmlParse::openFile(string fn) {

	file_name = fn;
	file_stream.open(file_name, fstream::in);
	
	if (file_stream.fail()) {
		cout << "Error oppening file. \n";
		return false;
	} else {
		parseFuncName();

		file_stream.close();

		return true;
	}
}

bool XmlParse::setFileName(string fn) {

	file_name = fn;
	
	file_stream.open(file_name, fstream::in);
	
	if (file_stream.fail())
		return false;
	else {
		file_stream.close();
		return true;
	}
}


string XmlParse::cleanString(string cur_line, string patt) { //new param: string patt
	string temp = " ";
	
	int str_size = cur_line.size();
	int index = 0;
	int patt_size = patt.size();
	
	index = cur_line.find(patt) + patt_size;

	if (index > patt_size - 1) {
		temp = cur_line.substr(index, str_size - patt_size);
		return temp;
	}
	else
		return cur_line;
}

string XmlParse::retrieveRetType(string cur_line) {

	int start_index = cur_line.find_first_not_of(' ');
	int end_index = cur_line.find(' ', start_index);
	int diff_index = end_index - start_index;
	string tempstr = "";

	if (start_index != string::npos && end_index != string::npos) {
		tempstr = cur_line.substr(start_index + 1, diff_index);
	}

	return tempstr;
}

void XmlParse::parseFunctionDeclaration(int vect_index) {
	int start_index = 0;
	int end_index = 0;

	string tempstr = "";
	string func_decl = xml_vect.at(vect_index).func_decl;

	xml_vect.at(vect_index).ret_type = retrieveRetType(func_decl); //parses the return type.

	start_index = func_decl.find(xml_vect.at(vect_index).ret_type) + xml_vect.at(vect_index).ret_type.size();
	
	tempstr = func_decl.substr(start_index, func_decl.size());
	
	end_index = tempstr.find("(");

	tempstr = func_decl.substr(start_index, end_index);

	xml_vect.at(vect_index).func_name = tempstr;

	if (func_decl.find("const") != string::npos)
		xml_vect.at(vect_index).isConstant = true;
	
}

string XmlParse::accumulateComments(string start_tag, string end_tag, string beg_line, XML_TAGS tags, int get_index) {
	
	//beg_line = cleanString(beg_line, format);
	string definition = beg_line;
	string tempstr = "";
	int start_index = 0;
	int end_index = 0;

	string accumulation_str = beg_line;

	start_index = beg_line.find(start_tag, get_index) + start_tag.size();
	end_index = beg_line.find(end_tag,start_index);
	if (end_index != string::npos)
		tempstr = accumulation_str.substr(start_index, end_index - start_index);
	else
		tempstr = "Not defined.";
	return tempstr;
}

void XmlParse::parseCommentBlocks(string comments, int vect_index) {

	string tempstr = "";

	string start_tag = "";
	string end_tag = "";


	int str_beg_index = 0;
	int str_end_index = 0;
	int str_var_end_index = 0;


	function_struct funcs;
	param_struct params;

	//what if summary doesn't come first? The speed of the algorithm will need to be changed.
	if (comments.find("<summary>") != string::npos) {

		start_tag = "<summary>";
		end_tag = "</summary>";
		str_end_index = comments.find(end_tag) + end_tag.size();

		tempstr = accumulateComments(start_tag, end_tag, comments, XML_TAGS::SUMMARY, 0);
		xml_vect.at(vect_index).summary = tempstr;
	}

	do {
		str_beg_index = comments.find("<param name", str_beg_index);

		if (str_beg_index != string::npos) {
			str_beg_index = comments.find("=", str_beg_index);

			if (str_beg_index != string::npos) {
				str_beg_index = comments.find("\"", str_beg_index);

				if (str_beg_index != string::npos) {
					++str_beg_index;

					start_tag = ">";
					end_tag = "</param>";

					str_var_end_index = comments.find("\"", str_beg_index);
					tempstr = comments.substr(str_beg_index, str_var_end_index - str_beg_index);

					params.param_name = tempstr;

					tempstr = accumulateComments(start_tag, end_tag, comments, XML_TAGS::PARAM, str_end_index);

					params.param_descript = tempstr;

					//cout << "var desc: " << tempstr << endl;

					xml_vect.at(vect_index).param_vect.push_back(params);
					++xml_vect.at(vect_index).num_of_param;

					str_end_index = comments.find(end_tag, str_var_end_index);

				}
			}
		}
	} while (str_beg_index != string::npos);

	if (str_beg_index < 0) //Ensures that we are reading from a valid position in the string.
		str_beg_index = 0;
	else
		str_beg_index = str_end_index; //by changing the index to the last character read, we save some processing time so chracters are not searched again.

	if (comments.find("<return>", str_beg_index) != string::npos) {
		start_tag = "<return>";
		end_tag = "</return>";

		str_beg_index = comments.find(start_tag, str_beg_index);
		tempstr = accumulateComments(start_tag, end_tag, comments, XML_TAGS::RETURN, str_beg_index);

	//	cout << "return: " << tempstr << endl;


		xml_vect.at(vect_index).ret_str = tempstr;
	}

	parseFunctionDeclaration(vect_index);

}

vector<function_struct> XmlParse::getFuncStruct() {
	return xml_vect;
}

XmlParse::~XmlParse()
{
}
