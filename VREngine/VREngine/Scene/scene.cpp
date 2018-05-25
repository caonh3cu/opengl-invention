
#include "scene.h"
void Scene::init() {
	
	width = 1350;
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
	shader = am->shaderPrograms["shader1"];
	
	bool isSuccess = vr.Init();
	if (isSuccess)
		cout << "success init vr" << endl;
	else
		cout << "failed to init vr" << endl;
}
void Scene::run() {
	//glUseProgram(shader);
	//light.bindLight(shader);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	while (1) {
		//handleInput中调用了vrDriver类的handleInput方法，更新头盔和手柄的位置
		if (!handleInput())
			break;

		//左眼
		vr.VRBegin(true);
		drawVR(true);
		vr.VREnd(true);
		//右眼
		vr.VRBegin(false);
		drawVR(false);
		vr.VREnd(false);
		//提交
		vr.Submit();

	}
	vr.Shutdown();
	window->close();
}
void Scene::drawVR(bool isLeftEye) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader);
	light.bindLight(shader);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &((vr.HMDPoseInverse)[3])[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &(vr.HMDPoseInverse)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &(vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right,0.1,500))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "eyeMat"), 1, GL_FALSE, &((vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right))[0][0]));
	ObjectManager::getInstance()->drawAllObject(shader);
}
bool Scene::handleInput() {
	if (!progressMessage()) return false;

	vr.HandleInput();

	return true;
}
bool Scene::progressMessage(){
	while (!window->messagePump.empty()) {
		Mymassage::MyWindow::Message m = window->messagePump.front();
		window->messagePump.pop();
		if (m.type == Mymassage::MyWindow::Message::keybordPress) {
			switch (m.value.value) {
			case GLFW_KEY_SPACE:
				return false;
				break;
			case GLFW_KEY_W:
				state[0] = true;
				break;
			case GLFW_KEY_A:
				state[1] = true;
				break;
			case GLFW_KEY_S:
				state[2] = true;
				break;
			case GLFW_KEY_D:
				state[3] = true;
				break;
			case GLFW_KEY_I:
				state[4] = true;
				break;
			case GLFW_KEY_K:
				state[5] = true;
				break;
			case GLFW_KEY_J:
				state[6] = true;
				break;
			case GLFW_KEY_L:
				state[7] = true;
				break;

			}
		}
		if (m.type == Mymassage::MyWindow::Message::keybordRelese) {
			switch (m.value.value) {
			case GLFW_KEY_SPACE:
				return false;
				break;
			case GLFW_KEY_W:
				state[0] = false;
				break;
			case GLFW_KEY_A:
				state[1] = false;
				break;
			case GLFW_KEY_S:
				state[2] = false;
				break;
			case GLFW_KEY_D:
				state[3] = false;
				break;
			case GLFW_KEY_I:
				state[4] = false;
				break;
			case GLFW_KEY_K:
				state[5] = false;
				break;
			case GLFW_KEY_J:
				state[6] = false;
				break;
			case GLFW_KEY_L:
				state[7] = false;
				break;
			}
		}
	}
	return true;
}