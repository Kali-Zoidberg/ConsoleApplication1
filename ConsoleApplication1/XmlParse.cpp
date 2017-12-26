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
bool XmlParse::parse() {
	
	string cur_line = " ";	
	int line_size = 0;
	int fslash_count = 0;
	bool flag = true;
	do {

		getline(file_stream, cur_line);
		line_size = cur_line.size();
		//use index of slash, then start searching from there!
		if (cur_line.find("///<summary>") != string::npos)
			parseString(cur_line, XML_TAGS::SUMMARY, file_stream.tellg());
		else if (cur_line.find("///<return>") != string::npos)
			parseString(cur_line, XML_TAGS::RETURN, file_stream.tellg());
	/*	for (int i = 0; i < line_size; ++i) {
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
					parseString(cur_line, XML_TAGS::SUMMARY, file_stream.tellg());
					flag = false;
				}
				break;

			case 'r':
				if (flag) {
					cout << "\nyo \n";
					fslash_count = 0;

					parseString(cur_line, XML_TAGS::RETURN, i);
					flag = false;
				}
				break;

			default:
				fslash_count = 0;
				flag = false;
			
			break;
			
			}

		}*/

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
		line = cleanString(line,format);
		funk.summary = line + "\n";

		do {

			getline(file_stream, definition); //do while until </summary> is found.
			definition = cleanString(definition, format);
			++amount_of_lines;
			tempstr = definition;					  //check for 3 f_slash. 
			def_size = 3;
	
			funk.summary += definition + "\n";
			
		 } while (endOfTag(definition, tags) && !file_stream.eof());
		 
		 start_index = funk.summary.find(startparam) + startparam.size();
		 end_index = funk.summary.find(endparam) - endparam.size();
		 tempstr = funk.summary.substr(start_index, end_index);
		 funk.summary = tempstr;
		 cout <<  endl << "funk.summary: \n" << funk.summary << endl;
	
		
		//initialize new vector.
		
		//get next line??

		break;
	
	case (XML_TAGS::RETURN): 
		startparam = "<return>";
		endparam = "</return>";

		start_get = index;
		line = cleanString(line,format);
		funk.ret_str = line + "\n";
		end_get = file_stream.tellg();
		do {
			getline(file_stream, definition); //do while until </summary> is found.
			definition = cleanString(definition, format);

			++amount_of_lines;
			tempstr = definition;					  //check for 3 f_slash. 
			def_size = 3;
			//if (definition.size() >= 3) {
			//definition = cleanString(definition, 3) + "\n";
			funk.ret_str += definition + "\n";
			//}

			end_get = file_stream.tellg();
		} while (endOfTag(definition, tags) && !file_stream.eof());
		start_index = funk.ret_str.find(startparam) + startparam.size();
		end_index = funk.ret_str.find(endparam) - endparam.size();
		tempstr = funk.ret_str.substr(start_index, end_index);
		funk.ret_str = tempstr;
		cout << endl << "funk.ret_str: \n" << funk.ret_str << endl;
		do {
			getline(file_stream, definition);

		} while (definition.find_first_not_of(' ') == string::npos && !file_stream.eof());
		cout << "the line that follows the ret: " << definition;

		//initialize new vector? When should we do this? when we find the data type?
		break;

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

/*
Maybe we should return false as soon as non slash found and not consecutive. But ignore empty space.
*/
/*
int XmlParse::indexOfSlash(string cur_line, int AMOUNT_OF_SLASH) {
	int str_size = cur_line.size();
	int fslash_count = 0;

	for (int i = 0; i < str_size; ++i) {
		switch (cur_line[i]) {
			case '/':
				fslash_count++;
				if (fslash_count == AMOUNT_OF_SLASH) {
					return i + 1;
				}
				break;

			default:
				fslash_count = 0;
				break;
			}
		}
		return -1;
}
*/

bool XmlParse::endOfTag(string cur_line, XML_TAGS tags) {
	int found_index = 0;

	switch (tags) {
		case XML_TAGS::SUMMARY:
			
			found_index = cur_line.find("</summary>", 0);

			if (found_index == string::npos) 
				return true;
			else 
				return false;
			
		break;

		case XML_TAGS::PARAM:
		
		break;

		case XML_TAGS::RETURN:
			found_index = cur_line.find("</return>", 0);
			if (found_index == string::npos)
				return true;
			else
				return false;
		break;

		default:
			
			return false;

		break;
	}
}


XmlParse::~XmlParse()
{
}
