#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
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
#include "../AssetManager/Loader.h"
#include "../object/light.h"
#include "../massage/myWindow.h"
#include "2dScene.h"

#include "../VRDriver/VRDriver.h"



class SpecialCamera {
public:
	SpecialCamera():trans(1.0f){ }
	void updateView(mat4 HMDview) {
		realView = HMDview;
		virtualView = realView*trans;
	}
	void reset() { trans = mat4(1.0f); }
	mat4 getRealViewMatrix() {
		return realView;
	}
	mat4 getVirtualViewMatrix() {
		return virtualView;
	}

	vec4 getRealPos() {
		return inverse(realView)[3];
	}
	vec4 getRealDir() {
		return -inverse(realView)[2];
	}
	vec4 getVirtulPos() {
		return inverse(virtualView)[3];
	}
	vec4 getVirtulDir() {
		return -inverse(virtualView)[2];
	}

	mat4 virtualView, realView;
private:
	mat4 trans;
};


class Scene {
public:
	Scene() {
		for (int i = 0; i < STATE_NUM; i++) state[i] = false;
	}
	void init();

	void run();
private:
	void draw2D();
	void draw(bool isLeftEye);
	bool handleInput();
	bool progressMessage();
private:
	int width, height;
	enum State {
		W = 0, A = 1, S = 2, D = 3, Q = 4, E = 5, P = 6,
		isOutFileOpen = 7, SPACE = 10,gameStart = 11
	};
	const static int STATE_NUM = 20;
	bool state[STATE_NUM];

	VRDriver vr;

	//¼ÇÂ¼µÄÎÄ¼þÁ÷
	ofstream os;
	void readSet(string config);
	bool hasVR;

	Mymassage::MyWindow* window;
	int frameNum;

	GLuint shader, shaderBox;
	SpecialCamera camera;
	Camera cameraNoVR;
	Light light;
};
