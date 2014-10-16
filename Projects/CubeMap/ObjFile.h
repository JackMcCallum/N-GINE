#pragma once

#include <string>
#define GLM_SWIZZLE
#include <glm/glm.hpp>

class ObjFile
{
public:
	ObjFile();
	~ObjFile();
	bool load(const std::string& fname);

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	Vertex* data;
	size_t size;

private:
	struct ObjIndex
	{
		unsigned int position;
		unsigned int normal;
	};

	ObjIndex _extractInds(const std::string& str);

};

