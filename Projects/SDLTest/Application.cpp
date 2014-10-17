#include "StdAfx.h"
#include "Application.h"
#include <NGineRenderCommand.h>

#include <glm/ext.hpp>
#include <glew/glew.h>
#include <gl/GL.h>
#include <Xinput/XInput.h>

//#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
//#include "C:/Program Files (x86)/Visual Leak Detector/include/vld.h"

void Application::onCreate()
{
	mControllingCamera = 0;
	mControlData = 0;

	ngResourceMgr.setDefaultTexture("Grid.png");
	
	NGine::SceneNode* tmpNode;
	NGine::MaterialPtr material;

	mDeferredRenderTarget = new NGine::DeferredRenderTarget(1280,720);

	// Load shaders
	material = ngResourceMgr.createMaterial("JM_MatPrev_First");
	material->setShader("S_Fresnel.shader");
	material->setDiffuse(glm::vec4(1,0.5,0.5, 0.5));
	material->setBlendMode(NGine::BM_MODULATIVE);
	material->setDepthMode(true, false);
	material->setTexture("T_MacroVariation.tga", 0);
	material->setTexture("T_Metal_Copper_D.tga", 1);
	material->setTexture("T_Metal_Gold_N.tga", 2);

	material = ngResourceMgr.createMaterial("JM_MatPrev_Second");
	material->setShader("S_Test.shader");
	material->setDiffuse(glm::vec4(0.7, 0.8, 0.9, 1));
	material->setTexture("T_MacroVariation.tga", 0);

	material = ngResourceMgr.createMaterial("JM_MatPrev_Text");
	material->setShader("S_Test.shader");
	material->setDiffuse(glm::vec4(0.9, 1, 0.8, 1));
	material->setTexture("T_MacroVariation.tga", 0);



	material = ngResourceMgr.createMaterial("SkyDome");
	material->setShader("S_Skydome.shader");
	material->setTexture("T_Skydome.png", 0);
	material->setCullMode(NGine::CM_DISABLED);

	material = ngResourceMgr.createMaterial("WorldPosMaterial");
	material->setShader("S_WorldPos.shader");
	material->setCullMode(NGine::CM_DISABLED);


	material = ngResourceMgr.createMaterial("TestMaterial");
	material->setShader("S_Test.shader");

	material = ngResourceMgr.createMaterial("Undefined");
	material->setShader("S_Test.shader");

	material = ngResourceMgr.createMaterial("GizmosMaterial");
	material->setShader("S_Gizmos.shader");
	mGizmos = new NGine::Gizmos(material);

	material = ngResourceMgr.createMaterial("Default");
	material->setShader("S_QuadShader.shader");
	material->setDepthMode(false, false);
	material->setTexture("DRT_HDRTexture", 0);

	mRootNode = new NGine::SceneNode("RootNode");
	mRootNode->attachComponent(mGizmos);
	mRootNode->attachComponent(new NGine::DirectionalLight(glm::vec3(1,1,1)));
	//NGine::DirectionalLight* dirLight = new NGine::DirectionalLight(glm::vec3(-1));
	//dirLight->setColor(glm::vec3(0.4, 0.4, 0.4) * 2.0f);
	//mRootNode->attachComponent(dirLight);
	mRootNode->attachComponent(new NGine::PointLight());

	mCamera = new NGine::Camera();
	mCamera->setProjection(60, 1280.0f / 720.0f, 0.01f, 256);
	mCameraNode = mRootNode->createChild("CameraNode");
	mCameraNode->attachComponent(mCamera);
	mCameraNode->attachComponent(new NGine::MeshRenderer("Camera.obj", "JM_MatPrev_First"));
	mRenderWindow->setCamera(mCamera);

	tmpNode = mRootNode->createChild("SkyNode");
	tmpNode->attachComponent(new NGine::MeshRenderer("M_Primitive_IcoSphere3.obj", "SkyDome"));
	tmpNode->setScale(glm::vec3(200));


	material = ngResourceMgr.createMaterial("FrontCol");
	material->setShader("S_Test.shader");
	material->setDiffuse(glm::vec4(1, 1, 1, 0.5));
	//material->setBlendMode(NGine::BM_ALPHA_BLEND);
	//material->setDepthMode(true, false);
	material->setTexture("T_MacroVariation.tga", 0);
	material->setTexture("T_Metal_Copper_D.tga", 1);
	material->setTexture("T_Metal_Gold_N.tga", 2);
	material->setTexture("T_Metal_Gold_N.tga", 2);
	tmpNode = mRootNode->createChild("Level");
	tmpNode->attachComponent(new NGine::MeshRenderer("M_Level_Test.obj", "FrontCol"));
	

	mSecondCamera = new NGine::Camera();
	mSecondCamera->setProjection(60, 1280.0f / 720.0f, 0.01f, 256);


	int s = 10;
	mGizmos->addLine(glm::vec3(0), glm::vec3(0, 1, 0), glm::vec3(0.4, 1, 0.4));
	for (int i = -s; i <= s; i++)
	{
		if (i == 0)
		{
			mGizmos->addLine(glm::vec3(-s, 0, i), glm::vec3(s, 0, i), glm::vec3(1, 0.4, 0.4));
			mGizmos->addLine(glm::vec3(i, 0, -s), glm::vec3(i, 0, s), glm::vec3(0.4, 0.4, 1));
		}
		else
		{
			mGizmos->addLine(glm::vec3(-s, 0, i), glm::vec3(s, 0, i), glm::vec3(0.4, 0.4, 0.4));
			mGizmos->addLine(glm::vec3(i, 0, -s), glm::vec3(i, 0, s), glm::vec3(0.4, 0.4, 0.4));
		}
	}


	for (int i = 0; i < 20; i++)
	{
		mTestNode = mRootNode->createChild("GizmosNode");
		mTestNode->setPosition(glm::vec3((rand() % 1000 - 500) / 30.0f, (rand() % 1000 - 500) / 100.0f, (rand() % 1000 - 500) / 30.0f));

		if (rand()%3 != 0)
		{
			NGine::PointLight* light = new NGine::PointLight(5);
			light->setColor(glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f) * 2.0f);
			mTestNode->attachComponent(light);
			mTestNode->attachComponent(new NGine::SphereRenderer("JM_MatPrev_Second"));
			mTestNode->setScale(glm::vec3(0.1));
			mGizmos->addSphere(mTestNode->getGlobalPosition(), light->getAttenuation(), light->getColor());
		}
		else
		{
			mMeshRenderer = new NGine::MeshRenderer("JM_MaterialPreview_01.obj");
			mTestNode->attachComponent(mMeshRenderer);
		}

	}

	mSecondCameraNode = mRootNode->createChild("");
	mSecondCameraNode->attachComponent(mSecondCamera);
	mSecondCameraNode->attachComponent(new NGine::MeshRenderer("Camera.obj", "TestMaterial"));
	
}

