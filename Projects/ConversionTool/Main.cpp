#include <NGineMeshFile.h>
#include <NGineObjFile.h>
#include <NGineUtilities.h>
#include <NGineLog.h>

#include <vector>
#include <time.h>
#include <soil\soil.h>
#include <iostream>



void main(int argc, char *argv[])
{
	NGine::Log::initialize("./N-GineConversionLog.html");

	for (int i = 1; i < argc; i++)
	{
		std::string dir, fname, ext;
		NGine::Utilities::splitPath(argv[i] , &dir, &fname, &ext);
		uint32 c = 0;
		if (ext == ".obj")
		{
			c = clock();
			NGine::Log::message("Parsing .obj file: %s\nNote: parsing large files will get exponentially slower due to checking for duplicates", argv[i]);
			NGine::ObjFile file;
			if (file.load(argv[i]))
				NGine::Log::message("Parsing successful! %ims", clock()-c);
			else
			{
				NGine::Log::error("Parsing unsuccessful...");
				continue;
			}

			c = clock();
			NGine::Log::message("Saving .obj file: %s", (dir + fname + ".ngm").c_str());
			if (file.save(dir + fname + ".ngm"))
				NGine::Log::message("Saving successful! %ims", clock() - c);
			else
				NGine::Log::error("Saving unsuccessful...");
		}
		else
		{
			NGine::Log::error("Unsupported extension '%s'", ext.c_str());
		}

	}

	NGine::Log::shutdown();
	system("pause");

}
