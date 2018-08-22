#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

#include <SOIL/SOIL.h>

class Meterial {
public:
	Meterial(): 
		specularColor(0,0,0), 
		diffuseColor(0, 0, 0),
		ambientColor(0, 0, 0),
		shininess(0),
		hasSpecular(false), 
		hasDiffuse(false),
		hasAmbient(false),
		hasShininess(false),

		hasNormal(false),
		hasHeight(false),
		hasLightmap(false), 
		hasEmissive(false),
		isLoaded(false)
	{
	}
	string name;
	glm::vec3 specularColor, diffuseColor, ambientColor;
	float shininess;
	string specularPath, diffusePath, ambientPath, shininessPath;
	GLuint specularId, diffuseId, ambientId, shininessId;
	//0表示颜色，1表示贴图
	bool hasSpecular, hasDiffuse, hasAmbient, hasShininess;
	
	string normalPath, heightPath, lightmapPath, emissivePath;
	GLuint normalId, heightId, lightmapId, emissiveId;
	bool hasNormal, hasHeight, hasLightmap, hasEmissive;
	bool isLoaded;
	//将材质根据shader绑定uniform
	unsigned int bindMeterial(GLuint shader, unsigned int startTexture = 0);
};

class Texture {
public:
	string path;
	GLuint id;
	//从文件读纹理，有mipmap
	Texture():id(0){}
	Texture(string _path):path(_path)
	{
		//Generate texture ID and load texture data 
		string filename = path;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			cout << "texture load failed.can't open " << filename << endl;
			return;
		}
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		id = textureID;
	}
};