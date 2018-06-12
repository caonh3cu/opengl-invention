#include <Windows.h>

#include "VRDriver.h"

#if defined(POSIX)
#include "unistd.h"
#endif

#ifndef _WIN32
#define APIENTRY
#endif

#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif

void ThreadSleep(unsigned long nMilliseconds)
{
#if defined(_WIN32)
	::Sleep(nMilliseconds);
#elif defined(POSIX)
	usleep(nMilliseconds * 1000);
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Outputs a set of optional arguments to debugging output, using
//          the printf format setting specified in fmt*.
//-----------------------------------------------------------------------------
static bool g_bPrintf = true;
void dprintf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	if (g_bPrintf)
		printf("%s", buffer);

	OutputDebugStringA(buffer);
}



//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
VRDriver::VRDriver():
	HMDPose(1.0f),
	HMDPoseInverse(1.0f),
	m_mat4Contraller1Pose(1.0f),
	m_mat4Contraller2Pose(1.0f),
	controllerNum(0),
	m_pHMD(NULL),
	m_strPoseClasses("")
{
	// other initialization tasks are done in BInit
	memset(m_rDevClassChar, 0, sizeof(m_rDevClassChar));
};


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
VRDriver::~VRDriver()
{
	// work is done in Shutdown
	dprintf("Shutdown");
}


//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool VRDriver::Init()
{
	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		cout << "VR_Init Failed : " << buf << endl;
		return false;
	}


	

	m_strDriver = "No Driver";
	m_strDisplay = "No Display";


	if (eError == vr::VRInitError_None) {
		m_strDriver = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
		m_strDisplay = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);
	}

	if (!SetupStereoRenderTargets())
	{
		printf("%s - Unable to initialize Frame Buffer!\n", __FUNCTION__);
		return false;
	}

	if (!BInitCompositor())
	{
		printf("%s - Failed to initialize VR Compositor!\n", __FUNCTION__);
		return false;
	}
	press_button = false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Compositor. Returns true if the compositor was
//          successfully initialized, false otherwise.
//-----------------------------------------------------------------------------
bool VRDriver::BInitCompositor()
{
	vr::EVRInitError peError = vr::VRInitError_None;

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

	return true;
}



//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool VRDriver::SetupStereoRenderTargets()

{
	if (!m_pHMD)
		return false;
	m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

	if (
		!CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc) ||
		!CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc)
		) {
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
// Purpose: Creates a frame buffer. Returns true if the buffer was set up.
//          Returns false if the setup failed.
//-----------------------------------------------------------------------------
bool VRDriver::CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc)
{
	framebufferDesc.width = nWidth;
	framebufferDesc.height = nHeight;

	glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

	glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);

	glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

	glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

	glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}








void VRDriver::VRBegin(bool isLeftEye) {
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	if (isLeftEye) {
		glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	}
	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
}
void VRDriver::VREnd(bool isLeftEye) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_MULTISAMPLE);
	if (isLeftEye) {

		glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);

	}
	else {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);
	}
	glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void VRDriver::Submit() {

	if (m_pHMD)
	{

		vr::EVRCompositorError eError1 = vr::VRCompositorError_None, eError2 = vr::VRCompositorError_None;



		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		eError1 = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		eError2 = vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

		if (eError1 != vr::VRCompositorError_None&&eError2 != vr::VRCompositorError_None)
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "Unable to Submit runtime: %d , %d", eError1, eError2);
			cout << "VR_Submit Failed : " << buf << endl;
		}
	}

}
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool VRDriver::HandleInput()
{
	UpdateHMDMatrixPose();
	// Process SteamVR events
	vr::VREvent_t event;
	while (m_pHMD->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event);
	}

	// Process SteamVR controller state
	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
		vr::VRControllerState_t state;
		if (m_pHMD->GetControllerState(unDevice, &state, sizeof(state)))
		{
			m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
		}
	}	


	return false;
}
//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void VRDriver::ProcessVREvent(const vr::VREvent_t & event)
{
	switch (event.eventType)
	{
	case vr::VREvent_ButtonTouch: {
	}
	break;
	case vr::VREvent_ButtonUntouch: {
	}
	break;
	case vr::VREvent_ButtonPress:{
		if (event.data.controller.button == vr::k_EButton_Axis1) {
			press_button = true;
		}
	}
	break;
	case vr::VREvent_TrackedDeviceActivated:
	{
		dprintf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		dprintf("Device %u detached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		dprintf("Device %u updated.\n", event.trackedDeviceIndex);
	}
	break;
	}
}








//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void VRDriver::Shutdown()
{
	if (m_pHMD)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}




	if (leftEyeDesc.m_nDepthBufferId) {
		glDeleteRenderbuffers(1, &leftEyeDesc.m_nDepthBufferId);
		glDeleteTextures(1, &leftEyeDesc.m_nRenderTextureId);
		glDeleteFramebuffers(1, &leftEyeDesc.m_nRenderFramebufferId);
		glDeleteTextures(1, &leftEyeDesc.m_nResolveTextureId);
		glDeleteFramebuffers(1, &leftEyeDesc.m_nResolveFramebufferId);

		glDeleteRenderbuffers(1, &rightEyeDesc.m_nDepthBufferId);
		glDeleteTextures(1, &rightEyeDesc.m_nRenderTextureId);
		glDeleteFramebuffers(1, &rightEyeDesc.m_nRenderFramebufferId);
		glDeleteTextures(1, &rightEyeDesc.m_nResolveTextureId);
		glDeleteFramebuffers(1, &rightEyeDesc.m_nResolveFramebufferId);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Gets a Matrix Projection Eye with respect to nEye.
//-----------------------------------------------------------------------------
mat4 VRDriver::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye,float nearClip,float farClip)
{
	if (!m_pHMD)
		return mat4();

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(nEye, nearClip, farClip);

	return mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}


//-----------------------------------------------------------------------------
// Purpose: Gets an HMDMatrixPoseEye with respect to nEye.
//-----------------------------------------------------------------------------
mat4 VRDriver::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return mat4();

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(nEye);
	mat4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return inverse(matrixObj);
}



//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void VRDriver::UpdateHMDMatrixPose()
{
	if (!m_pHMD)
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	m_strPoseClasses = "";
	int contrallerCount = 0;
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_pHMD->GetTrackedDeviceClass(nDevice) == vr::TrackedDeviceClass_Controller) {
				contrallerCount == 0 ? m_mat4Contraller1Pose = m_rmat4DevicePose[nDevice] : m_mat4Contraller2Pose = m_rmat4DevicePose[nDevice];
				contrallerCount++;
			}
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}
	controllerNum = contrallerCount;
	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		HMDPoseInverse = inverse(HMDPose);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
mat4 VRDriver::ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	mat4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}




