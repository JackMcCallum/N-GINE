#include "stdafx.h"
#include "QtRenderWindow.h"


QtRenderWindow::QtRenderWindow(QWidget* parent, QGLFormat format) : QGLWidget(format, parent)
{
	makeCurrent();
	deviceContext = wglGetCurrentDC();
	glContext = wglGetCurrentContext();
	onMakeCurrent();

	mRealtime = false;
	mNeedsUpdate = true;
}

QtRenderWindow::~QtRenderWindow()
{
}

void QtRenderWindow::onMakeCurrent()
{
	if (!wglMakeCurrent(deviceContext, glContext))
		NGine::Log::error("Failed to make current");
}

void QtRenderWindow::onDoneCurrent()
{
	if (!wglMakeCurrent(deviceContext, nullptr))
		NGine::Log::error("Failed to make current");
}

void QtRenderWindow::onSwapBuffers()
{
	SwapBuffers(deviceContext);
}

void QtRenderWindow::updateView()
{
	if (mNeedsUpdate || mRealtime)
	{
		mNeedsUpdate = false;
		repaint();
	}
}

void QtRenderWindow::notifyNeedsUpdate()
{
	mNeedsUpdate = true;
}

void QtRenderWindow::setRealtime(bool realtime)
{
	mRealtime = realtime;
}

void QtRenderWindow::startTicker()
{
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(updateView()));
	mTimer.setInterval(16); // 16ms / 60fps
	mTimer.start();
}
