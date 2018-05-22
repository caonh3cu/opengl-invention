#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> // Contains all the necessery OpenGL includes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "../object/object.h"
#include "../object/camera.h"
#include "../AssetManager/AssetManager.h"
#include "../object/light.h"
#include "../massage/myWindow.h"
#include "2dScene.h"

class SpecialCamera {
public:
	SpecialCamera(
		float _nearClip = 1.0f,
		float _farClip = 500.0f,
		float _FOV = 90.0f,
		float _aspect = 1.0f,
		vec3 _pos = vec3(0),
		float _pitch = 0,
		float _yaw = 0,
		float _roll = 0)
		:realCamera(_nearClip, _farClip, _FOV, _aspect, _pos, _pitch, _yaw, _roll),
		virtualCamera(_nearClip, _farClip, _FOV, _aspect, _pos, _pitch, _yaw, _roll) { }
	void move(float front, float left) {
		realCamera.move(front, left);
		virtualCamera.move(front, left);
	}
	void rollUp(float r) {
		realCamera.rollUp(r);
		virtualCamera.rollUp(r);
	}
	void rollLeft(float r) {
		realCamera.rollLeft(r);
		virtualCamera.rollLeft(r);
	}
	void rollVirtualLeft(float r) {
		virtualCamera.rollLeft(r);
	}

	mat4 getRealViewMatrix() {
		return realCamera.getViewMatrix();
	}
	mat4 getVirtualViewMatrix() {
		return virtualCamera.getViewMatrix();
	}
	mat4 getProjectionMatrix() { return virtualCamera.projectMat; }

	Camera realCamera, virtualCamera;
};


class Scene {
public:
	Scene() {
	}
	void init();

	Mymassage::MyWindow* window;
	GLuint shader;
	SpecialCamera camera;
	Light light;
	void run();
private:
	void draw();
	bool handleInput();
	bool progressMessage();
private:
	int width, height;
	bool state[8];
};
