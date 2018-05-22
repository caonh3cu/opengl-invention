#pragma once

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
#include <queue>
using namespace std;

// GLFW
#include <GLFW/glfw3.h>

namespace Mymassage {
	static char msg[128];
	static GLint width, height;
	static GLfloat xpos, ypos;
	void Init(int _width, int _height);
	void error_callback(int error, const char* description);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double x, double y);
	void scroll_callback(GLFWwindow* window, double x, double y);

	class MyWindow {
	public:
		static MyWindow *InitWindow(int width, int height, string name) {
			if (instance == NULL) {
				instance = new MyWindow(width, height, name);
			}
			return instance;
		}
		static MyWindow *getInstance() {
			if (instance == NULL)
				instance = new MyWindow();
			return instance;
		}
		void setCursorPos(double x, double y) {
			glfwSetCursorPos(window, x, y);
		}
		void close() {
			glfwTerminate();
		}
		void flush() {
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		int width, height;

	public:
		class Message {
		public:
			enum Type {
				mousePress, mouseRelese, keybordPress, keybordRelese, window, unknow
			};
			enum MauseEvent {
				left, middle, right
			};
			union Value {
				MauseEvent mause;
				unsigned char value;
			};
			Type type;
			Value value;
		};
		queue<Message> messagePump;
	private:
		static MyWindow* instance;
		GLFWwindow* window;

		MyWindow(int _width = 800, int _height = 800, string name = "default") 
			:width(_width), height(_height), messagePump()
		{
			Mymassage::Init(width, height);
			glfwSetErrorCallback(Mymassage::error_callback);
			if (!glfwInit())
			{
				cout << "faile to init glfw" << endl;
				return;
			}
			window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
			if (window == NULL)
			{
				glfwTerminate();
				cout << "faile to create window" << endl;
				return;
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
		}


	};
}