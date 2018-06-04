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
			to.scale(vec3(0.3, 0.3, 0.3));
			to.meshIndex = AssetManager::getInstance()->meshGroups[values1];
			objects[values1] = to;
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
void ObjectManager::drawObject(string name,GLuint shader) {
	vector<Mesh*> &meshes = AssetManager::getInstance()->meshes;
	Object to = objects[name];
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &(to.getModelMat())[0][0]);
	for (int j = to.meshIndex.first; j < to.meshIndex.second; j++) {
		meshes[j]->meterial->bindMeterial(shader);
		meshes[j]->Draw();
	}
	
}
