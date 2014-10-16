#include "stdafx.h"
#include "Console.h"
#include "NGineResourceManager.h"
#include "NGineMain.h"

ConsoleItem::ConsoleItem(QWidget* parent, const QString& message, Type type) :
QWidget(parent),
mMessage(message),
mType(type)
{
	
}

ConsoleItem::~ConsoleItem()
{
}

void ConsoleItem::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawText(0, 0, mMessage);
	painter.drawRect(geometry());
}

void ConsoleItem::resizeEvent(QResizeEvent *)
{
	QFontMetrics metrics(QApplication::font());
	QRect bounds = metrics.boundingRect(mMessage);

	int height = bounds.height();
	setMinimumHeight(32);
	setMaximumHeight(32);
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	repaint();
}




Console::Console(QWidget* parent) : 
QListWidget(parent),
mIconMessage(":icon_message.png"),
mIconWarning(":icon_warning.png"),
mIconError(":icon_error.png")
{
	ngLog->registerLogger(this);
	setLayout(new QVBoxLayout());
	ConsoleItem* item = new ConsoleItem(this, "Hello World!", ConsoleItem::T_MESSAGE);
	layout()->addWidget(item);

	setAlternatingRowColors(true);
}


Console::~Console()
{
	ngLog->unregisterLogger(this);
}

void Console::message(const char* str)
{
	QListWidgetItem* item = new QListWidgetItem(mIconMessage, str, this);
	this->setCurrentItem(item);
}

void Console::warning(const char* str)
{
	QListWidgetItem* item = new QListWidgetItem(mIconWarning, str, this);
	this->setCurrentItem(item);
}

void Console::error(const char* str)
{
	QListWidgetItem* item = new QListWidgetItem(mIconError, str, this);
	this->setCurrentItem(item);
}


