/*
Future: Implementation with an SQL database!

Notes: Runtime is o(n) where n is the number of lines.

Write file: once parsig is complete ask if they wish to store it in a file. Tag eaxh line w/ identifiers to determine what each parse line is (return, summary, var name).


Tag structure:
F.'Function name'.'Function Type'.'is it constant?'.'Number of Parameters'
S.'Summary string here'
R.'Return summary string here'.'return type' 
P:'paramater_name' followed by a '.' and parameter type.

TO do: support for constructors.

Split files -> create a Parser class that contains all of the parsing functions. XmlParse only controls the parsing and input/output files.
	Clean up function that reads all of the funct_struct and removes the spaces from beginning and ///
	block comment ignorance.

	static implementation.
	Inheritance
	Make parse functions a separate cpp file?
	Look at intial declaration of start_index and end_index for the accumulate comments function and everywhere else

Changelog: Added support for pushing back summary names.
Fixed issue where the file would be opened twice and would cause every other function's comments to be skipped and not stored.
Added support for return types, function names, return summaries, parameter names and descriptions.
Added a print function that prints the contents of the function_structure.

Added support for constructors and deconstructors.
Included trimPattern and trimCharacter functions that remove specified patterns and characters from a string.
Fixed improperly formatted summary definitions.

Issues: First function's comments not stored properly.
logical issue with parseCommentBlocks function.
Return type is a problem with the function names.

Edge cases:
class names in multiple places.
block comments.
extra amount of keywords for function declarations.

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

	cout << "Class declaration: " << class_declaration << endl;
	cout << "Class name: " << class_name << endl;
}

bool XmlParse::writeToFile(string file_name) {
	ofstream write_stream;
	

	if (!write_stream.is_open()) { //File is already open, we should return false.
		
		write_stream.open(file_name,ofstream::out);
		
		if (write_stream.fail()) { //If for whatever reason the stream fails, we should return false and exit the function.
			cout << "Oops, something went wrong. Could the file be currently open?";
			return false;
		}

	} else {
		cout << "Error, the file is already open.";
			return false;
	}
	
	int vect_size = xml_vect.size();
	int amount_of_params = 0;
	function_struct temp_xml_struct; //Used as a temporary source of the xml_vect at the index i.
	//Would it be faster to copy xml_vect.at(i) to a structure and then call from there such that we aren't constantly accessing the outside scope?
	
	if (vect_size <= 0) //If the vector is empty, the function exists.
		return false;

	for (int i = 0; i < vect_size; ++i) {
		temp_xml_struct = xml_vect.at(i); //
		write_stream << "F." << temp_xml_struct.func_name << ".";
		if (temp_xml_struct.isConstructor)
			write_stream << "Constructor.";
		else
			if (temp_xml_struct.isDestructor)
				write_stream << "Destructor.";
			else
				write_stream << temp_xml_struct.ret_type << ".";

		if (temp_xml_struct.isConstant)
			write_stream << "true.";
		else
			write_stream << "false.";

		write_stream << temp_xml_struct.num_of_param << endl;
	}
	
	write_stream.close();

	return true;
	//If at this point the write_stream has no issues, continue along with the function for it would have existed otherwise.

}

string XmlParse::trimPattern(string str, string pattern) 
{
	int start_index = 0;

	string tempstr = str;

	do {

		start_index = tempstr.find(pattern, start_index);
		if (start_index != string::npos)
			tempstr.erase(start_index, pattern.size());

	} while (start_index != string::npos);

	return tempstr;
}

string XmlParse::trimCharacter(string str, char character) {
	
	string tempstr = str;
	string temppattern = "" + character;
	
	tempstr = trimPattern(str, temppattern);
	
	return tempstr;
}

void XmlParse::parseClassDecl(string str_cls) {
	int start_index = -1;
	int end_index = -1;

	start_index = str_cls.find("class");

	if (start_index != string::npos) {
		start_index = str_cls.find_first_not_of(' ', start_index + 5); // this puts the index at the start of the class name.
		end_index = str_cls.find(':');

		if (end_index == string::npos) {
			end_index = str_cls.find_first_of(' ', start_index);
			if (end_index == string::npos) {
				end_index = str_cls.find('{');
			}
		}
	}

	if (end_index != string::npos) {
		class_name = str_cls.substr(start_index, end_index - start_index);
		class_name = trimCharacter(class_name, ' ');
	}

}

void XmlParse::parseFuncName() {

	function_struct func = initFuncStruct();

	string cur_line = " ";
	string tempstr = "";
	int start_index = -1;
	int end_index = -1;

	long get_count = 0;

	if (!file_stream.is_open()) //Checks to see if the file has been opened or not.
		file_stream.open(file_name);

	do {

		

		getline(file_stream, cur_line);
		get_count = file_stream.tellg();


		if (cur_line.find("class ") != string::npos) { //The reason why I did not assign start_index to before the if statement is so that start_index is not assigned a variable every loop in conjuction with theif statement. It should slightly reduce run-time.
			start_index = cur_line.find("class") + 5;
			tempstr = cur_line;
			if (cur_line.find("{") != string::npos) {
				class_declaration = tempstr;
				parseClassDecl(tempstr);
				start_index = -1;
			}
		}

		end_index = cur_line.find("{");

		if (start_index > -1 && end_index != string::npos) {

			tempstr += cur_line;
			if (end_index > -1 && start_index > -1) {//both class and a  { exist. 
				class_declaration = tempstr;
				parseClassDecl(tempstr);
			}
			start_index = -1;
			end_index = -1;
		}

		if (cur_line.find("//", 0) == string::npos && cur_line.find('(', 0) != string::npos && cur_line.find(')', 0) != string::npos) { //found ( and ) somewhere in the string. Assume the programmer has the correct file format.
		
			func.func_decl = cur_line;
			func.get_index = get_count;

			xml_vect.push_back(func);
		}

	} while (!file_stream.eof());
	
	for (unsigned int i = 0; i < xml_vect.size(); ++i)
		parse(i);

}

function_struct XmlParse::initFuncStruct() {
	int vect_size = xml_vect.size();
	for (int i = 0; i < vect_size; ++i) {
		xml_vect.pop_back();
	}
	function_struct func;
	func.get_index = 0;
	func.func_name = "Not provided.";
	func.isConstructor = false;
	func.isDestructor = false;
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
			
		}
		parseCommentBlocks(cur_line, index);

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
	string tempstr = "Not Defined.";
	string decon_name = "~" + class_name;


	if (cur_line.find(decon_name) != string::npos) {
		tempstr = "Destructor.";
		return tempstr;
	}

	if (cur_line.find(class_name) != string::npos) {
		tempstr = "Constructor.";
		return tempstr;
	}


	if (start_index != string::npos && end_index != string::npos) {
		tempstr = cur_line.substr(start_index + 1, diff_index - 1);
	}

	return tempstr;
}

void XmlParse::parseFunctionDeclaration(int vect_index) {
	int start_index = 0;
	int end_index = 0;

	string tempstr = "";
	string ret_str = "";
	string func_decl = xml_vect.at(vect_index).func_decl;

	xml_vect.at(vect_index).ret_type = retrieveRetType(func_decl); //parses the return type.

	ret_str = xml_vect.at(vect_index).ret_type;
	
	if (ret_str.compare("Constructor.") == 0) { // Checks to see if the return string contains 

		xml_vect.at(vect_index).isConstructor = true;
		xml_vect.at(vect_index).func_name = class_name;

	} else if (ret_str.compare("Destructor.") == 0) {
		
		cout << "went to destructor??\n";
		xml_vect.at(vect_index).isDestructor = true;
		xml_vect.at(vect_index).func_name = "~" + class_name;

	} else {

		start_index = func_decl.find(xml_vect.at(vect_index).ret_type) + xml_vect.at(vect_index).ret_type.size(); //finds where the return type was in the function declaration and the index begins there.
		/*^^^ new formula needs to be made to accomdate for white space sooooo maybe we find the ret type, increment it with it's size and then find first not of white space and take the difference between the (ret_type_index + ret_type_size) - find_first_not_of(' ')
		
		*/
		tempstr = func_decl.substr(start_index, func_decl.size());

		end_index = tempstr.find("(");

		tempstr = func_decl.substr(start_index, end_index);

		xml_vect.at(vect_index).func_name = tempstr;

		if (func_decl.find("const") != string::npos) // if we find the constant keyword, tag the function as constant.
			xml_vect.at(vect_index).isConstant = true;
	}
}


