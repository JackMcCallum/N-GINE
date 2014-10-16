#include "stdafx.h"
#include "CubeMap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CubeMap w;
	w.show();
	return a.exec();
}
