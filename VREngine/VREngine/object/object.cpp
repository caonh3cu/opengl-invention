#include "object.h"


ObjectManager *ObjectManager::instance = NULL;

void ObjectManager::readConfig(string config) {
	stringstream configStream;
	configStream.str(config);
	string operation, values1, values2, values3;
	int valuei;
	configStream >> operation;
	while (operation.compare("end") != 0) {
		if (operation.compare("model") == 0) {
			configStream >> values1 >> values2 >> valuei;
			Object to(values1);
			to.meshIndex = AssetManager::getInstance()->meshGroups[values1];
			objects.push_back(to);
		}
		else if (operation.compare("shader") == 0) {
			configStream >> values1 >> valuei;
			if (valuei == 2) {
				configStream >> values2 >> values3;
			}
		}
		configStream >> operation;
	}
}
void ObjectManager::drawAllObject(GLuint shader) {
	vector<Mesh*> &meshes = AssetManager::getInstance()->meshes;
	for (int i = 0; i < objects.size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &(objects[i].getModelMat())[0][0]);
		for (int j = objects[i].meshIndex.first; j < objects[i].meshIndex.second; j++) {
			meshes[j]->meterial->bindMeterial(shader);
			meshes[j]->Draw();
		}
	}
}
