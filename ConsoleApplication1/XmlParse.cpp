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
	//funct_struct funk;
	string cur_line = " ";	
	int line_size = 0;
	int fslash_count = 0;
	bool flag = true;
	do {

		getline(file_stream, cur_line);
		line_size = cur_line.size();
		//use index of slash, then start searching from there!
		if (cur_line.find("///<summary>") != string::npos)
			parseString(cur_line, XML_TAGS::SUMMARY, file_stream.tellg()); //maybe here we should 
			//funk.summary = parseString(cur_line,XML_TAGS::SUMMARY, file_stream.tellg())
		else if (cur_line.find("///<return>") != string::npos)
			parseString(cur_line, XML_TAGS::RETURN, file_stream.tellg());
			//funk.ret_str = parseString(cur_line, XML_TAGS::RETURN, file_stream.tellg());
		else if (cur_line.find("///<param name") != string::npos)
			parseString(cur_line, XML_TAGS::PARAM, file_stream.tellg());
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
	string definition = " ";
	string tempstr = " ";
	int start_index = 0;
	int end_index = 0;

	string accumulation_str = beg_line + "\n";

	do {
		getline(file_stream, definition); //do while until </summary> is found.
		definition = cleanString(definition, format);

		accumulation_str += definition + "\n";
		//cout << accumulation_str;
	} while ((definition.find(end_tag) == string::npos) && !file_stream.eof());
	
	start_index = accumulation_str.find(start_tag) + start_tag.size();
	end_index = accumulation_str.find(end_tag) - end_tag.size();
	tempstr = accumulation_str.substr(start_index, end_index);
	return tempstr;
}

void XmlParse::parseString(string line, XML_TAGS tags, int index) {

	string tempstr = " ";
	string definition = " ";
	string deft = " ";
	int start_index = 0;
	int end_index = 0;
	int def_size = 0;
	int start_get = index;
	int end_get = 0;
	int s_e_diff = 0;
	int amount_of_lines = 0;
	string startparam = " ";
	string endparam = " ";
	char c = 'a';

	funct_struct funk;

	switch (tags) {

	case (XML_TAGS::PARAM):
		startparam = "<param name = ";
		endparam = "</param>";
		//temp_sub_str = line.substr(index, )
		break;

	case (XML_TAGS::SUMMARY):
		startparam = "<summary>";
		endparam = "</summary>";
		start_get = index;
		funk.summary = accumulateComments(startparam, endparam, line, tags, index);
		cout << funk.summary << endl;

		break;

	case (XML_TAGS::RETURN):
		startparam = "<return>";
		endparam = "</return>";

		start_get = index;
		line = cleanString(line, format);
		tempstr = accumulateComments(startparam, endparam, line, tags, index);
		funk.ret_str = tempstr;
		cout << funk.ret_str << endl;
	
		break;

	}
}

XmlParse::~XmlParse()
{
}
