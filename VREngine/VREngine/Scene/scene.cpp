
#include "scene.h"
void Scene::init() {
	
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

}
void Scene::run() {
	light.position = vec3(-5, 5, 5);
	camera.realCamera.pos = camera.virtualCamera.pos = vec3(0,1, 0);
	//glUseProgram(shader);
	//light.bindLight(shader);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	while (1) {
		if (!handleInput())
			break;
		glClearColor(1.0f,1.0f,0.0f,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, width, height);
		draw();

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