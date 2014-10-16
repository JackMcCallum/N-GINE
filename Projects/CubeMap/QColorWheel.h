#pragma once
#include <QWidget>

class QColorWheel : public QWidget
{
	Q_OBJECT;

public:
	QColorWheel(QWidget* parent = nullptr);
	~QColorWheel();

	virtual void paintEvent(QPaintEvent* evt);
	virtual void resizeEvent(QResizeEvent* evt);


	virtual void mousePressEvent(QMouseEvent* evt);
	virtual void mouseReleaseEvent(QMouseEvent* evt);
	virtual void mouseMoveEvent(QMouseEvent* evt);

	QRect mArea;

};

