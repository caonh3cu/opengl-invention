
#include "sceneDelo.h"
void SceneDelo::init() {
	
	width = 900;
	height = 900;
	window = Mymassage::MyWindow::InitWindow(width, height, "driver");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return;
	}


	
}

void SceneDelo::run() {
	while (1) {
		if (!handleInput())
			break;
		glClearColor(1.0f,1.0f,1.0f,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, width, height);
		glEnable(GL_POINT_SMOOTH);
		draw();

		window->flush();
	}
	window->close();
}

void SceneDelo::draw() {
	if (state[isComplete]) {
		delo.draw();
	}
	else if(pointList.size()>0)
	{
		vec2 p = pointList[0];
		map2d::drawPoint(p.x, p.y, 10.0f);
		state[isInsect] = false;
		for (int i = 0; i < pointList.size()-1; i++) {
			map2d::drawLine(pointList[i], pointList[i + 1],vec3(0,1,0));
			p = pointList[i + 1];
			map2d::drawPoint(p.x, p.y, 10.0f);
			if (
				i!= pointList.size() - 2&&Delo::intersect(pointList[i], pointList[i + 1],
					pointList[pointList.size() - 1], mousePos))
				state[isInsect] = true;
		}
		
		if(state[isInsect])
			map2d::drawLine(pointList[pointList.size() - 1], mousePos, vec3(1, 0, 0));
		else
			map2d::drawLine(pointList[pointList.size() - 1], mousePos, vec3(0, 1, 1));
		vec2 pos(mousePos);
		if ((pos.x - pointList[0].x)*(pos.x - pointList[0].x) + (pos.y - pointList[0].y)*(pos.y - pointList[0].y)<0.001)
			map2d::drawPoint(pointList[0].x, pointList[0].y, 20.0f);

	}
}

bool SceneDelo::handleInput() {
	if (!progressMessage()) return false;


	return true;
}

bool SceneDelo::progressMessage(){
	while (!window->messagePump.empty()) {
		Mymassage::MyWindow::Message m = window->messagePump.front();
		window->messagePump.pop();
		if (m.type == Mymassage::MyWindow::Message::keybordPress) {
			switch (m.value.value) {
			case GLFW_KEY_ESCAPE:
				return false;
				break;
			case GLFW_KEY_F: {
				delo.clear();
				state[isComplete] = false;
				int s = pointList.size();
				for (int i = 0; i < s; i++)
					pointList.pop_back();
				break;
			}
			}
		}
		if (m.type == Mymassage::MyWindow::Message::mouseMove) {
			mousePos = m.value.pos;
			mousePos.y = -mousePos.y;
		}
		if (m.type == Mymassage::MyWindow::Message::mousePress) {
			if (m.value.mouse == Mymassage::MyWindow::Message::MauseEvent::left) {
				if ((!state[isComplete])&&(!state[isInsect])) {
					vec2 pos = mousePos;
					
					if(pointList.size()==0)
						pointList.push_back(pos);
					else if((pos.x - pointList[0].x)*(pos.x - pointList[0].x)+ (pos.y - pointList[0].y)*(pos.y - pointList[0].y)>0.001)
						pointList.push_back(pos);
					else {
						state[isComplete] = true;
						if (!delo.addPoint(pointList)) {
							pointList.pop_back();
							state[isComplete] = false;
						}
					}
				}
			}
		}
	}
	return true;
}