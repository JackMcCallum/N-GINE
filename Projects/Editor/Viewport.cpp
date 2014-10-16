#include "stdafx.h"
#include "Viewport.h"
#include <NGineCamera.h>
#include <NGineResourceManager.h>
#include <NGineMain.h>
#include <NGineMaterial.h>
#include <NGineGizmos.h>

Viewport::Viewport(QWidget* parent, QGLFormat format) :
QtRenderWindow(parent, format)
{
	setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	setFocus();

	mControllingCamera = false;
	mControlData = 0;
}

Viewport::~Viewport()
{
}

void Viewport::onCreate()
{
	mRootNode = new NGine::SceneNode("Root Node");

	// Setup Gizmos
	NGine::MaterialPtr material = ngResourceMgr.createMaterial("GizmosMaterial");
	material->setShader("S_Gizmos.shader");
	mGizmos = new NGine::Gizmos(material);
	mRootNode->attachComponent(mGizmos);

	material = ngResourceMgr.createMaterial("WorldPosMaterial");
	material->setShader("S_WorldPos.shader");

	// Setup camera
	mCamera = new NGine::Camera();
	mCameraNode = mRootNode->createChild("Camera");
	mCameraNode->attachComponent(mCamera);
	this->setCamera(mCamera);


}

void Viewport::onUpdate()
{
	float speed = 0.1f;
	if (mControlData & 32) speed = 0.6f;
	if (mControlData & 1) mCamVelocity += mCameraNode->getOrientation() * glm::vec3(0, 0, -speed);
	if (mControlData & 2) mCamVelocity += mCameraNode->getOrientation() * glm::vec3(-speed, 0, 0);
	if (mControlData & 4) mCamVelocity += mCameraNode->getOrientation() * glm::vec3(0, 0, speed);
	if (mControlData & 8) mCamVelocity += mCameraNode->getOrientation() * glm::vec3(speed, 0, 0);
	if (glm::length(mCamVelocity) > 0) notifyNeedsUpdate();
	mCameraNode->translate(mCamVelocity, NGine::Space::PARENT);
	mCamVelocity *= 0.5f;

	mGizmos->clear();

	mRootNode->recursivly([this](NGine::SceneNode* node)
	{
		mGizmos->addTransform(node->getGlobalTransform());
		return true;
	});

	mGizmos->addSphere(glm::vec3(0), 1, glm::vec3(1, 0, 0));

	this->updateTarget();

	ngRenderSys.setWorldMatrix(glm::mat4(), 0);
	ngRenderSys.renderSphere(ngResourceMgr.getMaterial("WorldPosMaterial"));

}

void Viewport::onDestroy()
{
	delete mRootNode;
}

void Viewport::mousePressEvent(QMouseEvent* evt)
{
	mLastMousePos.x = evt->globalX();
	mLastMousePos.y = evt->globalY();
	mControlData |= 16;
}

void Viewport::mouseReleaseEvent(QMouseEvent* evt)
{
	mControlData &= ~16;
}

void Viewport::mouseDoubleClickEvent(QMouseEvent* evt)
{
	
}

void Viewport::mouseMoveEvent(QMouseEvent* evt)
{
	if (mControlData & 16)
	{
		mCameraNode->yaw(glm::radians(-(evt->globalX() - mLastMousePos.x)*0.2f), NGine::Space::WORLD);
		mCameraNode->pitch(glm::radians(-(evt->globalY() - mLastMousePos.y)*0.2f), NGine::Space::LOCAL);

		mLastMousePos.x = evt->globalX();
		mLastMousePos.y = evt->globalY();

		notifyNeedsUpdate();
	}
}

void Viewport::wheelEvent(QWheelEvent* evt)
{
	
}

void Viewport::keyPressEvent(QKeyEvent* evt)
{
	if (evt->isAutoRepeat()) return;

	else if (evt->key() == Qt::Key_W) mControlData |= 1;
	else if (evt->key() == Qt::Key_A) mControlData |= 2;
	else if (evt->key() == Qt::Key_S) mControlData |= 4;
	else if (evt->key() == Qt::Key_D) mControlData |= 8;
	else if (evt->key() == Qt::Key_Shift) mControlData |= 32;
	notifyNeedsUpdate();
}

void Viewport::keyReleaseEvent(QKeyEvent* evt)
{
	if (evt->isAutoRepeat()) return;

	else if (evt->key() == Qt::Key_W) mControlData &= ~1;
	else if (evt->key() == Qt::Key_A) mControlData &= ~2;
	else if (evt->key() == Qt::Key_S) mControlData &= ~4;
	else if (evt->key() == Qt::Key_D) mControlData &= ~8;
	else if (evt->key() == Qt::Key_Shift) mControlData &= ~32;
}

void Viewport::focusInEvent(QFocusEvent* evt)
{
	if (parent()) ((QWidget*)parent())->setStyleSheet("background-color: palette(highlight); padding: 2px;");
}

void Viewport::focusOutEvent(QFocusEvent* evt)
{
	if (parent()) ((QWidget*)parent())->setStyleSheet("background-color: #acacac; padding: 2px;");
}

void Viewport::resizeEvent(QResizeEvent* evt)
{
	this->setDimensions(width(), height());
	mCamera->setProjection(50, float(width()) / float(height()));

}

void Viewport::paintEvent(QPaintEvent* evt)
{
	onUpdate();
	ngRenderSys.swapBuffers();
	ngRenderSys.flushQueue();
}
