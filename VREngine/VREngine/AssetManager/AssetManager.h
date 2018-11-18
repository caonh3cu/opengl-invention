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
//using namespace glm;

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
#include "Loader.h"



//AssetManager类：单例类，基于assimp的模型导入类，可以导入材质、纹理，也可以导入shader，并对材质和相应shader进行绑定以供渲染
//其中材质支持：
//specularColor, diffuseColor, ambientColor，shininess;
//specularTexture, diffuseTexture, ambientTexture, shininessTexture，
//normalTexture, heightTexture, lightmapTexture, emissiveTexture
//贴图除了漫反射和环境光都没有测试
//
//贴图有缓存保证不重复读取图片，材质也有材质名的唯一性（同样的名字只读取第一个）
//
//和材质类直接相配的两个着色器如shader / shader1
//
//另外实现了简易的根据配置文件读取模型和着色器（实际上传入的是stringstream）
//挺想写支持XML或者json格式的然而不太了解╮(╯﹏╰）╭
class AssetManager {
public:
	AssetManager(){}
	static AssetManager* getInstance() {
		if (instance == NULL) {
			instance = new AssetManager();
		}
		return instance;
	}
	//读配置信息，根据配置导入模型、材质、贴图，编译着色器
	void readConfig(MyLoader::Node* node);
	//加载模型
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
	//每个obj文件一个group
	map<string ,pair<int, int> > meshGroups;

	//着色器表，名字-对象
	map<string,Shader> shaderPrograms;

	//材质表,贴图唯一，材质名唯一
	map<string,Meterial> meterials;
	map<string, Texture> textures;
};
