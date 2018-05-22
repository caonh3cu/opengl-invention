#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class OlaCamera {
public:
	OlaCamera(
		float _nearClip = 0.3f, 
		float _farClip = 500.0f,
		float _FOV = 45.0f,
		float _aspect = 1.0f,
		vec3 _pos = vec3(0),
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
		vec4 dir = rotate(mat4(1.0f), yaw, vec3(0.0f, 1.0f, 0.0f)) * (rotate(mat4(1.0f), pitch, vec3(1, 0, 0))*dir);
		vec4 up = rotate(mat4(1.0f), roll, vec3(dir))*up;
		vec3 l = cross(vec3(up), vec3(dir));
		dir = normalize(dir);
		l = normalize(l);
		pos -= vec3(dir)*front+l*left;
	}
	void addPitch(float d) {
		pitch -= d;
		if (pitch > radians(90.0f))pitch = 90;
		if (pitch < radians(-90.0f))pitch = -90;
	}
	void addYaw(float d) {
		yaw -= d;
		if (pitch > radians(360.0f))pitch -= 360;
		if (pitch < radians(-360.0f))pitch += 360;
	}
	void addRoll(float d) {
		roll -= d;
		if (pitch > radians(360.0f))pitch -= 360;
		if (pitch < radians(-360.0f))pitch += 360;
	}
	mat4 getViewMatrix() {
		vec4 dir(0,0,1,0), up(0,1,0,0);
		dir = rotate(mat4(1.0f), yaw, vec3(0.0f, 1.0f, 0.0f)) * (rotate(mat4(1.0f), pitch, vec3(1, 0, 0))*dir);
		up = rotate(mat4(1.0f), roll, vec3(dir))*up;
		return lookAt(pos,pos+vec3(dir),vec3(up));
	}
	mat4 getProjectionMatrix() { return perspective(radians(FOV), aspect, nearClip, farClip); }
	vec3 pos;
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
		vec3 _pos = vec3(0),
		float pitch = 0,
		float yaw = 0,
		float roll = 0)
		:nearClip(_nearClip),
		farClip(_farClip),
		FOV(_FOV),
		aspect(_aspect),
		pos(_pos)
	{
		vec4 d(0, 0, 1, 0), u(0, 1, 0, 0);
		d = rotate(mat4(1.0f), yaw, vec3(0.0f, 1.0f, 0.0f)) * (rotate(mat4(1.0f), pitch, vec3(1, 0, 0))*d);
		u = rotate(mat4(1.0f), roll, vec3(d))*u;
		vec3 s = glm::cross(vec3(d), vec3(u));
		up = cross(s, vec3(d));
		dir = vec3(d);
		up = normalize(up);
		dir = normalize(dir);
		projectMat = perspective(radians(FOV), aspect, nearClip, farClip);
	}
public:
	mat4 projectMat;
	vec3 dir,up;	
	vec3 pos;

	float nearClip, farClip, FOV, aspect;

public:
	void move(float front, float left) {
		pos += front*dir + cross(up, dir)*left;
	}
	void rollUp(float r) {
		vec4 d(dir, 0),u(up,0);
		d = rotate(mat4(1.0f), r, cross(dir, up))*d;
		u = rotate(mat4(1.0f), r, cross(dir, up))*u;
		dir = normalize(vec3(d));
		up = normalize(vec3(u));
	}
	void rollLeft(float r) {
		vec4 d(dir, 0), u(up, 0);
		d = rotate(mat4(1.0f), r, vec3(0, 1, 0))*d;
		u = rotate(mat4(1.0f), r, vec3(0, 1, 0))*u;
		dir = normalize(vec3(d));
		up = normalize(vec3(u));
	}
	mat4 getViewMatrix() {
		return lookAt(pos, pos + vec3(dir), vec3(up));
	}
	mat4 getProjectionMatrix() { return projectMat; }
};