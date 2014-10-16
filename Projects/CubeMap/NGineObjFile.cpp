#include "NGineStable.h"
#include "NGineObjFile.h"
#include <fstream>
#include <string>
#include <sstream>
#include <limits.h>  
#include <algorithm>

namespace NGine
{
	ObjFile::ObjFile() :
		MeshFile()
	{
	}

	ObjFile::ObjFile(const std::string& fname) :
		MeshFile()
	{
		load(fname);
	}

	ObjFile::~ObjFile()
	{
	}

	bool ObjFile::load(const std::string& fname)
	{
		bool status = false;

		// Open the file
		std::ifstream file(fname);
		if (!file.is_open())
		{
			mIsLoaded = false;
			return false;
		}

		// Parse the file
		status = _parse(file);
		if (!status)
		{
			mIsLoaded = false;
			return false;
		}

		// Done with file
		file.close();

		// Organize the parsed data
		status = _organise();
		if (!status)
		{
			mIsLoaded = false;
			return false;
		}

		_finalize();

		// Success!
		mIsLoaded = true;
		return true;
	}

	bool ObjFile::_parse(std::istream& is)
	{
		// Clear any previous garbage
		mObjObjects.clear();
		// Unload binary mesh content just in case
		unload();

		// Reference to non existing obj, until an obj is defined in the file
		mObjObjects.emplace_back();
		ObjObject* obj = &mObjObjects[0];
		obj->name = "Undefined";
		bool firstObject = true;

		// Reference to non existing obj, until an obj is defined in the file
		obj->materials.emplace_back();
		ObjMaterial* mat = &obj->materials[0];
		mat->name = "Undefined";
		bool firstMaterial = true;

		uint32 pOffset = 0, tOffset = 0, nOffset = 0;
		std::string line, token;

		while (!is.eof())
		{
			std::getline(is, line, '\n');
			std::stringstream lstream(line);
			lstream >> token;

			if (token == "o")
			{
				// Grab the obj name
				lstream >> token;

				// If there is an undefined obj to use
				// just use that otherwise make a new one
				if (firstObject)
				{
					obj->name = token;
					firstObject = false;
				}
				else
				{
					pOffset += obj->positions.size();
					tOffset += obj->texcoords.size();
					nOffset += obj->normals.size();

					size_t s = mObjObjects.size();
					mObjObjects.emplace_back();
					obj = &mObjObjects[s];
					obj->name = token;

					// New object so make a new place to put the indices
					// Undefined material for now until a usemtl shows up
					obj->materials.resize(1);
					mat = &obj->materials[0];
					mat->name = "Undefined";
					firstMaterial = true;
				}
			}
			else if (token == "usemtl")
			{
				// Grab the material name
				lstream >> token;

				// If there is an undefined material to use
				// just use that otherwise make a new one
				if (firstMaterial)
				{
					mat->name = token;
					firstMaterial = false;
				}
				else
				{
					size_t s = obj->materials.size();
					obj->materials.resize(s + 1);
					mat = &obj->materials[s];
					mat->name = token;
				}
			}
			else if (token == "v")
			{
				// Add vertex to the current obj vertex list
				glm::vec3 vec;
				sscanf(line.c_str(), "v %f %f %f", &vec.x, &vec.y, &vec.z);
				obj->positions.push_back(vec);
			}
			else if (token == "vt")
			{
				// Add texcoord to the current obj vertex list
				glm::vec2 vec;
				sscanf(line.c_str(), "vt %f %f", &vec.x, &vec.y);
				obj->texcoords.push_back(vec);
				obj->hasTexcoords = true;
			}
			else if (token == "vn")
			{
				// Add normal to the current obj vertex list
				glm::vec3 vec;
				sscanf(line.c_str(), "vn %f %f %f", &vec.x, &vec.y, &vec.z);
				obj->normals.push_back(vec);
				obj->hasNormals = true;
			}
			else if (token == "f")
			{
				ObjIndex ind[4];
				std::string t[4];
				for (int i = 0; i < 4; i++)
				{
					lstream >> t[i];
					if (!t[i].empty())
					{
						ind[i] = _extractInds(t[i]);
						ind[i].position -= pOffset;
						ind[i].texcoord -= tOffset;
						ind[i].normal -= nOffset;
						ind[i]._hash();
					}
				}

				mat->indices.push_back(ind[0]);
				mat->indices.push_back(ind[1]);
				mat->indices.push_back(ind[2]);

				// Add the alternating triangle if its a quad and no,
				// this does not support ngons
				if (!t[3].empty())
				{
					mat->indices.push_back(ind[0]);
					mat->indices.push_back(ind[2]);
					mat->indices.push_back(ind[3]);
				}
			}
		}

		return true;
	}

