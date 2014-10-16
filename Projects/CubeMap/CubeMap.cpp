#include "stdafx.h"
#include "CubeMap.h"

CubeMap::CubeMap(QWidget *parent)
	: QMainWindow(parent)
{
	this->setupUi(this);
	viewPort->mSelectedImageLabels[0] = selectedLabel0;
	viewPort->mSelectedImageLabels[1] = selectedLabel1;
	viewPort->mSelectedImageLabels[2] = selectedLabel2;
	viewPort->mSelectedImageLabels[3] = selectedLabel3;
	viewPort->mSelectedImageLabels[4] = selectedLabel4;
	viewPort->mSelectedImageLabels[5] = selectedLabel5;


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
}

CubeMap::~CubeMap()
{

}
