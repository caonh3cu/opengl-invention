#include "MyFrame.h"


bool MyFramebuffer::createNormal() {
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId);

	if (ColorBufferId == 0) {
		glGenRenderbuffers(1, &ColorBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, ColorBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBufferId);
		isColorTexture = 0;
	}
	

	if (DepthBufferId == 0) {
		glGenRenderbuffers(1, &DepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBufferId);
		isDepthTexture = 0;
	}

	if (StencilBufferId == 0) {
		glGenRenderbuffers(1, &StencilBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, StencilBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, StencilBufferId);
	}


	// check FBO status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR::FRAMEBUFFER_NOT_COMPLETE " << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool MyFramebuffer::createDepthTexture() {
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId);

	glGenTextures(1, &DepthBufferId);
	glBindTexture(GL_TEXTURE_2D, DepthBufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	//¿ÉÑ¡
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthBufferId, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "ERROR::FRAMEBUFFER_NOT_COMPLETE " << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void MyFramebuffer::readColor() {
	if(imageData==NULL)
		imageData = new unsigned char[4 * height*width];
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyFramebuffer::saveToFile(int i) {
	char fname[100];
	sprintf_s(fname, "data/img/%d.bmp", i);
	SOIL_save_image(fname, SOIL_SAVE_TYPE_BMP, width, height, 4, imageData);
}