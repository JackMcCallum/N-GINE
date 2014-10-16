#ifndef __NGINE_MESH_FILE_H_
#define __NGINE_MESH_FILE_H_

#include "NGineCommon.h"
#include <glm\glm.hpp>
#include <fstream>

namespace NGine
{
	/*
	@author

	@description

	*/

	class MeshFile
	{
	public:
		typedef glm::vec3 PositionType;
		typedef glm::vec4 ColorType;
		typedef glm::vec2 TexcoordType;
		typedef glm::vec3 NormalType;
		typedef glm::vec4 TangentType;
		typedef glm::vec4 BoneWeightType;
		typedef glm::vec4 BoneIndexType;

		enum ChunkID : uint32
		{
			END_OF_FILE,
			INDICES,
			VERTICES,
			SUBMESH,
		};

		struct Chunk
		{
			ChunkID id;
			uint32 size;
		};

		// Describes the file
		struct Header
		{
			uint16 versionMajor;
			uint16 versionMinor;
		};

		// Describes the index data
		struct IndexData
		{
			IndexData();
			~IndexData();
			size_t sizeInBytes() const;
			bool write_1_0(std::ostream& os) const;
			bool read_1_0(std::istream& os);

			uint32 numIndices;
			bool has32bitIndices;
			void* data;
		};

		// Describes the vertex data,
		// nullptr if data doesn't exist
		struct VertexData
		{
			VertexData();
			~VertexData();
			size_t sizeInBytes() const;

			// Read write v1.0 support
			bool write_1_0(std::ostream& os) const;
			bool read_1_0(std::istream& os);

			uint32 numVertices;
			PositionType* positionData;
			ColorType* colorData;
			TexcoordType* texcoordData[4];
			NormalType* normalData;
			TangentType* tangentData;
			BoneWeightType* boneWeightData;
			BoneIndexType* boneIndexData;
		};

		// Describes each material in the file
		struct SubMesh
		{
			SubMesh();
			size_t sizeInBytes() const;

			// Read write v1.0 support
			bool write_1_0(std::ostream& os);
			bool read_1_0(std::istream& is);

			uint32 count;
			uint32 offset;
			uint8 lod;
			std::string material;
		};

	public:
		MeshFile();
		MeshFile(const std::string& fname);
		virtual ~MeshFile();

		// Return true if the file is successfully loaded
		virtual bool load(const std::string& fname);

		// Return true if the file is successfully saved
		virtual bool save(const std::string& fname) const;

		// Unload the mesh file
		virtual bool unload();

		// Return true if the file successfully loaded
		bool isLoaded() const;

		// Return a const reference to the vertex data that can be easily read from
		const VertexData& getVertexData() const;

		// Return a const reference to the index data that can be easily read from
		const IndexData& getIndexData() const;

		// Return a const reference to the sub mesh data that can be easily read from
		const std::vector<SubMesh>& getSubMeshData() const;

	protected:
		bool _read_1_0(std::istream& is);
		bool _write_1_0(std::ostream& os) const;

		bool _generateTangents(uint32 texIndex);

	protected:
		VertexData mVertexData;
		IndexData mIndexData;
		std::vector<SubMesh> mSubMeshData;
		bool mIsLoaded;
	};
}

#endif