string XmlParse::accumulateComments(string start_tag, string end_tag, string beg_line, int get_index) {
	
	//beg_line = cleanString(beg_line, format);
	string definition = beg_line;
	string tempstr = "";
	int start_index = 0;
	int end_index = 0;

	string accumulation_str = beg_line;

	start_index = beg_line.find(start_tag, get_index) + start_tag.size();
	end_index = beg_line.find(end_tag,start_index);

	if (end_index != string::npos) { //If the end_parameter is found, end_index will != string::npos, therefore we can substring the summary string.

		tempstr = accumulation_str.substr(start_index, end_index - start_index);
		tempstr = trimPattern(tempstr, format);
	
	} else
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


	/*
	Searches through the source file and looks for the xml tags for summary, parameters, and returns.
	The markup is parsed and stored in their respective strings.
	*/	

	if (comments.find("<summary>") != string::npos) {

		start_tag = "<summary>";
		end_tag = "</summary>";
		str_end_index = comments.find(end_tag) + end_tag.size();

		tempstr = accumulateComments(start_tag, end_tag, comments, 0);
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

					start_tag = "\">";
					end_tag = "</param>";

					str_var_end_index = comments.find("\"", str_beg_index);
					tempstr = comments.substr(str_beg_index, str_var_end_index - str_beg_index); //parses the parameter name from the markup
					
					params.param_name = tempstr;

					tempstr = accumulateComments(start_tag, end_tag, comments, str_end_index); //parses the parameter description.

					params.param_descript = tempstr;

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
		
	if (comments.find("<return>") != string::npos) {

		start_tag = "<return>";
		end_tag = "</return>";

		str_beg_index = comments.find(start_tag, str_beg_index);
		tempstr = accumulateComments(start_tag, end_tag, comments, str_beg_index);

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
