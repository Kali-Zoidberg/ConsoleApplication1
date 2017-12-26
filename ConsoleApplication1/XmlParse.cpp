#include "stdafx.h"
#include "XmlParse.h"



XmlParse::XmlParse()
{
	file_name = ""; 
}
XmlParse::XmlParse(string fn) {
	file_name = fn;
}
/*
find all /// first. and use that as data set. Longer runtime though but easier to parse.
change the way stuff is stored. when <summary found. summarysytring = from <summary> to </summary> if /// found. remove it.
*/
bool XmlParse::parse() {
	
	string cur_line = " ";	
	int line_size = 0;
	int fslash_count = 0;
	bool flag = true;
	do {

		getline(file_stream, cur_line);
		line_size = cur_line.size();
		//use index of slash, then start searching from there!
		for (int i = 0; i < line_size; ++i) {
			switch (cur_line[i]) {
			case '/':
				
				fslash_count++; //we need a correct switch thingy
				if (flag) {
					flag = false;
					fslash_count = 0;
				}
				break;
			case '<':
				if (fslash_count >= 3) {
					fslash_count = 0;
					flag = true;
				}
				break;
			case 'p':
				if (flag) {
					fslash_count = 0;
					parseString(cur_line, XML_TAGS::PARAM, i); // we should probably remove the slashes
					flag = false;
				}
				break;

			case 's':
				if (flag) {
					fslash_count = 0;
					parseString(cur_line, XML_TAGS::SUMMARY, i);
					flag = false;
				}
				break;

			case 'r':
				if (flag) {
					fslash_count = 0;

					parseString(cur_line, XML_TAGS::RETURN, i);
					flag = false;
				}
				break;

			default:
				fslash_count = 0;
				flag = false;
			
			break;
				/*
				Begin search for the </ maybe look for </p or </s etc. then sub string.
				*/
			}

		}

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
/*
change do while condition so that if </summary> isn't found and < is found is to just skip that summary
fix do while. maybe we need just while.
*/
void XmlParse::parseString(string line, XML_TAGS tags, int index) {

	string tempstr = " ";
	string definition = " ";
	int def_size = 0;
	funct_struct funk;
	
	switch (tags) {

	case (XML_TAGS::PARAM):
		//temp_sub_str = line.substr(index, )
		break;

	case (XML_TAGS::SUMMARY):
		do {
			getline(file_stream, definition); //do while until </summary> is found.
			tempstr = definition;					  //check for 3 f_slash. 
			def_size = 3;												
			if (definition.size() >= 3) {
				definition = cleanString(definition, 3) + "\n";
				funk.summary += definition;
			}

			cout << funk.summary << endl;
		} while (!endOfTag(definition, tags) && !file_stream.eof()); 
		//initialize new vector.
		
		//get next line??

		break;

	case (XML_TAGS::RETURN): 
		do {
			getline(file_stream, definition); //do while until </return> is found.
			tempstr = definition;					  //check for 3 f_slash. 
			def_size = 3;
			if (definition.size() >= 3) {
				definition = cleanString(definition, 3) + "\n";
				funk.ret_str += definition;
			}

			cout << funk.ret_str << endl;
		} while (!endOfTag(definition, tags) && !file_stream.eof());
		
		//initialize new vector? When should we do this? when we find the data type?
		break;

	}
}

string XmlParse::cleanString(string cur_line, int AMOUNT_OF_SLASH) {
	int str_size = cur_line.size();
	int slash_index = indexOfSlash(cur_line, AMOUNT_OF_SLASH);
	string temp = " ";
	
	if (slash_index > -1) {
		temp = cur_line.substr(slash_index + 1, str_size - slash_index);
		return temp;
	} else {
		return cur_line;
	}
}

/*
Maybe we should return false as soon as non slash found and not consecutive. But ignore empty space.
*/
int XmlParse::indexOfSlash(string cur_line, int AMOUNT_OF_SLASH) {
	int str_size = cur_line.size();
	int fslash_count = 0;

	for (int i = 0; i < str_size; ++i) {
		switch (cur_line[i]) {
			case '/':
				fslash_count++;
				if (fslash_count == AMOUNT_OF_SLASH) {
					return i;
				}
				break;

			default:
				fslash_count = 0;
				break;
			}
		}
		return -1;
}

bool XmlParse::endOfTag(string cur_line, XML_TAGS tags) {
	int found_index = 0;
	switch (tags) {
		case XML_TAGS::SUMMARY:
			found_index = cur_line.find("</summary>", 0);
			if (found_index != string::npos)
				return true;
			break;
		case XML_TAGS::PARAM:
			break;
		case XML_TAGS::RETURN:
			found_index = cur_line.find("</return>", 0);
			if (found_index != string::npos)
				return true;
			break;
		default:
			return false;
			break;
	}
}


XmlParse::~XmlParse()
{
}
