#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
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

#include "../massage/myWindow.h"
#include "2dScene.h"

class Delo {
public:
	bool addPoint(vector<vec2> list);
	void draw();
	void clear(); 
	static bool lineIntersectSide(vec2 A, vec2 B, vec2 C, vec2 D);

	static bool intersect(vec2 A, vec2 B, vec2 C, vec2 D);
	
private:
	class halfEdge {
	public:
		halfEdge():isDraw(false){}
		bool isDraw;
		int orgin;
		int twin;
		int next;
	};
	int pointNum;
	vector<vec2> pointList;
	vector<halfEdge> edgeList;

	class Event{
	public:
		vec2 pos;
		int eIn, eOut;
		enum Type { normal, downLeft, downRight, upLeft, upRight };
		Type type;
		const bool operator<(const Event &e) const {
			if (pos.y == e.pos.y)
				return pos.x < e.pos.x;
			else
				return pos.y < e.pos.y;
		}
	};
	class bucket {
	public:
		bucket():next(NULL){}
		bucket* next;
		int edgeLeft,edgeRight;
		int helpEdge;
		bool isHelpFen;
	};
	bucket first;
	priority_queue<Event> eQueue;
	int step;
	void init();
	void nextStep();
	void destroy();
	bool isClockwise;
	void link(int i, int j);
};

class SceneDelo {
public:
	SceneDelo() {
	}
	void init();

	Mymassage::MyWindow* window;
	GLuint shader;
	void run();
private:
	void draw();
	bool handleInput();
	bool progressMessage();
private:
	int width, height;	
	enum State {
		F = 0, isComplete = 1, isInsect = 2
	};
	bool state[20];

	Delo delo;

	vector<vec2> pointList;
	vec2 mousePos;
};