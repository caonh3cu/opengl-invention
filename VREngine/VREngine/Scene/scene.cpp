
#include "scene.h"
void Scene::init() {

	Loader loader;
	loader.load("config.txt");
	readSet(loader.stringMap["set"] + "end\n");
	
	if (hasVR)
		width = 450;
	else
		width = 900 + 450;
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


	AssetManager *am = AssetManager::getInstance();
	ObjectManager *om = ObjectManager::getInstance();
	am->readConfig(loader.stringMap["model"] + loader.stringMap["shader"] + "end\n");
	om->readConfig(loader.stringMap["model"] + "end\n");
	shader = am->shaderPrograms["roll"]; 
	shaderBox = am->shaderPrograms["box"];
	light.position = vec3(-5, 5, 5);


	if (hasVR) {
		if (!vr.Init()) {
			cout << "failed to init VR" << endl;
		}
		camera.updateView(vr.HMDPoseInverse);
	}
	else {
		cameraNoVR.pos = vec3(0, 1.5, 0);
		camera.updateView(cameraNoVR.getViewMatrix());
	}

	frameNum = 0;
	
	
	ObjectManager::getInstance()->objects["floor"].scale(vec3(2, 2, 2));

}

void Scene::readSet(string config) {
	stringstream configStream;
	configStream.str(config);
	string operation, values1, values2, values3;
	int valuei;
	configStream >> operation;
	while (operation.compare("end") != 0) {
		if (operation.compare("set") == 0) {
			configStream >> values1 >> values2;
			if (values1.compare("hasVR") == 0) {
				if (values2.compare("true") == 0)
					hasVR = true;
				else
					hasVR = false;
				cout << "set hasVR:" << hasVR << endl;
			}
		}
		configStream >> operation;
	}
}

void Scene::run() {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	while (1) {
		if (!handleInput())
			break;

		if (hasVR) {
			//左眼
			vr.VRBegin(true);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw(true);
			vr.VREnd(true);
			//右眼
			vr.VRBegin(false);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw(false);
			vr.VREnd(false);
			//提交
			vr.Submit();
		}
		else {
			glClearColor(1.0f, 1.0f, 1.0f, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, width * 2 / 3, height);
			draw(true);
		}

		draw2D();

		window->flush();
	}
	window->close();
}

//画场景
void Scene::draw(bool isLeftEye) {
	glUseProgram(shader);
	light.bindLight(shader);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &(camera.getVirtulPos())[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &(camera.getVirtualViewMatrix())[0][0]);
	if (hasVR) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &(vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, 0.1, 500))[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "eyeMat"), 1, GL_FALSE, &((vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right))[0][0]));
	}
	else {
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &(cameraNoVR.getProjectionMatrix())[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "eyeMat"), 1, GL_FALSE, &(mat4(1.0f)[0][0]));
	}
	ObjectManager::getInstance()->drawObject("migong", shader);
	ObjectManager::getInstance()->drawObject("floor", shader);

	glUseProgram(shaderBox);
	glUniform3fv(glGetUniformLocation(shaderBox, "viewPos"), 1, &(camera.getVirtulPos())[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderBox, "view"), 1, GL_FALSE, &(camera.getRealViewMatrix())[0][0]);	
	if (hasVR) {
		glUniformMatrix4fv(glGetUniformLocation(shaderBox, "projection"), 1, GL_FALSE, &(vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, 0.1, 500))[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderBox, "eyeMat"), 1, GL_FALSE, &((vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right))[0][0]));
	}
	else {
		glUniformMatrix4fv(glGetUniformLocation(shaderBox, "projection"), 1, GL_FALSE, &(cameraNoVR.getProjectionMatrix())[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderBox, "eyeMat"), 1, GL_FALSE, &(mat4(1.0f)[0][0]));
	}
	ObjectManager::getInstance()->drawObject("box", shaderBox);
}

