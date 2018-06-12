#include "2dScene.h"



vec2 map2d::vWall[V_WALL_NUM * 2] = {
	vec2(0.9f,-2.9f),	vec2(-0.8f,-2.9f),
	vec2(-0.8f,-2.9f),	vec2(-0.8f,-5.8f),
	vec2(0.9f,-5.8f),	vec2(0.9f,-2.9f),
	vec2(-0.8f,-5.8f),	vec2(0.9f,-5.8f),

	vec2(2.9f,-2.9f),	vec2(4.6f,-2.9f),
	vec2(4.6f,-2.9f),	vec2(4.6f,-5.8f),
	vec2(2.9f,-5.8f),	vec2(2.9f,-2.9f),
	vec2(4.6f,-5.8f),	vec2(2.9f,-5.8f),

	vec2(2.9f,-0.9f),	vec2(4.6f,-0.9f),
	vec2(4.6f,-0.9f),	vec2(4.6f,2.0f),
	vec2(2.9f,2.0f),	vec2(4.6f,2.0f),
	vec2(2.9f,2.0f),	vec2(2.9f,-0.9f)
};


void map2d::drawPoint(float x, float y, float r) {
	if (r < 10)r = 22.5f;
	glPointSize(r);
	glBegin(GL_POINTS);

	glColor3f(1.0, 0.0, 0.0);    // Red  
	glVertex2f(x, -y);
	glEnd();
}
void map2d::drawLinv(vec2 p1, vec2 p2, vec3 color)
{
	p1.y *= -1;
	p2.y *= -1;
	glLineWidth(4);//设置线段宽度  
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex2f(p1.x, p1.y); //定点坐标范围  

	glVertex2f(p2.x, p2.y);
	glEnd();

	vec2 dir = p2 - p1;
	dir /= sqrt(dir.x*dir.x + dir.y*dir.y);
	vec2 left(-dir.y, dir.x), right(dir.y, -dir.x);
	left = p2 - ((dir - left*0.6f)*0.02f);
	right = p2 - ((dir - right*0.6f)*0.02f);
	glBegin(GL_TRIANGLES);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(p2.x + dir.x*0.02f, p2.y + dir.y*0.02f, 0.0);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(left.x, left.y, 0.0);

	glColor3f(color.x, color.y, color.z);
	glVertex3f(right.x, right.y, 0.0);
	glEnd();
}
void map2d::drawLine(vec2 p1, vec2 p2, vec3 color, float lineWidth) {
	glLineWidth(lineWidth);//设置线段宽度  
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex2f(p1.x, -p1.y); //定点坐标范围  

	glVertex2f(p2.x, -p2.y);
	glEnd();
}

void map2d::drawReal() {
	//网格
	drawLine(vec2(1, 0), vec2(-1, 0), vec3(1, 0.85, 0.85));
	drawLine(vec2(0, -1), vec2(0, 1), vec3(1, 0.85, 0.85));
	for (float i = -0.9; i < 0.99; i += 0.2) {
		if (fabs(fabs(i) - BOUNDARY)>0.01) {
			drawLine(vec2(1, i), vec2(-1, i), vec3(1, 1, 0.7));
			drawLine(vec2(i, -1), vec2(i, 1), vec3(1, 1, 0.7));
		}
	}
	//边界
	drawLine(vec2(BOUNDARY, BOUNDARY), vec2(-BOUNDARY, BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(-BOUNDARY, BOUNDARY), vec2(-BOUNDARY, -BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(-BOUNDARY, -BOUNDARY), vec2(BOUNDARY, -BOUNDARY), vec3(0.5, 0.5, 0.1));
	drawLine(vec2(BOUNDARY, -BOUNDARY), vec2(BOUNDARY, BOUNDARY), vec3(0.5, 0.5, 0.1));
}

void map2d::drawVirtul() {
	//网格
	drawLine(vec2(1, 0), vec2(-1, 0), vec3(1, 0.85, 0.85));
	drawLine(vec2(0, -1), vec2(0, 1), vec3(1, 0.85, 0.85));
	for (int i = 0; i < V_WALL_NUM; i++)
		drawLine((vWall[i * 2] + vec2(-1.9f, 1.9f)) / 4.0f , (vWall[i * 2 + 1] + vec2(-1.9f, 1.9f)) / 4.0f  , vec3(1, 0.3, 0.3));

	for (float i = -0.9; i < 0.99; i += 0.2) {
		if (fabs(fabs(i) - BOUNDARY)>0.01) {
			drawLine(vec2(1, i), vec2(-1, i), vec3(1, 1, 0.7));
			drawLine(vec2(i, -1), vec2(i, 1), vec3(1, 1, 0.7));
		}
	}
}