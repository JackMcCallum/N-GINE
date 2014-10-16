#include "StdAfx.h"
#include "Application.h"
#include "Test.h"

int main()
{
	{
		Application* app = new Application();
		app->run();
		delete app;
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
