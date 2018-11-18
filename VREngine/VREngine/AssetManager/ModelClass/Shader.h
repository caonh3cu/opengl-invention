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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader() { Program = 0; }
	Shader(Shader &s) { Program = s.Program; }
	Shader &operator=(Shader&s) { Program = s.Program; return *this; }
	Shader(string vertex,string fragment) { 
		GLuint vs = Shader::compileShader(vertex, Shader::shaderType::vertexShader);
		GLuint fs = Shader::compileShader(fragment, Shader::shaderType::fragmentShader);
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vs);
		glAttachShader(this->Program, fs);
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

	void use() { glUseProgram(Program); }
	void setMat4(const char *s, glm::mat4 &value) { glUniformMatrix4fv(glGetUniformLocation(Program, s), 1, GL_FALSE, &(value)[0][0]); }
	void set3f(const char *s, glm::vec3 &value) { glUniform3fv(glGetUniformLocation(Program, s), 1, &(value)[0]); }
	void set3f(const char *s, glm::vec4 &value) { glUniform3fv(glGetUniformLocation(Program, s), 1, &(value)[0]); }
	void setInt(const char *s, GLint value) { glUniform1i(glGetUniformLocation(Program, s), value); }


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
