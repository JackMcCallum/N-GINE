#include "stdafx.h"
#include "ObjFile.h"
#include <fstream>
#include <string>
#include <sstream>


ObjFile::ObjFile()
{
	data = nullptr;
	size = 0;
}

ObjFile::~ObjFile()
{
	if (data)
		delete[] data;
}

bool ObjFile::load(const std::string& fname)
{
	std::ifstream file(fname);
	if (!file.is_open())
		return false;

	std::string line, token;

	std::vector<ObjIndex> indices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;

	while (!file.eof())
	{
		std::getline(file, line, '\n');
		std::stringstream lstream(line);
		lstream >> token;

		if (token == "v")
		{
			// Add vertex to the current obj vertex list
			glm::vec3 vec;
			sscanf(line.c_str(), "v %f %f %f", &vec.x, &vec.y, &vec.z);
			positions.push_back(vec);
		}
		else if (token == "vn")
		{
			// Add normal to the current obj vertex list
			glm::vec3 vec;
			sscanf(line.c_str(), "vn %f %f %f", &vec.x, &vec.y, &vec.z);
			normals.push_back(vec);
		}
		else if (token == "f")
		{
			ObjIndex ind[4];
			std::string t[4];
			for (int i = 0; i < 4; i++)
			{
				lstream >> t[i];
				if (!t[i].empty())
					ind[i] = _extractInds(t[i]);
			}

			indices.push_back(ind[0]);
			indices.push_back(ind[1]);
			indices.push_back(ind[2]);

			if (!t[3].empty())
			{
				indices.push_back(ind[0]);
				indices.push_back(ind[2]);
				indices.push_back(ind[3]);
			}
		}
	}

	if (data) delete[] data;
	data = new Vertex[indices.size()];
	size = sizeof(Vertex)*indices.size();

	for (int i = 0; i < indices.size(); i++)
	{
		data[i].position = positions[indices[i].position];
		if (normals.size() > 0)
			data[i].normal = normals[indices[i].normal];
	}

	return true;
}



ObjFile::ObjIndex ObjFile::_extractInds(const std::string& str)
{
	ObjIndex ind;

	unsigned int slashCount = 0;
	unsigned int i = 0;
	while (true)
	{
		if (str[i] == '/') ++slashCount;
		if (str[i] == '\0') break;
		++i;
	}

	// Count how many slashes in string
	if (slashCount == 0)
	{
		// "vertex"
		ind.position = atol(str.c_str()) - 1;
	}
	else if (slashCount == 1)
	{
		// "vertex/texcoord"
		std::string token;
		std::stringstream ssin(str);

		std::getline(ssin, token, '/');
		ind.position = atol(token.c_str()) - 1;

		std::getline(ssin, token, '/');
	}
	else if (slashCount == 2)
	{
		// "vertex//normal" or...
		// "vertex/texcoord/normal"
		std::string token;
		std::stringstream ssin(str);

		std::getline(ssin, token, '/');
		ind.position = atol(token.c_str()) - 1;

		std::getline(ssin, token, '/');
		
		std::getline(ssin, token, '/');
		ind.normal = atol(token.c_str()) - 1;
	}

	return ind;
}
