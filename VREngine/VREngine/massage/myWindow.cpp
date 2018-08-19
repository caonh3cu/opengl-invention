#include "myWindow.h"
namespace Mymassage {
	void Init(int _width, int _height) {
		width = _width;
		height = _height;
		for (int i = 0; i < 128; i++)msg[i] = 0;
	}

	void error_callback(int error, const char* description)
	{
		cout << "error : " << description << endl;
		return;
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		MyWindow::Message m;
		if (action == GLFW_PRESS)
			m.type = MyWindow::Message::keybordPress;
		else if (action == GLFW_RELEASE)
			m.type = MyWindow::Message::keybordRelese;
		else
			m.type = MyWindow::Message::unknow;

		m.value.value = key;

		MyWindow::getInstance()->messagePump.push(m);
	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		MyWindow::Message m;
		if (action == GLFW_PRESS)
			m.type = MyWindow::Message::mousePress;
		else if (action == GLFW_RELEASE)
			m.type = MyWindow::Message::mouseRelese;
		else
			m.type = MyWindow::Message::unknow;
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m.value.mouse = MyWindow::Message::MauseEvent::left;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			m.value.mouse = MyWindow::Message::MauseEvent::middle;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			m.value.mouse = MyWindow::Message::MauseEvent::right;
			break;
		default:
			return;
		}
		MyWindow::getInstance()->messagePump.push(m);
		return;
	}
	void cursor_position_callback(GLFWwindow* window, double x, double y)
	{
		xpos = float((x - width / 2) / width) * 2;
		ypos = float(0 - (y - height / 2) / height) * 2;

		MyWindow::Message m;
		m.type = MyWindow::Message::mouseMove;
		m.value.pos = glm::vec2(xpos, ypos);
		MyWindow::getInstance()->messagePump.push(m);
		return;
	}
	void scroll_callback(GLFWwindow* window, double x, double y)
	{
		return;
	}

	MyWindow* MyWindow::instance = NULL;


}