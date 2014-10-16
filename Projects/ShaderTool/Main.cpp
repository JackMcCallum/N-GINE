#include "Precompiled.h"
#include "ProgramMain.h"

int main(int argc, char* argv[])
{
	{
		ProgramMain* program = new ProgramMain();
		if (!program->run())
		{
			delete program;
			return EXIT_FAILURE;
		}
		delete program;
	}
	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}