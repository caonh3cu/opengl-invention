#include "object.h"


ObjectManager *ObjectManager::instance = NULL;

void ObjectManager::readConfig(MyLoader::Node* node) {
	if (node->name.compare("model") == 0) {

		MyLoader::Node *tnode = node->getNode("name");
		if (tnode == NULL) {
			cout << "config error::line" << node->lineNum << " the <model> don't have <name>" << endl;
			return;
		}
		string name = tnode->value;

		Object to(name);
		to.meshIndex = AssetManager::getInstance()->meshGroups[name];
		objects[name] = to;
		cout << "name: " << name << "  mesh index:" << to.meshIndex.first << " to " << to.meshIndex.second << endl;
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
