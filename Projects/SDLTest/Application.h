#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <NGineGLMesh.h>
#include <NGineGLShader.h>
#include <NGineGLTexture2D.h>
#include <NGineGLFrameBuffer.h>

#include <NGineSDLRenderWindow.h>
#include <NGineCamera.h>
#include <NGineMain.h>
#include <NGineGizmos.h>
#include <NGineMesh.h>
#include <NGineShader.h>
#include <NGineLight.h>

#include <NGineXInputPoller.h>
#include <NGineDeferredRenderTarget.h>
#include <NGineResourceManager.h>

class Application : public NGine::SDLRenderWindow::Listener
{
public:
	int run();

	void onCreate();
	void onUpdate();
	void onDestroy();

	float deltaTime();

	virtual void onMouseEvent(const NGine::MouseEvent& evt);
	virtual void onKeyboardEvent(const NGine::KeyboardEvent& evt);
	virtual void onWindowEvent(const NGine::WindowEvent& evt);

	NGine::RenderWindow* mWindow;


	// Back buffer scene
	NGine::SceneNode* mBBRootNode;
	NGine::SceneNode* mBBCameraNode;
	NGine::MeshRenderer* mBBQuadRenderer;
	NGine::Camera* mBBCamera;

	// Main scene
	NGine::SceneNode* mRootNode;
	NGine::SceneNode* mCameraNode;
	NGine::SceneNode* mTestNode;

	// Components
	NGine::MeshRenderer* mMeshRenderer;
	NGine::Gizmos* mGizmos;
	NGine::Camera* mCamera;
	NGine::Camera* mSecondCamera;
	NGine::SceneNode* mSecondCameraNode;
	bool mControllingCamera;


	uint8 mControlData; // W,A,S,D,CAM,BOOST,0,0
	glm::vec3 mCamVelocity;

	NGine::XInputPoller mXinputPoller;
	NGine::XInputPoller::State mPrevState;
	NGine::XInputPoller::State mState;

	NGine::SDLRenderWindow* mRenderWindow;
	NGine::DeferredRenderTarget* mDeferredRenderTarget;

};

#endif