void Application::onUpdate()
{
	NGine::SceneNode* cam = mControllingCamera ? mCameraNode : mSecondCameraNode;

	float speed = 0.1f;
	if (mControlData & 32) speed = 0.6f;
	if (mControlData & 1) mCamVelocity += cam->getOrientation() * glm::vec3(0, 0, -speed);
	if (mControlData & 2) mCamVelocity += cam->getOrientation() * glm::vec3(-speed, 0, 0);
	if (mControlData & 4) mCamVelocity += cam->getOrientation() * glm::vec3(0, 0, speed);
	if (mControlData & 8) mCamVelocity += cam->getOrientation() * glm::vec3(speed, 0, 0);
	cam->translate(mCamVelocity, NGine::Space::PARENT);
	mCamVelocity *= 0.5f;

	//mRootNode->recursivly([this](NGine::SceneNode* node)
	//{
	//	if (node != mRootNode)
	//	{
	//		mGizmos->addTransform(node->getGlobalTransform());
	//	}
	//	return true;
	//});



	//mRenderWindow->setCamera(mBBCamera);
	mDeferredRenderTarget->setCamera(mControllingCamera ? mCamera : mSecondCamera);
	mDeferredRenderTarget->updateTarget();
	//mRenderWindow->update();

	mRenderWindow->setActive();
	ngResourceMgr.getMaterial("Default")->setActive();
	ngResourceMgr.renderQuad();


	mPrevState = mState;
	mState = mXinputPoller.pollState(0);
}

void Application::onDestroy()
{
	delete mRootNode;
	delete mDeferredRenderTarget;
}

void Application::onMouseEvent(const NGine::MouseEvent& evt)
{
	switch (evt.type)
	{
	case NGine::MouseEvent::MOTION:
		if (mControlData & 16)
		{
			NGine::SceneNode* cam = mControllingCamera ? mCameraNode : mSecondCameraNode;
			cam->yaw(glm::radians(-evt.relative.x*0.2f), NGine::Space::WORLD);
			cam->pitch(glm::radians(-evt.relative.y*0.2f), NGine::Space::LOCAL);
		}
		break;
	case NGine::MouseEvent::PRESS:
		if (evt.button == SDL_BUTTON_RIGHT) mControlData |= 16;
		break;
	case NGine::MouseEvent::RELEASE:
		if (evt.button == SDL_BUTTON_RIGHT) mControlData &= ~16;
		break;
	}
}

void Application::onKeyboardEvent(const NGine::KeyboardEvent& evt)
{
	switch (evt.type)
	{
	case NGine::KeyboardEvent::PRESS:
		if (evt.repeat) return;
		else if (evt.keycode == SDLK_w) mControlData |= 1;
		else if (evt.keycode == SDLK_a) mControlData |= 2;
		else if (evt.keycode == SDLK_s) mControlData |= 4;
		else if (evt.keycode == SDLK_d) mControlData |= 8;
		else if (evt.keycode == SDLK_LSHIFT) mControlData |= 32;

		if (evt.keycode == SDLK_SPACE) mControllingCamera = !mControllingCamera;
		break;
	case NGine::KeyboardEvent::RELEASE:
		if (evt.repeat) return;
		else if (evt.keycode == SDLK_w) mControlData &= ~1;
		else if (evt.keycode == SDLK_a) mControlData &= ~2;
		else if (evt.keycode == SDLK_s) mControlData &= ~4;
		else if (evt.keycode == SDLK_d) mControlData &= ~8;
		else if (evt.keycode == SDLK_LSHIFT) mControlData &= ~32;
		break;
	}
}

void Application::onWindowEvent(const NGine::WindowEvent& evt)
{
	
}

float Application::deltaTime()
{
	return 1.0f / 60.0f;
}

int Application::run()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL failed to initialize" << std::endl;
		return -1;
	}

	NGine::ConsoleLogger logger;

	mRenderWindow = new NGine::SDLRenderWindow(1280, 720, true , 0);
	NGine::Main* main = new NGine::Main(mRenderWindow);
	main->initialize();
	main->getResourceManager().addResourceLocation("../Data");

	mRenderWindow->addListener(this);
	onCreate();

	while (!mRenderWindow->wasClosed())
	{
		onUpdate();
		mRenderWindow->pollEvents();
		main->getRenderSystem().swapBuffers();
		main->getRenderSystem().flushQueue();
	}

	onDestroy();
	mRenderWindow->removeListener(this);

	delete main;
	SDL_Quit();

	return true;
}