	bool ObjFile::_organise()
	{
		size_t numObjs = mObjObjects.size();
		mMeshObjects.resize(numObjs);

		// Iterate the objects
		for (size_t i = 0; i < numObjs; i++)
		{
			// Shortcut references
			MeshObject& mesh = mMeshObjects[i];
			ObjObject& obj = mObjObjects[i];
			mesh.name = obj.name;
			mesh.hasNormals = obj.hasNormals;
			mesh.hasTexcoords = obj.hasTexcoords;

			// Match the two materials list sizes
			size_t numMats = obj.materials.size();
			mesh.materials.resize(numMats);

			// Iterate the materials
			for (size_t j = 0; j < numMats; j++)
			{
				// Short cut references
				MeshMaterial& meshMat = mesh.materials[j];
				ObjMaterial& objMat = obj.materials[j];
				meshMat.name = objMat.name;

				// Match the two index lists sizes
				size_t f = objMat.indices.size();

				// Iterate the indices
				for (size_t k = 0; k < f; k++)
				{
					uint32 dupIndex = 0;
					size_t v = mesh.vertices.size();

					// Find a matching index set
					size_t l;
					for (l = 0; l < v; l++)
					{
						if (objMat.indices[k].hash == mesh.vertices[l].inds.hash &&
							objMat.indices[k] == mesh.vertices[l].inds)
							dupIndex = l;
					}

					if (dupIndex)
						meshMat.indices.push_back(dupIndex);
					else
					{
						MeshVertex v;
						v.position = obj.positions[objMat.indices[k].position];

						if (obj.hasNormals)
							v.normal = obj.normals[objMat.indices[k].normal];

						if (obj.hasTexcoords)
							v.texcoord = obj.texcoords[objMat.indices[k].texcoord];

						v.inds = objMat.indices[k];

						meshMat.indices.push_back(mesh.vertices.size());
						mesh.vertices.push_back(v);
					}
				}
			}
		}

		return true;
	}

	bool ObjFile::_finalize()
	{
		bool useTexcoords = false;
		bool useNormals = false;

		mVertexData.numVertices = 0;
		mIndexData.numIndices = 0;

		// Count the number of vertices & indices
		// Also work out, if any of the objects use normals or 
		// texcoords then we have to use them for all
		for (auto obj : mMeshObjects)
		{
			mVertexData.numVertices += obj.vertices.size();

			if (!useNormals && obj.hasNormals)
				useNormals = true;

			if (!useTexcoords && obj.hasTexcoords)
				useTexcoords = true;

			for (auto mat : obj.materials)
				mIndexData.numIndices += mat.indices.size();
		}

		// Allocate the space for the vertices
		mVertexData.positionData = new PositionType[mVertexData.numVertices];
		if (useNormals) mVertexData.normalData = new NormalType[mVertexData.numVertices];
		if (useTexcoords) mVertexData.texcoordData[0] = new TexcoordType[mVertexData.numVertices];

		if (mVertexData.numVertices < USHRT_MAX)
		{
			mIndexData.has32bitIndices = false;
			mIndexData.data = new uint16[mIndexData.numIndices];
		}
		else
		{
			mIndexData.has32bitIndices = true;
			mIndexData.data = new uint32[mIndexData.numIndices];
		}

		size_t indexStart = 0;
		size_t vertexStart = 0;

		for (auto obj : mMeshObjects)
		{
			size_t numVerts = obj.vertices.size();
			for (size_t i = 0; i < numVerts; i++)
			{
				mVertexData.positionData[vertexStart + i] = obj.vertices[i].position;
				if (useNormals) mVertexData.normalData[vertexStart + i] = obj.vertices[i].normal;
				if (useTexcoords) mVertexData.texcoordData[0][vertexStart + i] = obj.vertices[i].texcoord;
			}
			for (auto mat : obj.materials)
			{
				size_t numInds = mat.indices.size();

				if (mIndexData.has32bitIndices)
				{
					for (size_t i = 0; i < numInds; i++)
						((uint16*)mIndexData.data)[indexStart + i] = mat.indices[i] + vertexStart;
				}
				else
				{
					for (size_t i = 0; i < numInds; i++)
						((uint32*)mIndexData.data)[indexStart + i] = mat.indices[i] + vertexStart;
				}

				SubMesh submesh;
				submesh.material = mat.name;
				submesh.lod = 0;
				submesh.count = mat.indices.size();
				submesh.offset = indexStart;
				mSubMeshData.push_back(submesh);

				indexStart += numInds;
			}

			vertexStart += numVerts;
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
			ind.texcoord = atol(token.c_str()) - 1;
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
			if (token != "")
				ind.texcoord = atol(token.c_str()) - 1;
			else
				ind.texcoord = 0;

			std::getline(ssin, token, '/');
			ind.normal = atol(token.c_str()) - 1;
		}

		return ind;
	}

}