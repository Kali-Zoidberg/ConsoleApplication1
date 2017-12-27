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
/*
find all /// first. and use that as data set. Longer runtime though but easier to parse.
change the way stuff is stored. when <summary found. summarysytring = from <summary> to </summary> if /// found. remove it.
so do a dowhile and keep adding each line together until you find </summary> then truncate? no when it's found exit loop.
*/
bool XmlParse::parse() { //this must handle the vector. Idk how but it should. Maybe 
	funct_struct funk;
	string cur_line = " ";	
	int line_size = 0;
	int fslash_count = 0;
	bool flag = true;
	do {

		getline(file_stream, cur_line);
	
		line_size = cur_line.size();
		//use index of slash, then start searching from there!

		if (cur_line.find("///<summary>") != string::npos) {

			parseString(cur_line, XML_TAGS::SUMMARY, file_stream.tellg()); //maybe here we should 

		}
			//funk.summary = parseString(cur_line,XML_TAGS::SUMMARY, file_stream.tellg())
		 if (cur_line.find("///<return>") != string::npos)
			parseString(cur_line, XML_TAGS::RETURN, file_stream.tellg());
		//funk.ret_str = parseString(cur_line, XML_TAGS::RETURN, file_stream.tellg());
		 if (cur_line.find("///<param name") != string::npos)
			parseString(cur_line, XML_TAGS::PARAM, file_stream.tellg());
		// if (cur_line.find("///") == string::npos) { //and have not found tag?
			 //  now we can push back the vector. 
			 //push back
			 //initialize all components to 0
		//}
			//funk.param_str = parseString
		/*if (last_num_of_sums < cur_num_of_sums) {
			xml_vect.push_back(funk);
			
			}
		*/
	} while (!file_stream.eof());
	
	file_stream.close();
	
	return true;
}

bool XmlParse::openFile() {

	file_stream.open(file_name, fstream::in);

	if (file_stream.fail()) {
		cout << "Error openning file. \n";
		return false;
	} else {

		parse();

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

		parse();

		return true;
	}

}

bool XmlParse::setFileName(string fn) {

	file_name = fn;
	return true;

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


string XmlParse::accumulateComments(string start_tag, string end_tag, string beg_line, XML_TAGS tags, int get_index) {
	
	beg_line = cleanString(beg_line, format);
	string definition = beg_line;
	string tempstr = "";
	int start_index = 0;
	int end_index = 0;

	string accumulation_str = beg_line + "\n";

	while ((definition.find(end_tag) == string::npos) && !file_stream.eof()) {
		getline(file_stream, definition); //do while until </summary> is found.
		definition = cleanString(definition, format);

		accumulation_str += definition + "\n";

	} 
	start_index = accumulation_str.find(start_tag) + start_tag.size();
	end_index = accumulation_str.find(end_tag,start_index);

	tempstr = accumulation_str.substr(start_index, end_index - start_index);

	return tempstr;
}
string XmlParse::removeWhiteSpace(string cur_line) {

	int start_index = cur_line.find_first_not_of(' ');
	int end_index = cur_line.find(' ', start_index);
	int diff_index = end_index - start_index;
	string tempstr = "";

	if (start_index != string::npos && end_index != string::npos) {
		tempstr = cur_line.substr(start_index + 1, diff_index);
	}

	return tempstr;

}


void XmlParse::parseString(string line, XML_TAGS tags, int index) {
	string func_decl = "";
	string definition = "";
	string var_name = "";
	string start_tag = "";
	string end_tag = "";
	string tempstr = "";
	int start_index = 0;
	int end_index = 0;
	int diff_index = 0;
	

	funct_struct funk;
	param_struct punk;

	funk.isConstant = false;
	switch (tags) {

	case (XML_TAGS::PARAM):
		start_tag = "\">";
		end_tag = "</param>";

		start_index = line.find("\"", 0) + 1;
		end_index = line.find("\"", start_index);

		if (start_index > -1 && end_index > -1) {
			
			diff_index = end_index - start_index;
			var_name = line.substr(start_index, diff_index);

			punk.param_name = var_name;
			cout << "var name: " << var_name << endl;
		}

		punk.param_descript = accumulateComments(start_tag, end_tag, line, tags, index);

		funk.param_vect.push_back(punk);

		funk.num_of_param++;
		cout << "desc: " << punk.param_descript << endl;

		break;

	case (XML_TAGS::SUMMARY):
		start_tag = "<summary>";
		end_tag = "</summary>";

		funk.summary = accumulateComments(start_tag, end_tag, line, tags, index);
		cout << funk.summary << endl;

		break;

	case (XML_TAGS::RETURN):
		start_tag = "<return>";
		end_tag = "</return>";

		line = cleanString(line, format);
		funk.ret_str = accumulateComments(start_tag, end_tag, line, tags, index);
		cout << funk.ret_str << endl;

		do {
			getline(file_stream, func_decl);

		} while (func_decl.find_first_not_of(' ') == string::npos && !file_stream.eof());
		
		funk.ret_type = removeWhiteSpace(func_decl); //parses the return type.

		start_index = func_decl.find_first_not_of(' ', funk.ret_type.size()); 
		tempstr = func_decl.substr(start_index, func_decl.size());
		end_index = tempstr.find("(");

		tempstr = func_decl.substr(start_index, end_index);
		funk.func_name = tempstr;
		cout << endl<< tempstr << endl;
		cout  << funk.ret_type;

		if (func_decl.find("const") != string::npos)
			funk.isConstant = true;
		//  bool  fun
		// get index of last white space, substring it until the next white space is found. then remove all whitespaces for func name.

		break;

	}
}

XmlParse::~XmlParse()
{
}
