#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <time.h>
using namespace std;

#ifndef GLEW_STATIC
#define GLEW_STATIC  
#endif // !GLEW_STATIC
#include <GL/glew.h> 

#include <GLFW/glfw3.h>  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

#include <openvr/openvr.h>

#include "CGLRenderModel.h"

//VRDriver类：就是驱动vr的，官方给的示例代码改的，简化了一下，手柄和定位器显示的部分我给删了
//这里我用的设备是vive，版本不记得了
//主要过程就是先init() //常规套路
//渲染的时候先调用 VRBegin(); 然后渲染;  然后调用VREnd(); 最后submit()
//实际上是绑定一个帧缓存然后渲染最后提交的过程，也可以直接获取帧缓存，但是这部分我还没写o(*￣3￣)o
//
//消息部分是调用HandleInput()来更新头盔和手柄位置，处理各种事件，这里我没写事件队列，
//需要亲自去ProcessVREvent()里改写事件处理过程
//
//另外HandleInput()和submit()两个函数的调用顺序steam有过建议，
//说要在提交后马上更新位置，还是在更新位置后马上提交，我忘了，也忘了在哪里看的了_(:3 | ∠)_
//
//这个代码已经测试过了，使用方法在另一个fork里面，我也不知道这样是否应该这样管理反正先这样吧。
class VRDriver
{
public:
	VRDriver();
	virtual ~VRDriver();

	bool Init(); 
	void Shutdown();

	//在VRBegin和VREnd之间绘图，已经设置好viewport和clearcolor，一对begin和end的左右眼要一致
	void VRBegin(bool isLeftEye);
	void VREnd(bool isLeftEye);
	//提交帧缓存到VR设备
	void Submit();
	//处理输入，要调用到ProcessVREvent
	bool HandleInput();

	//获取Projection Matrix和PoseEye Matrix，无法设置视角大小
	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye, float nearClip, float farClip);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);

private:
	void ProcessVREvent(const vr::VREvent_t & event);
	bool BInitCompositor();
	bool SetupStereoRenderTargets();

	void UpdateHMDMatrixPose();
	glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);

private:
	vr::IVRSystem *m_pHMD;

	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];
	std::string m_strDriver;
	std::string m_strDisplay;

	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class





public:
	//帧缓存
	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
		int width;
		int height;
	};
	//左右眼帧缓存
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;
	bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc);

	//头盔位置和手柄位置，手柄数量
	glm::mat4 HMDPose, HMDPoseInverse, m_mat4Contraller1Pose, m_mat4Contraller2Pose;
	int controllerNum;

	bool press_button;
};
