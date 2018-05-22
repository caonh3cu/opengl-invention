#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelClass/Shader.h"
#include "ModelClass\meterial.h"
#include "ModelClass\mesh.h"

class AssetManager {
public:
	AssetManager(){}
	static AssetManager* getInstance() {
		if (instance == NULL) {
			instance = new AssetManager();
		}
		return instance;
	}
	void readConfig(string configStream);
	void loadMesh(string name, string path, bool isLoadMeterial);
private:
	void processNode(string path, aiNode* node, const aiScene* scene, bool isLoadMeterial);
	void processMesh(string path, aiMesh* mesh, const aiScene* scene, bool isLoadMeterial);
	void loadMeterial(Meterial &m);
private:
	static AssetManager* instance;
	
public:
	//网格
	vector<Mesh*> meshes;
	map<string ,pair<int, int> > meshGroups;

	//着色器表，名字-对象
	map<string,GLuint> shaderPrograms;

	//材质表,贴图唯一
	map<string,Meterial> meterials;
	map<string, Texture> textures;
};