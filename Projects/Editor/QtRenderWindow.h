#ifndef __NGINE_QT_RENDER_WINDOW_H_
#define __NGINE_QT_RENDER_WINDOW_H_

#include <QGLWidget>
#include <NGineRenderWindow.h>

class QtRenderWindow : public QGLWidget, public NGine::RenderWindow
{
	Q_OBJECT;

public:
	QtRenderWindow(QWidget* parent = nullptr, QGLFormat format = QGLFormat());
	virtual ~QtRenderWindow();

	virtual void onMakeCurrent();
	virtual void onDoneCurrent();
	virtual void onSwapBuffers();

	virtual void glInit() {}
	virtual void glDraw() {}

	void notifyNeedsUpdate();
	void startTicker();

public slots:
	void updateView();
	void setRealtime(bool realtime);

private:
	HDC deviceContext;
	HGLRC glContext;

	QTimer mTimer;
	bool mNeedsUpdate;
	bool mRealtime;
};

#endif // !__NGINE_QT_RENDER_WINDOW_H_
