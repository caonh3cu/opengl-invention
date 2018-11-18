
#include "scene.h"
void Scene::init() {
	//read config
	MyLoader::Loader loader;
	loader.load("config.txt");
	vector<MyLoader::Node*> tn = loader.root.getNodes("set");
	// set some value
	for(int i=0;i<tn.size();i++)
		readSet(tn[i]);
	
	//has vr or not
	if (hasVR)
		width = 450;
	else
		width = 900 + 450;
	height = 900;
	//create the window
	window = Mymassage::MyWindow::InitWindow(width, height, "driver");
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return;
	}

	//asset manager and object manager
	AssetManager *am = AssetManager::getInstance();
	ObjectManager *om = ObjectManager::getInstance(); 
	
	vector<MyLoader::Node*> tnm,tns;
	tnm = loader.root.getNodes("model");
	tns = loader.root.getNodes("shader");
	//compile shader
	for (int i = 0; i<tns.size(); i++)
		am->readConfig(tns[i]);
	for (int i = 0; i < tnm.size(); i++) {
		//顺序不能反过来
		//load meshes
		am->readConfig(tnm[i]);
		//create object with mesh
		om->readConfig(tnm[i]);
	}
	//get shader id
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

void Scene::readSet(MyLoader::Node* node) {
	MyLoader::Node* tn = node->getNode("hasVR");
	if (tn == NULL)return;
	string value = tn->value;
	hasVR = (value.compare("true") == 0);
	cout << "set " << " hasVR" << " " << (value.compare("true") == 0) << std::endl;
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
	shader.use();
	light.bindLight(shader.Program);
	shader.set3f("viewPos", camera.getVirtulPos());
	shader.setMat4("view", camera.getVirtualViewMatrix());
	if (hasVR) {
		shader.setMat4("projection", vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, 0.1, 500));
		shader.setMat4("eyeMat", vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right));
	}
	else {
		shader.setMat4("projection", cameraNoVR.getProjectionMatrix());
		shader.setMat4("eyeMat", mat4(1.0f));
	}
	ObjectManager::getInstance()->drawObject("migong", shader.Program);
	ObjectManager::getInstance()->drawObject("floor", shader.Program);

	shaderBox.use();
	shaderBox.set3f("viewPos", camera.getVirtulPos());
	shaderBox.setMat4("view", camera.getRealViewMatrix());
	if (hasVR) {
		shaderBox.setMat4("projection", vr.GetHMDMatrixProjectionEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, 0.1, 500));
		shaderBox.setMat4("eyeMat", vr.GetHMDMatrixPoseEye(isLeftEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right));
	}
	else {
		shaderBox.setMat4("projection", cameraNoVR.getProjectionMatrix());
		shaderBox.setMat4("eyeMat", mat4(1.0f));
	}
	ObjectManager::getInstance()->drawObject("box", shaderBox.Program);
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
		if (state[LEFT])cameraNoVR.rollLeft(0.015);
		if (state[RIGHT])cameraNoVR.rollLeft(-0.015);
		if (state[UP])cameraNoVR.rollUp(0.015);
		if (state[DOWN])cameraNoVR.rollUp(-0.015);
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
			case GLFW_KEY_LEFT:
				state[LEFT] = true;
				break;
			case GLFW_KEY_RIGHT:
				state[RIGHT] = true;
				break;
			case GLFW_KEY_UP:
				state[UP] = true;
				break;
			case GLFW_KEY_DOWN:
				state[DOWN] = true;
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
			case GLFW_KEY_LEFT:
				state[LEFT] = false;
				break;
			case GLFW_KEY_RIGHT:
				state[RIGHT] = false;
				break;
			case GLFW_KEY_UP:
				state[UP] = false;
				break;
			case GLFW_KEY_DOWN:
				state[DOWN] = false;
				break;
			case GLFW_KEY_SPACE:
				state[SPACE] = false;
				break;
			}
		}
	}
	return true;
}

