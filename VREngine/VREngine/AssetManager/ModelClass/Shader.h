#pragma once
//shader compile and link
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h>


class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(GLuint vertex, GLuint fragment)
	{
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);

		// Print linking errors if any
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	GLuint getUniformLocation(const char *s) { return glGetUniformLocation(Program, s); }


	enum shaderType{vertexShader,fragmentShader};

	static GLuint compileShader(string Path,shaderType type){
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string Code;
		std::ifstream ShaderFile;
		// ensures ifstream objects can throw exceptions:
		ShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			ShaderFile.open(Path);
			if (!ShaderFile.is_open() ) {
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << Path << std::endl;
			}
			std::stringstream ShaderStream;
			// Read file's buffer contents into streams
			ShaderStream << ShaderFile.rdbuf();
			// close file handlers
			ShaderFile.close();
			// Convert stream into string
			Code = ShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << Path <<  std::endl;
		}
		const GLchar* ShaderCode = Code.c_str();

		// 2. Compile shaders
		GLuint shaderId;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		switch (type) {
		case vertexShader:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case fragmentShader:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		}
		glShaderSource(shaderId, 1, &ShaderCode, NULL);
		glCompileShader(shaderId);
		// Print compile errors if any
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << Path << "\n" << infoLog << std::endl;
		}
		return shaderId;
	}
	
	static void deleteShader(GLuint shader) { glDeleteShader(shader); }
};
