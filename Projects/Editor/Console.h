#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "NGineLog.h"

class ConsoleItem : public QWidget
{
public:
	enum Type
	{
		T_ERROR,
		T_WARNING,
		T_MESSAGE,
	};

public:
	ConsoleItem(QWidget* parent, const QString& message, Type type);
	~ConsoleItem();

	virtual void paintEvent(QPaintEvent *);

	virtual void resizeEvent(QResizeEvent *);

	QString mMessage;
	Type mType;
};

class Console : public QListWidget, public NGine::Logger
{
public:
	Console(QWidget* parent = nullptr);
	~Console();

	virtual void message(const char* str);
	virtual void warning(const char* str);
	virtual void error(const char* str);

private:
	QIcon mIconMessage;
	QIcon mIconWarning;
	QIcon mIconError;



};

#endif