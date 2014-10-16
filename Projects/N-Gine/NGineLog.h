/************************************************************
@author
Jack McCallum

@description
Basic static logging class designed to be accessed from anywhere in the code

By default the .log file will be outputted to the working directory unless changed
Also the default logging level is high unless changed
*/

#ifndef __NGINE_LOG_H_
#define __NGINE_LOG_H_

#include "NGineCommon.h"

#define ngLog NGine::Log::getSingleton()

namespace NGine
{
	class Logger
	{
	public:
		virtual ~Logger() {}
		virtual void message(const char* str) = 0;
		virtual void warning(const char* str) = 0;
		virtual void error(const char* str) = 0;
	};

	class Log
	{
	public:
		~Log();

		static void registerLogger(Logger* logger);
		static void unregisterLogger(Logger* logger);

		static void message(const char* str, ...);
		static void warning(const char* str, ...);
		static void error(const char* str, ...);

		static Log* getSingleton();

	private:
		char mBuffer[4096];
		std::vector<Logger*> mLoggers;
	};

	class ConsoleLogger : public Logger
	{
	public:
		ConsoleLogger();
		~ConsoleLogger();

		virtual void message(const char* str);
		virtual void warning(const char* str);
		virtual void error(const char* str);
	};
}

#endif