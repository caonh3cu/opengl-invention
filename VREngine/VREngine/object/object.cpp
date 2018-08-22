#include "object.h"


ObjectManager *ObjectManager::instance = NULL;

void ObjectManager::readConfig(string config) {
	cout << "building object" << endl;
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
			objects[values1] = to;
			cout << "name: " << values1 << "  mesh index:" << to.meshIndex.first << " to " << to.meshIndex.second << endl;
		}
		configStream >> operation;
	}
}
void ObjectManager::drawObject(string name,GLuint shader) {
	map<string,Object>::iterator iter = objects.find(name);
	if (iter == objects.end())
		return;
	Object to = iter->second;

	vector<Mesh*> &meshes = AssetManager::getInstance()->meshes;
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &(to.getModelMat())[0][0]);
	for (int j = to.meshIndex.first; j < to.meshIndex.second; j++) {
		meshes[j]->meterial->bindMeterial(shader);
		meshes[j]->Draw();
	}
	
}
