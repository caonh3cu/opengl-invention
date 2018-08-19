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

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 

class Light {
public:
	Light(
		glm::vec3 _position = glm::vec3(3.0f, 3.0f, 14.0f),
		glm::vec3 _ambient = glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3 _diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 _specular = glm::vec3(1.0f, 1.0f, 1.0f),
		float _constant = 1.0f,
		float _linear = 0.009,
		float _quadratic = 0.0022)
		:position(_position),
		ambient(_ambient),
		diffuse(_diffuse),
		specular(_specular),
		constant(_constant),
		linear(_linear),
		quadratic(_quadratic)
	{

	}
	glm::vec3 position, ambient, diffuse, specular;
	float constant, linear, quadratic;
	void bindLight(GLuint shader, int i = 0) {
		char ts[20];
		_itoa_s(i, ts, 18);
		glUniform3f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].position").c_str()), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].specular").c_str()), specular.x, specular.y, specular.z);
		glUniform1f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shader, (string("pointLights[") + ts + "].quadratic").c_str()), quadratic);
	}
};