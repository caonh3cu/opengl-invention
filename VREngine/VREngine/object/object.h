#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 
#include "../AssetManager/AssetManager.h"

//object类：比较简单了就，只实现了基本功能，物体的各种变换，绑定网格
//另外顺便写了物体管理器，更简单了，只是够现在用
//
//配套的写了light类（与示例shader匹配的）和camera类，都是刚好能用的。
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
	void setModelMat(glm::mat4 mat) { modelMat = mat; }
	void translate(glm::vec3 v) { pos += v; calculatModelMat(); }
	void setPos(glm::vec3 v) { pos = v; calculatModelMat();}
	void rotate(float _angle, glm::vec3 _axis) { angle = _angle; axis = _axis; calculatModelMat(); }
	void scale(glm::vec3 v) { this->scalev = glm::vec3(this->scalev.x*v.x, this->scalev.y*v.y, this->scalev.z*v.z); calculatModelMat(); }
	glm::mat4 getModelMat() { return modelMat; }
protected:
	glm::vec3 pos;
	glm::vec3 scalev;
	float angle;
	glm::vec3 axis;
	glm::mat4 modelMat;
	void calculatModelMat() {
		glm::mat4 mat(1.0f);
		mat = glm::translate(glm::mat4(1.0f), pos)*glm::rotate(glm::mat4(1.0f), angle, axis)*glm::scale(glm::mat4(1.0f), this->scalev)*mat;
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
	void ObjectManager::drawObject(string name, GLuint shader);
	map<string,Object> objects;
private:
	static ObjectManager *instance;
};
