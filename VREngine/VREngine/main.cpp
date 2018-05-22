
#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> // Contains all the necessery OpenGL includes


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;


#include <fstream>
#include <iostream>
using namespace std;

// GLFW
#include <GLFW/glfw3.h>

#include "AssetManager\AssetManager.h"
#include "object\object.h"
#include "Scene\scene.h"

int main(int argc, char *argv[]) {
	Scene scene;
	scene.init();
	scene.run();
}
int main1(int argc, char *argv[])
{
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();


	GLFWwindow* window;
	GLint width, height;
	width = 900;
	height = 900;
	Mymassage::Init(width, height);
	glfwSetErrorCallback(Mymassage::error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(width, height, "opengl tutorial 002-color box", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	glfwSetKeyCallback(window, Mymassage::key_callback);
	glfwSetMouseButtonCallback(window, Mymassage::mouse_button_callback);
	glfwSetCursorPosCallback(window, Mymassage::cursor_position_callback);
	glfwSetScrollCallback(window, Mymassage::scroll_callback);
	glfwMakeContextCurrent(window);
	glewExperimental = true; // Needed for core propbmpfile  
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	std::ifstream configFile;
	string config;
	// ensures ifstream objects can throw exceptions:
	configFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		std::stringstream configStream;
		configFile.open("config.txt");;
		if (!configFile.is_open()) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		// Read file's buffer contents into streams
		configStream << configFile.rdbuf();
		config = configStream.str();
		// close file handlers
		configFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}


	AssetManager *am = AssetManager::getInstance();
	ObjectManager *om = ObjectManager::getInstance();
	am->readConfig(config);
	om->readConfig(config);
	Scene scene;
	scene.light.position = vec3(-5, 5, 5);
	//scene.camera.pos = vec3(0, 5, 0);
	scene.camera.rollUp(radians(-70.0f));
	scene.shader = am->shaderPrograms["shader1"];

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);
		glClearColor(0, 0, 0.9f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		scene.light.position.x += 0.01;
		//scene.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}