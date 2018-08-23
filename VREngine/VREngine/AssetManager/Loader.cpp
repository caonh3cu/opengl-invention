
#include "Loader.h"

void Loader::load(string fileName) {
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
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		// Read file's buffer contents into streams
		configStream << configFile.rdbuf();
		// close file handlers
		configFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	string operation, values1, values2, values3;
	
	string modelString = "", shaderString = "",setString = "";
	int modC = 0, shaderC = 0, setC = 0;

	while (configStream >> operation) {
		if (operation.substr(0, 2).compare("//") == 0) {
			getline(configStream, operation);
		}
		else if (operation.compare("model") == 0) {
			modC++;
			configStream >> values1 >> values2 >> values3;
			modelString += "model " + values1 + " " + values2 + " " + values3 + "\n";
		}
		else if (operation.compare("shader") == 0) {
			shaderC++;
			configStream >> values1 >> values2;
			shaderString += "shader " + values1 + " " + values2;
			if (values2.compare("2") == 0) {
				configStream >> values2 >> values3;
				shaderString += " " + values2 + " " + values3 + "\n";
			}
			else {
				cout << "not understand operation " << values2 << endl;
			}
		}
		else if (operation.compare("set") == 0) {
			setC++;
			configStream >> values1 >> values2;
			setString += "set " + values1 + " " + values2 + "\n";
		}
		else {
			cout << "not understand operation" << operation << endl;
		}
	}

	stringMap["model"] = modelString;
	stringMap["shader"] = shaderString;
	stringMap["set"] = setString;
	cout << "model:" << modC << " shader:" << shaderC << " set:" << setC << endl;
}