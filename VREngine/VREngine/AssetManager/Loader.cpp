
#include "Loader.h"


void MyLoader::Loader::load(string fileName) {
	cout << "loading config : " << fileName << endl;
	std::ifstream configFile;
	std::stringstream configStream;
	// ensures ifstream objects can throw exceptions:
	configFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		configFile.open(fileName);;
		if (!configFile.is_open()) {
			std::cout << "ERROR::CONFIG::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		// Read file's buffer contents into streams
		configStream << configFile.rdbuf();
		// close file handlers
		configFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::CONFIG::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	char c;
	int lineNum = 1;
	int charNum = 0;
	vector<Node *> stack;
	Node* tnode = new Node;
	int state = 7;
	string strs("");
	bool isError = false;
	while ((c = configStream.get())!=-1) {
		charNum++;
		if (isError)
			break;
		if (c == '\n') {
			lineNum++;
			charNum = 0;
			if (state != 7)
				continue;
		}
		else if ((c == ' ' || c == '\t') && state != 7)
			continue;
		switch (state){
		case 0: {
			if (c == '<') {
				state = 1;
			}
			else if (c == '/' || c == '>') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				strs.clear();
				strs.push_back(c);
				state = 7;
			}
			break;
		}
		case 1: {
			if (c == '/') {
				state = 3;
			}
			else if (c == '<' || c == '>') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				state = 2;
				strs.clear();
				strs.push_back(c);
			}
			break;
		}
		case 2: {
			if (c == '>') {
				stack.push_back(tnode);
				Node* ttnode = new Node;
				ttnode->lineNum = lineNum;
				ttnode->name = strs;
				tnode->child.push_back(ttnode);
				tnode = ttnode;
				state = 0;
				strs.clear();
			}
			else if (c == '/') {
				Node* ttnode = new Node;
				ttnode->lineNum = lineNum;
				ttnode->name = strs;
				tnode->child.push_back(ttnode);
				state = 5;
				strs.clear();
			}
			if (c == '<') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				strs.push_back(c);
			}
			break;
		}
		case 3: {
			if (c == '<' || c == '>' || c == '/') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				state = 4;
				strs.clear();
				strs.push_back(c);
			}
			break;
		}
		case 4: {
			if (c == '>') {
				if (strs.compare(tnode->name) == 0) {
					tnode = stack.back();
					stack.pop_back();
					state = 0;
				}
				else {
					cout << "Error:line " << lineNum << " charactor " << charNum << " expect </" << tnode->name << "> not </"<<strs<<'>' << endl;
					state = 6;
				}
				strs.clear();
			}
			if (c == '<' || c == '/') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				strs.push_back(c);
			}
			break;
		}
		case 5: {
			if (c == '>') {
				state = 0;
			}
			else {
				cout << "Error:line " << lineNum << " charactor " << charNum << " need \'>\'" << endl;
				state = 6;
			}
			break;
		}
		case 7: {
			if (c == '<') {
				//sss
				while ((!strs.empty())&&strs.back() == ' '&&strs.back() == '\n'&&strs.back() == '\t')strs.pop_back();
				tnode->value = strs;
				strs.clear();
				state = 1;
			}
			else if (c == '\\') {
				state = 8;
			}
			else if (c == '>') {
				cout << "Error:line " << lineNum << " charactor " << charNum << " unexpected \'" << c << "\'" << endl;
				state = 6;
			}
			else {
				strs.push_back(c);
				state = 7;
			}
			break;
		}
		case 8: {
			{
				strs.push_back(c);
				state = 7;
			}
			isError = true;
			break;
		}
		case 6: {
			isError = true;
			break;
		}
		default: {
			isError = true;
		}
				 break;
		}
	};
	if (state != 0 && state != 7 && state != 8) {
		cout << "file not complete" << endl;
	}
	if (!stack.empty()) {
		cout << "need </" << tnode->name << ">" << endl;
	}
	this->root = *tnode;
	delete tnode;
}