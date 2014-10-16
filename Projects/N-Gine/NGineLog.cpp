#include "NGineStable.h"
#include "NGineLog.h"

#include <fstream>
#include <iostream>

namespace NGine
{

	Log::~Log()
	{
		for (auto iter : mLoggers)
			delete iter;
	}

	void Log::message(const char* str, ...)
	{
		va_list ap;
		va_start(ap, str);
		vsprintf(getSingleton()->mBuffer, str, ap);
		va_end(ap);

		for (auto iter : getSingleton()->mLoggers)
			iter->message(getSingleton()->mBuffer);
	}

	void Log::warning(const char* str, ...)
	{
		va_list ap;
		va_start(ap, str);
		vsprintf(getSingleton()->mBuffer, str, ap);
		va_end(ap);

		for (auto iter : getSingleton()->mLoggers)
			iter->warning(getSingleton()->mBuffer);
	}

	void Log::error(const char* str, ...)
	{
		va_list ap;
		va_start(ap, str);
		vsprintf(getSingleton()->mBuffer, str, ap);
		va_end(ap);

		for (auto iter : getSingleton()->mLoggers)
			iter->error(getSingleton()->mBuffer);
	}

	Log* Log::getSingleton()
	{
		static Log log;
		return &log;
	}

	void Log::registerLogger(Logger* logger)
	{
		getSingleton()->mLoggers.push_back(logger);
	}

	void Log::unregisterLogger(Logger* logger)
	{
		auto i = std::find(
			getSingleton()->mLoggers.begin(), 
			getSingleton()->mLoggers.end(), logger);
		getSingleton()->mLoggers.erase(i);
	}

	void ConsoleLogger::message(const char* str)
	{
		std::cout << str << std::endl;
	}

	void ConsoleLogger::warning(const char* str)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0E);
		std::cout << str << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}

	void ConsoleLogger::error(const char* str)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
		std::cout << str << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}

	ConsoleLogger::ConsoleLogger()
	{
		ngLog->registerLogger(this);
	}

	ConsoleLogger::~ConsoleLogger()
	{
		ngLog->unregisterLogger(this);
	}

}