//画两个地图
void Scene::draw2D() {
	//固定管线
	glUseProgram(0);
	vec4 pos, dir;
	//分割线
	if (hasVR) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
		map2d::drawLine(vec2(-1.0f, 0), vec2(1.0f, 0), vec3(0.3, 0.6, 0), 5);
	}
	else {
		glViewport(0, 0, width, height);
		map2d::drawLine(vec2(1.0f / 3, 0), vec2(1.0f, 0), vec3(0.3, 0.6, 0), 5);
		map2d::drawLine(vec2(1.0f / 3, -1.0f), vec2(1.0f / 3, -1.0f), vec3(0.3, 0.6, 0), 5);
	}
	//上面的地图
	if (hasVR)
		glViewport(0, height / 2, width, height / 2);
	else
		glViewport(width * 2 / 3, height / 2, width / 3, height / 2);
	map2d::drawVirtul();
	pos = (camera.getVirtulPos());
	pos += vec4(-1.9f, 0, 1.9f, 0);
	pos /= 4.0f * pos.w;
	map2d::drawPoint(pos.x, pos.z, 10.0f);
	dir = (pos + camera.getVirtulDir()*0.15f);
	//朝向
	map2d::drawLinv(vec2(pos.x, pos.z), vec2(dir.x, dir.z), vec3(0.8, 1.0, 0.5));

	//下面的地图
	if (hasVR)
		glViewport(0, 0, width, height / 2);
	else
		glViewport(width * 2 / 3, 0, width / 3, height / 2);
	map2d::drawReal();
	pos =  (camera.getRealPos());
	pos *= 0.4 / pos.w;
	map2d::drawPoint(pos.x, pos.z, 20.0f);
	dir = (pos + camera.getRealDir()*0.3f);
	//方向
	map2d::drawLinv(vec2(pos.x, pos.z), vec2(dir.x, dir.z), vec3(1.0, 1.0, 0.5));

}


bool Scene::handleInput() {
	if (!progressMessage()) return false;

	if (hasVR) {
		vr.HandleInput();
		camera.updateView(vr.HMDPoseInverse);
	}
	else {
		if (state[Q])cameraNoVR.rollLeft(0.015);
		if (state[E])cameraNoVR.rollLeft(-0.015);
		if (state[W])cameraNoVR.move(0.01, 0);
		if (state[S])cameraNoVR.move(-0.01, 0);
		if (state[A])cameraNoVR.move(0, 0.01);
		if (state[D])cameraNoVR.move(0, -0.01);
		camera.updateView(cameraNoVR.getViewMatrix());
	}

	frameNum++;



	return true;
}
bool Scene::progressMessage(){
	while (!window->messagePump.empty()) {
		Mymassage::MyWindow::Message m = window->messagePump.front();
		window->messagePump.pop();
		if (m.type == Mymassage::MyWindow::Message::keybordPress) {
			switch (m.value.value) {
			case GLFW_KEY_ESCAPE:
				return false;
				break;
			case GLFW_KEY_W:
				state[W] = true;
				break;
			case GLFW_KEY_A:
				state[A] = true;
				break;
			case GLFW_KEY_S:
				state[S] = true;
				break;
			case GLFW_KEY_D:
				state[D] = true;
				break;
			case GLFW_KEY_Q:
				state[Q] = true;
				break;
			case GLFW_KEY_E:
				state[E] = true;
				break;
			case GLFW_KEY_P:
				state[P] = true;
				break;
			case GLFW_KEY_SPACE:
				state[SPACE] = true;
				break;

			}
		}
		if (m.type == Mymassage::MyWindow::Message::keybordRelese) {
			switch (m.value.value) {
			case GLFW_KEY_W:
				state[W] = false;
				break;
			case GLFW_KEY_A:
				state[A] = false;
				break;
			case GLFW_KEY_S:
				state[S] = false;
				break;
			case GLFW_KEY_D:
				state[D] = false;
				break;
			case GLFW_KEY_Q:
				state[Q] = false;
				break;
			case GLFW_KEY_E:
				state[E] = false;
				break;
			case GLFW_KEY_SPACE:
				state[SPACE] = false;
				break;
			}
		}
	}
	return true;
}

