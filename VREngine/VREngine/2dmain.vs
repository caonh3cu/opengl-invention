#include "redirectionController/redirectionController.h"

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

#include "massage/massage.h"

#define V_WALL_NUM 10

vec2 point1, point2;
vec2 speed1, speed2;
vec2 force1, force2;
vec2 theta1, theta2;
int id1, id2;
float BOUNDARY = 0.8;
redirectionController *rc;
GLfloat time;
unsigned int frameNum;

vec2 vWall[V_WALL_NUM*2] = {
	vec2(0.85,0.85),vec2(0.85,-0.85),
	vec2(0,0),vec2(0,0),
	vec2(0,0),vec2(0,0),

	vec2(0,0),vec2(0,0),
	vec2(0,0),vec2(0,0),
	vec2(0,0),vec2(0,0),

	vec2(0,0),vec2(0,0),
	vec2(0,0),vec2(0,0),
	vec2(0,0),vec2(0,0),

	vec2(0,0),vec2(0,0),

};

void drawPoint(float x, float y,float r) {
	if (r < 10)r = 22.5f;
	glPointSize(r);
	glBegin(GL_POINTS);

	glColor3f(1.0, 0.0, 0.0);    // Red  
	glVertex2f(x, y);
	glEnd();
}
void drawLinv(vec2 p1,vec2 p2,vec3 color)
{

	glLineWidth(4);//设置线段宽度  
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex2f(p1.x, p1.y); //定点坐标范围  
	
	glVertex2f(p2.x, p2.y);
	glEnd();

	vec2 dir = p2 - p1;
	dir /= sqrt(dir.x*dir.x + dir.y*dir.y);
	vec2 left(-dir.y, dir.x),right(dir.y, -dir.x);
	left = p2 - ((dir - left*0.6f)*0.02f);
	right = p2 - ((dir - right*0.6f)*0.02f);
	glBegin(GL_TRIANGLES);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(p2.x+dir.x*0.02f, p2.y+dir.y*0.02f, 0.0);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(left.x, left.y, 0.0);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(right.x, right.y, 0.0);
	glEnd();
}
void drawLine(vec2 p1, vec2 p2,vec3 color) {
	glLineWidth(2);//设置线段宽度  
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex2f(p1.x, p1.y); //定点坐标范围  

	glVertex2f(p2.x, p2.y);
	glEnd();
}
//正常一帧一帧画
void draw_scene()
{

	//网格
	drawLine(vec2(1, 0), vec2(-1, 0), vec3(1, 0.85, 0.85));
	drawLine(vec2(0, -1), vec2(0, 1), vec3(1, 0.85, 0.85)); 
	for(int i=0;i<V_WALL_NUM;i++)
		drawLine(vWall[i * 2], vWall[i * 2 + 1], vec3(1, 0.3, 0.3));

	for (float i = -0.9; i < 0.99; i += 0.2) {
		if (fabs(fabs(i) - BOUNDARY)>0.01) {
			drawLine(vec2(1, i), vec2(-1, i), vec3(1, 1, 0.7));
			drawLine(vec2(i, -1), vec2(i, 1), vec3(1, 1, 0.7));
		}
	}

	float r;
	//两点
	drawPoint(point1.x, point1.y, 20.0f);
	drawPoint(point2.x, point2.y, 20.0f);


	//力
	drawLinv(point1, point1 + force1, vec3(0, 0, 0));
	drawLinv(point2, point2 + force2, vec3(0, 0, 0));
	//速度
	drawLinv(point1, point1 + speed1, vec3(0.8, 1.0, 0.5));
	drawLinv(point2, point2 + speed2, vec3(0.8, 1.0, 0.5));
	//角度
	drawLinv(point1, point1 + theta1, vec3(0, 1.0, 0));
	drawLinv(point2, point2 + theta2, vec3(0, 1.0, 0));

	//边界
	drawLine(vec2(BOUNDARY, BOUNDARY), vec2(-BOUNDARY, BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(-BOUNDARY, BOUNDARY), vec2(-BOUNDARY, -BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(-BOUNDARY, -BOUNDARY), vec2(BOUNDARY, -BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(BOUNDARY, -BOUNDARY), vec2(BOUNDARY, BOUNDARY), vec3(0.5, 0.5, 0.1));

	
}


//初始化
void init() {
	rc = redirectionController::getHandle();
	rc->Init(BOUNDARY);
	rc->addVirtulWall(vWall[0], vWall[1]);
	point2 = vec2(0, 0.5);
	id1 = rc->addObject(point1.x, point1.y);
	id2 = rc->addObject(point2.x, point2.y);

	time = glfwGetTime();
	frameNum = 0; 
	//画圆点
	glEnable(GL_POINT_SMOOTH);
}
//主循环
void mainloop(GLFWwindow* window) {
	GLfloat tt = glfwGetTime()/3;
	point1.x = sin(tt)*0.7f;//Mymassage::xpos;
	point1.y = sin(2*tt)*0.7f;//Mymassage::ypos;

	mat4 rm = rotate(mat4(1.0f), 0.006f, vec3(0, 0, 1));
	vec4 p(point2.x, point2.y, 0, 0);
	p = p*rm;
	point2.x = p.x;
	point2.y = p.y;

	GLfloat now = glfwGetTime();
	GLfloat delta = now - time;
	time = now;
	delta *= 1000;
	rc->update(id1, point1.x, point1.y, delta);
	rc->update(id2, point2.x, point2.y, delta);

	//计算速度
	speed1 = rc->getSpeed(id1);
	speed2 = rc->getSpeed(id2);
	//计算角度
	theta1 = rc->getAngle(id1, delta);
	theta2 = rc->getAngle(id2, delta);
	//计算力
	force1 = rc->getForce(id1, delta);
	force2 = rc->getForce(id2, delta);

	//if (frameNum < 5)
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, Mymassage::width, Mymassage::height);
	draw_scene();
	//glViewport(Mymassage::width, 0, Mymassage::width, Mymassage::height);
	//draw_scene();
	//else if(frameNum%12==0)
	//	draw_scene_no_flush(window);
	frameNum++;
}
void init_opengl(void)
{
	glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading  
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background  
	init();
}
int main(int argc, char *argv[])
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
	//initialize opengl  
	init_opengl();

	while (!glfwWindowShouldClose(window))
	{
		mainloop(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}