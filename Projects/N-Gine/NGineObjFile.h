#ifndef __NGINE_OBJ_FILE_H_
#define __NGINE_OBJ_FILE_H_

#include "NGineCommon.h"
#include "NGineMeshFile.h"

namespace NGine
{
	class ObjFile : public MeshFile
	{
	public:
		// ========================================================
		// Structures used in the parsing stage

		struct ObjIndex
		{
			inline ObjIndex() : position(0), normal(0), texcoord(0), hash(0) { }

			uint32 position;
			uint32 normal;
			uint32 texcoord;
			uint32 hash;

			inline bool operator == (const ObjIndex& a)
			{
				return (memcmp(this, &a, sizeof(ObjIndex)) == 0);
			}

			// If you have less than 2 million indices,
			// no collisions should occur. (I think...)
			inline void _hash()
			{
				hash = (uint32)(((uint64)position | ((uint64)normal << 22) | ((uint64)texcoord << 43)) % UINT_MAX);
			}
		};

		struct ObjMaterial
		{
			std::string name;
			std::vector<ObjIndex> indices;
		};

		struct ObjObject
		{
			std::string name;
			bool hasNormals;
			bool hasTexcoords;
			std::vector<ObjMaterial> materials;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> texcoords;
		};

		// ========================================================
		// Structures used to simplify the data

		struct MeshVertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
			ObjIndex inds;
		};

		struct MeshMaterial
		{
			std::string name;
			std::vector<uint32> indices;
		};

		struct MeshObject
		{
			std::string name;
			bool hasNormals;
			bool hasTexcoords;
			std::vector<MeshVertex> vertices;
			std::vector<MeshMaterial> materials;
		};

	public:
		ObjFile();
		ObjFile(const std::string& fname);
		virtual ~ObjFile();

		virtual bool load(const std::string& fname) override;

		// Only parses the object. this does not reorganize the data for N-Gine, 
		// this might be useful if you want to do your own specific operations directly on the obj file
		bool parse(const std::string& fname);

		// Return a const reference to the parsed obj data
		const std::vector<ObjObject>& getObjData() const;

	private:
		std::vector<ObjObject> mObjObjects;
		std::vector<MeshObject> mMeshObjects;

	private:
		// Step 1: parse the obj file into mObjObjects
		bool _parse(std::istream& is);

		// Step 2: organize the data into mMeshObjects
		// Using hash may speed up the process but meshes with
		// large triangle counts it may cause unwanted artifacts
		bool _organise();

		// Step 3: finalize the data by moving it into the mesh file ready for saving
		bool _finalize();

		// Helper method to extract indices from the weird format obj puts them in
		ObjIndex _extractInds(const std::string& str);

	};
}

#endif