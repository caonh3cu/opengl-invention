#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> // Contains all the necessery OpenGL includes

#include <SOIL/SOIL.h>

#include <iostream>
#include <string>
using namespace std;

//帧缓存类，用长宽初始化，建立一个framebuffer
//createNormal()将framebuffer关联到3个renderbuffer，color、depth、stencil各一个，并申请相应大小内存空间用于保存颜色帧数据
//createDepthTexture()是将framebuffer关联到一个深度的贴图，需在createNormal()调用之前调用才好使
//readColor()可将帧缓存内容存入声明的数组imageData里面，saveToFile(int FrameNumber)是把imageData里面数据保存成图片
///根据createDepthTexture()类似的可以编写关联颜色贴图，这里没有具体实现，实际可以根据情况改写
class MyFramebuffer {
public:
	int width;
	int height;
	MyFramebuffer(int nWidth, int nHeight) {
		width = nWidth;
		height = nHeight;

		glGenFramebuffers(1, &FramebufferId);
		ColorBufferId = DepthBufferId = StencilBufferId = 0;
		imageData = new unsigned char[4 * height*width];
	}
	~MyFramebuffer() {
		delete[] imageData;
		if(ColorBufferId!=0)
			glDeleteBuffers(1, &ColorBufferId);
		if (DepthBufferId != 0)
			glDeleteBuffers(1, &DepthBufferId);
		if (StencilBufferId != 0)
			glDeleteBuffers(1, &StencilBufferId);
	}
	void bind() { glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId); }
	void unBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	//全创建为renderbuffer,id为0才会创建对应对象
	bool createNormal();
	bool createDepthTexture();

	//默认Stencil是renderbuffer
	GLuint DepthBufferId;
	bool isDepthTexture;
	GLuint ColorBufferId;
	bool isColorTexture;
	GLuint StencilBufferId;

	GLuint FramebufferId;

	unsigned char* imageData;
	void readColor();

	void saveToFile(int FrameNumber);

};
