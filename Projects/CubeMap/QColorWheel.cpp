#include "stdafx.h"
#include "QColorWheel.h"


QColorWheel::QColorWheel(QWidget* parent) :
QWidget(parent)
{
}


QColorWheel::~QColorWheel()
{
}

void QColorWheel::paintEvent(QPaintEvent* evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawEllipse(mArea);

}

void QColorWheel::resizeEvent(QResizeEvent* evt)
{
	if (evt->size().width() > evt->size().height())
	{
		mArea.setLeft((evt->size().width() - evt->size().height()) / 2 + 1);
		mArea.setTop(1);
		mArea.setWidth(evt->size().height() - 2);
		mArea.setHeight(evt->size().height() - 2);
	}
	else
	{
		mArea.setTop((evt->size().height() - evt->size().width()) / 2 + 1);
		mArea.setLeft(1);
		mArea.setWidth(evt->size().width() - 2);
		mArea.setHeight(evt->size().width() - 2);
	}
}

void QColorWheel::mousePressEvent(QMouseEvent* evt)
{
	
}

void QColorWheel::mouseReleaseEvent(QMouseEvent* evt)
{
	
}

void QColorWheel::mouseMoveEvent(QMouseEvent* evt)
{
	
}
