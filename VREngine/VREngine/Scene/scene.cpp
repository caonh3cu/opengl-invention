
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

	light.position = vec3(-5, 5, 5);
	
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
		if (!handleInput())
			break;

		vr.VRBegin(true);
		drawVR(true);
		vr.VREnd(true);
		vr.VRBegin(false);
		drawVR(false);
		vr.VREnd(false);
		vr.Submit();

		glClearColor(1.0f,1.0f,1.0f,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, width*2/3, height);
		draw();

		//固定管线
		glUseProgram(0);
		//分割线
		glViewport(0, 0, width, height);
		map2d::drawLine(vec2(1.0f / 3, 1), vec2(1.0f / 3, -1), vec3(0.3, 0.6, 0), 5);
		map2d::drawLine(vec2(1.0f / 3, 0), vec2(1, 0), vec3(0.3, 0.6, 0), 5);

		vec3 pos, dir;
		//上面的地图
		glViewport(width * 2 / 3, height / 2, width / 3, height / 2);
		map2d::drawVirtul();
		pos = (camera.realCamera.pos);
		pos *= 0.2;
		map2d::drawPoint(pos.x, pos.z, 10.0f);
		dir = (pos + camera.realCamera.dir*0.15f);
		map2d::drawLinv(vec2(pos.x, pos.z), vec2(dir.x, dir.z), vec3(0.8, 1.0, 0.5));

		//下面的地图
		glViewport(width * 2 / 3, 0, width / 3, height / 2);
		glUseProgram(0);
		map2d::drawReal();
		pos = (camera.realCamera.pos);
		pos *= 0.4;
		map2d::drawPoint(pos.x,pos.z,20.0f);
		dir = (pos+camera.realCamera.dir*0.3f);
		map2d::drawLinv(vec2(pos.x, pos.z), vec2(dir.x, dir.z), vec3(0.8, 1.0, 0.5));
		window->flush();
	}
	window->close();
}
void Scene::draw() {
	glUseProgram(shader);
	light.bindLight(shader);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &(camera.virtualCamera.pos)[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &(camera.virtualCamera.getViewMatrix())[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &(camera.virtualCamera.projectMat)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "eyeMat"), 1, GL_FALSE, &(mat4(1.0f)[0][0]));
	ObjectManager::getInstance()->drawObject("floor",shader);
}
void Scene::drawVR(bool isLeftEye) {
	glUseProgram(shader);
	light.bindLight(shader);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &((vr.GetHMDMatrixPoseEye(isLeftEye?vr::Hmd_Eye::Eye_Left:vr::Hmd_Eye::Eye_Right))[3])[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &(vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &(vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right,0.1,500))[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "eyeMat"), 1, GL_FALSE, &(mat4(1.0f)[0][0]));
	ObjectManager::getInstance()->drawAllObject(shader);
}
bool Scene::handleInput() {
	if (!progressMessage()) return false;

	if (state[0])camera.rollUp(0.015);
	if (state[1])camera.rollLeft(0.015);
	if (state[2])camera.rollUp(-0.015);
	if (state[3])camera.rollLeft(-0.015);
	if (state[4])camera.move(0.05, 0);
	if (state[5])camera.move(-0.05, 0);
	if (state[6])camera.move(0, 0.05);
	if (state[7])camera.move(0, -0.05);

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