#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QMainWindow>
#include "ui_CubeMap.h"

class CubeMap : public QMainWindow, public Ui::CubeMapClass
{
	Q_OBJECT

public:
	CubeMap(QWidget *parent = 0);
	~CubeMap();

};

#endif // CUBEMAP_H
