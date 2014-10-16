#ifndef __VIEWPORT_H_
#define __VIEWPORT_H_

#include <QGLWidget>
#include "QtRenderWindow.h"
#include "NGineCommon.h"

class Viewport : public QtRenderWindow
{
	Q_OBJECT;

public:
	Viewport(QWidget* parent, QGLFormat format);
	virtual ~Viewport();

	void onCreate();
	void onUpdate();
	void onDestroy();

	// Mouse events
	virtual void mousePressEvent(QMouseEvent* evt);
	virtual void mouseReleaseEvent(QMouseEvent* evt);
	virtual void mouseDoubleClickEvent(QMouseEvent* evt);
	virtual void mouseMoveEvent(QMouseEvent* evt);
	virtual void wheelEvent(QWheelEvent* evt);

	// Keyboard events
	virtual void keyPressEvent(QKeyEvent* evt);
	virtual void keyReleaseEvent(QKeyEvent* evt);

	// Widget events
	virtual void focusInEvent(QFocusEvent* evt);
	virtual void focusOutEvent(QFocusEvent* evt);
	virtual void resizeEvent(QResizeEvent* evt);

	virtual void paintEvent(QPaintEvent* evt);



private:
	NGine::SceneNode* mRootNode;
	NGine::Gizmos* mGizmos;

	// Camera
	NGine::SceneNode* mCameraNode;
	NGine::Camera* mCamera;

	bool mControllingCamera;
	uint8 mControlData; // W,A,S,D,CAM,BOOST,0,0
	glm::vec3 mCamVelocity;
	glm::vec2 mLastMousePos;

};

#endif // !__VIEWPORT_H_
