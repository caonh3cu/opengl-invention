#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Loader {
public:
	void load(string fileName);
	map<string, string> stringMap;
};