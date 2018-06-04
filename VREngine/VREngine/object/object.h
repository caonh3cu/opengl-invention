#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <hash_map>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 
#include "../AssetManager/AssetManager.h"

class Object {
public:
	Object(string _name = "") :
		name(_name),
		pos(0, 0, 0),
		scalev(1, 1, 1),
		angle(0),
		axis(0, 1, 0),
		modelMat(1.0f){	}
	string name;
	pair<int,int> meshIndex;
	void setModelMat(mat4 mat) { modelMat = mat; }
	void translate(vec3 v) { pos += v; calculatModelMat(); }
	void rotate(float _angle, vec3 _axis) { angle = _angle; axis = _axis; calculatModelMat(); }
	void scale(vec3 v) { this->scalev = vec3(this->scalev.x*v.x, this->scalev.y*v.y, this->scalev.z*v.z); calculatModelMat(); }
	mat4 getModelMat() { return modelMat; }
protected:
	vec3 pos;
	vec3 scalev;
	float angle;
	vec3 axis;
	mat4 modelMat;
	void calculatModelMat() {
		mat4 mat(1.0f);
		mat = glm::translate(mat4(1.0f), pos)*glm::rotate(mat4(1.0f), angle, axis)*glm::scale(mat4(1.0f), this->scalev)*mat;
		modelMat = mat;
	}
};

class ObjectManager {
public:
	ObjectManager() {}
	static ObjectManager* getInstance() {
		if (instance == NULL) {
			instance = new ObjectManager();
		}
		return instance;
	}
	void readConfig(string config); 
	void ObjectManager::drawObject(string name, GLuint shader);
private:
	static ObjectManager *instance;
	hash_map<string,Object> objects;
};
