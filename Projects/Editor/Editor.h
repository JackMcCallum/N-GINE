#ifndef _NGINE_EDITOR_H_
#define _NGINE_EDITOR_H_

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#define LAYOUT_FILE "Editor.layout"

#define ngEditor Editor::getSingleton()

class Viewport;

namespace NGine
{
	class Main;
	class QtRenderWindow;
	class Gizmos;
	class Camera;
	class SceneNode;
}

class Editor : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT;

public:
	Editor(QWidget *parent = 0);
	~Editor();

	void setupStyle();
	void saveLayout();
	void restoreLayout();

	static Editor& getSingleton();

private:
	// NGine
	Viewport* mViewport;
	NGine::Main* mNGineMain;
	NGine::ConsoleLogger mConsoleLogger;

	static Editor* sSinglton;
};


#endif // _NGINE_EDITOR_H_
