#include "object.h"


ObjectManager *ObjectManager::instance = NULL;

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
