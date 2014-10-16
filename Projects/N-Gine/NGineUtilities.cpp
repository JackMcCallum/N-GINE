#include "NGineStable.h"
#include "NGineUtilities.h"
#include "NGineLog.h"

#include <fstream>
#include <iostream>

namespace NGine
{

	bool Utilities::fileToString(const std::string& fname, std::string& output)
	{
		// Open file
		std::ifstream file(fname);
		if (!file.is_open())
			return false;

		// get size of file
		file.seekg(0,std::ifstream::end);
		uint32 size = (uint32)file.tellg();
		file.seekg(0);

		// new buffer
		char* buffer = new char[size];
		// Set memory to ' '
		memset(buffer,0,size);

		// Read file to buffer
		file.read(buffer,size);
		// Close
		file.close();   

		output = std::string(buffer);
		delete[] buffer;
		
		return true;
	}

	void Utilities::splitPath(std::string path, std::string* dir, std::string* fname, std::string* ext)
	{
		char cdrive[8];
		char cdir[1024];
		char cfname[256];
		char cext[32];

		_splitpath(path.c_str(), cdrive, cdir, cfname, cext);

		if (dir) *dir = std::string(cdrive) + std::string(cdir);
		if (fname) *fname = std::string(cfname);
		if (ext) *ext = std::string(cext);
	}

	bool Utilities::hexColorString(std::string str, float& red, float &green, float &blue)
	{
		size_t s = str.size();
		if (s != 6) 
			return false;

		char v[2];
		v[0] = str[0];
		v[1] = str[1];
		red = std::stoi(v, NULL, 16) / 255.0f;
		v[0] = str[2];
		v[1] = str[3];
		green = std::stoi(v, NULL, 16) / 255.0f;
		v[0] = str[4];
		v[1] = str[5];
		blue = std::stoi(v, NULL, 16) / 255.0f;

		return true;
	}

	uint32 Utilities::hash(const std::string& key)
	{
		const char* str = key.c_str();
		unsigned long h = 0;
		while (*str) 
		{
			h = (h << 4) + *str++;
			unsigned long g = h & 0xF0000000L;
			if (g) h ^= g >> 24;
			h &= ~g;
		}
		return h;
	}

	void Utilities::showMessage(const std::string& message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "N-GINE Message", message.c_str(), nullptr);
		Log::message("MessageBox: %s", message.c_str());
	}

	void Utilities::showMessage(const std::string& title, const std::string& message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title.c_str(), message.c_str(), nullptr);
		Log::message("MessageBox: %s", message.c_str());
	}


}
