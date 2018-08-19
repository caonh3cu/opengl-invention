#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

class OlaCamera {
public:
	OlaCamera(
		float _nearClip = 0.3f, 
		float _farClip = 500.0f,
		float _FOV = 45.0f,
		float _aspect = 1.0f,
		glm::vec3 _pos = glm::vec3(0),
		float _pitch = 0, 
		float _yaw = 0, 
		float _roll = 0)
	: nearClip(_nearClip), 
		farClip(_farClip), 
		FOV(_FOV), 
		aspect(_aspect),
		pos(_pos),
		pitch(_pitch),
		yaw(_yaw),
		roll(_roll){}

	void move(float front, float left) {
		glm::vec4 dir = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * (glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1, 0, 0))*dir);
		glm::vec4 up = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(dir))*up;
		glm::vec3 l = cross(glm::vec3(up), glm::vec3(dir));
		dir = normalize(dir);
		l = normalize(l);
		pos -= glm::vec3(dir)*front+l*left;
	}
	void addPitch(float d) {
		pitch -= d;
		if (pitch > glm::radians(90.0f))pitch = 90;
		if (pitch < glm::radians(-90.0f))pitch = -90;
	}
	void addYaw(float d) {
		yaw -= d;
		if (pitch > glm::radians(360.0f))pitch -= 360;
		if (pitch < glm::radians(-360.0f))pitch += 360;
	}
	void addRoll(float d) {
		roll -= d;
		if (pitch > glm::radians(360.0f))pitch -= 360;
		if (pitch < glm::radians(-360.0f))pitch += 360;
	}
	glm::mat4 getViewMatrix() {
		glm::vec4 dir(0,0,1,0), up(0,1,0,0);
		dir = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * (glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1, 0, 0))*dir);
		up = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(dir))*up;
		return glm::lookAt(pos,pos+ glm::vec3(dir), glm::vec3(up));
	}
	glm::mat4 getProjectionMatrix() { return glm::perspective(glm::radians(FOV), aspect, nearClip, farClip); }
	glm::vec3 pos;
	float pitch;
	float yaw;
	float roll;

	float nearClip, farClip, FOV, aspect;
};

class Camera{
public:
	Camera(
		float _nearClip = 1.0f,
		float _farClip = 500.0f,
		float _FOV = 90.0f,
		float _aspect = 1.0f,
		glm::vec3 _pos = glm::vec3(0),
		float pitch = 0,
		float yaw = 0,
		float roll = 0)
		:nearClip(_nearClip),
		farClip(_farClip),
		FOV(_FOV),
		aspect(_aspect),
		pos(_pos)
	{
		glm::vec4 d(0, 0, 1, 0), u(0, 1, 0, 0);
		d = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * (glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1, 0, 0))*d);
		u = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(d))*u;
		glm::vec3 s = cross(glm::vec3(d), glm::vec3(u));
		up = cross(s, glm::vec3(d));
		dir = glm::vec3(d);
		up = glm::normalize(up);
		dir = glm::normalize(dir);
		projectMat = glm::perspective(glm::radians(FOV), aspect, nearClip, farClip);
	}
public:
	glm::mat4 projectMat;
	glm::vec3 dir,up;
	glm::vec3 pos;

	float nearClip, farClip, FOV, aspect;

public:
	void move(float front, float left) {
		pos += front*dir + cross(up, dir)*left;
	}
	void rollUp(float r) {
		glm::vec4 d(dir, 0),u(up,0);
		d = glm::rotate(glm::mat4(1.0f), r, cross(dir, up))*d;
		u = glm::rotate(glm::mat4(1.0f), r, cross(dir, up))*u;
		dir = glm::normalize(glm::vec3(d));
		up = glm::normalize(glm::vec3(u));
	}
	void rollLeft(float r) {
		glm::vec4 d(dir, 0), u(up, 0);
		d = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0, 1, 0))*d;
		u = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0, 1, 0))*u;
		dir = glm::normalize(glm::vec3(d));
		up = glm::normalize(glm::vec3(u));
	}
	void rollLeftFromPoint(float r, glm::vec3 Pos) {
		glm::vec4 d(dir, 0), u(up, 0),p(pos,1);
		d = glm::translate(glm::mat4(1.0f), Pos)*glm::rotate(glm::mat4(1.0f), r, glm::vec3(0, 1, 0))*glm::translate(glm::mat4(1.0f), -Pos)*d;
		u = glm::translate(glm::mat4(1.0f), Pos)*glm::rotate(glm::mat4(1.0f), r, glm::vec3(0, 1, 0))*glm::translate(glm::mat4(1.0f), -Pos)*u;
		p = glm::translate(glm::mat4(1.0f), Pos)*glm::rotate(glm::mat4(1.0f), r, glm::vec3(0, 1, 0))*glm::translate(glm::mat4(1.0f), -Pos)*p;
		dir = glm::normalize(glm::vec3(d));
		up = glm::normalize(glm::vec3(u));
		pos = glm::vec3(p / p.w);
	}
	glm::mat4 getViewMatrix() {
		return glm::lookAt(pos, pos + glm::vec3(dir), glm::vec3(up));
	}
	glm::mat4 getProjectionMatrix() { return projectMat; }
};