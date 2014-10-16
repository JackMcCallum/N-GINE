#include "stdafx.h"
#include "Editor.h"
#include "qgl.h"
#include <iostream>

#include <NGineResourceManager.h>
#include <NGineMain.h>
#include "Viewport.h"

Editor* Editor::sSinglton;
Editor& Editor::getSingleton()
{
	return *sSinglton;
}

Editor::Editor(QWidget *parent)
: QMainWindow(parent)
{
	sSinglton = this;

	QSplashScreen splash(QPixmap("../Data/N-GINE.png"));
	splash.show();

	setupStyle();
	setupUi(this);
	restoreLayout();

	((QMainWindow*)this)->centralWidget()->setVisible(false);

	// Create the window
	QGLFormat format;
	format.setProfile(QGLFormat::CompatibilityProfile);
	format.setVersion(3, 3);
	format.setDoubleBuffer(true);
	mViewport = new Viewport(sceneView, format);

	// Setup the layout
	QHBoxLayout* layout = new QHBoxLayout;
	layout->setMargin(2);
	layout->setSpacing(0);
	layout->addWidget(mViewport);
	sceneView->setLayout(layout);

	// Give it a blue border
	sceneView->setStyleSheet("background-color: palette(Button); padding: 2px;");

	// Create and initialize NGine
	mNGineMain = new NGine::Main(mViewport);
	if (!mNGineMain->initialize()) QApplication::quit();
	mNGineMain->getResourceManager().addResourceLocation("../Data");
	
	mViewport->onCreate();

	// Starts the update loop ticker
	mViewport->startTicker();

	NGine::Log::message("Loging a message!");
	NGine::Log::warning("Loging a warning!");
	NGine::Log::error("Loging an error!");


	show();
}

Editor::~Editor()
{
	mViewport->onDestroy();

	// Save the layout so it is remembered next time its opened
	saveLayout();

	delete mNGineMain;
}

void Editor::setupStyle()
{
	// Manage the style
	qApp->setStyle(QStyleFactory::create("fusion"));

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(32, 32, 32));
	palette.setColor(QPalette::AlternateBase, QColor(40, 40, 40));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Highlight, QColor(97, 161, 248));
	palette.setColor(QPalette::HighlightedText, Qt::black);

	qApp->setPalette(palette);

	QString style =
		//"QMainWindow::separator { background-color: palette(Base); margin: 0px; height: 3px; }"
		//"QMainWindow::separator::hover { background-color: palette(Highlight); }"
		//
		//"QSplitter::separator { background-color: palette(Base); margin: 0px; height: 3px; }"
		//"QSplitter::separator::hover { background-color: palette(Highlight); }"
		
		"QDockWidget::title { background-color: palette(Base); padding: 2px 2px 2px 8px; }"
		;

	this->setStyleSheet(style);

}

void Editor::saveLayout()
{
	// Save layout
	QSettings settings(LAYOUT_FILE, QSettings::IniFormat, this);
	settings.beginGroup("mainWindow");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
}

void Editor::restoreLayout()
{
	// Restore layout
	QSettings settings(LAYOUT_FILE, QSettings::IniFormat, this);
	settings.beginGroup("mainWindow");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
}
