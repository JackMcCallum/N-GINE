#include "NGineStable.h"
#include "NGineMeshFile.h"

namespace NGine
{
	// ===========================================================================
	// MeshFile::IndexData

	MeshFile::IndexData::IndexData()
	{
		numIndices = 0;
		data = nullptr;
		has32bitIndices = false;
	}

	MeshFile::IndexData::~IndexData()
	{
		if (data)
		{
			if (has32bitIndices)
				delete[](uint32*)data;
			else
				delete[](uint16*)data;
			data = nullptr;
		}
		numIndices = 0;
		has32bitIndices = false;
	}

	size_t MeshFile::IndexData::sizeInBytes() const
	{
		size_t bytes = 0;
		if (has32bitIndices)
			bytes = numIndices * sizeof(uint32);
		else
			bytes = numIndices * sizeof(uint16);
		bytes += sizeof(numIndices);
		bytes += sizeof(has32bitIndices);
		bytes += sizeof(bool);
		return bytes;
	}

	bool MeshFile::IndexData::write_1_0(std::ostream& os) const
	{
		os.write((char*)&numIndices, sizeof(numIndices));
		os.write((char*)&has32bitIndices, sizeof(has32bitIndices));

		// Write a bool fail safe to confirm if there actually is any data
		bool hasData = data != nullptr;
		os.write((char*)&hasData, sizeof(hasData));

		if (hasData)
		{
			if (has32bitIndices)
				os.write((char*)data, numIndices * sizeof(uint32));
			else
				os.write((char*)data, numIndices * sizeof(uint16));
		}
		return true;
	}

	bool MeshFile::IndexData::read_1_0(std::istream& is)
	{
		// Clean up previous shit
		this->~IndexData();

		bool hasData = false;
		is.read((char*)&numIndices, sizeof(numIndices));
		is.read((char*)&has32bitIndices, sizeof(has32bitIndices));
		is.read((char*)&hasData, sizeof(hasData));

		if (hasData)
		{
			size_t bytes = 0;
			if (has32bitIndices)
				bytes = numIndices * sizeof(uint32);
			else
				bytes = numIndices * sizeof(uint16);
			assert(bytes > 0);

			data = malloc(bytes);
			if (data)
				is.read((char*)data, bytes);
			else
			{
				Log::error("Unable to allocate on line: %s\nFile: %s", __LINE__, __FILE__);
				return false;
			}
		}
		return true;
	}

	// ===========================================================================
	// MeshFile::VertexData

	MeshFile::VertexData::VertexData()
	{
		positionData = nullptr;
		colorData = nullptr;
		normalData = nullptr;
		tangentData = nullptr;
		boneWeightData = nullptr;
		boneIndexData = nullptr;
		for (int i = 0; i < 4; i++)
			texcoordData[i] = nullptr;
	}

	MeshFile::VertexData::~VertexData()
	{
		_DELETE_ARRAY_IF(positionData);
		_DELETE_ARRAY_IF(colorData);
		_DELETE_ARRAY_IF(normalData);
		_DELETE_ARRAY_IF(tangentData);
		_DELETE_ARRAY_IF(boneWeightData);
		_DELETE_ARRAY_IF(boneIndexData);
		_DELETE_ARRAY_IF(texcoordData[0]);
		_DELETE_ARRAY_IF(texcoordData[1]);
		_DELETE_ARRAY_IF(texcoordData[2]);
		_DELETE_ARRAY_IF(texcoordData[3]);
	}

	size_t MeshFile::VertexData::sizeInBytes() const
	{
		size_t bytes = 0;
		bytes += sizeof(numVertices);
		bytes += sizeof(uint32);
		bytes += positionData ? numVertices * sizeof(PositionType) : 0;
		bytes += colorData ? numVertices * sizeof(ColorType) : 0;
		bytes += normalData ? numVertices * sizeof(NormalType) : 0;
		bytes += tangentData ? numVertices * sizeof(TangentType) : 0;
		bytes += boneWeightData ? numVertices * sizeof(BoneWeightType) : 0;
		bytes += boneIndexData ? numVertices * sizeof(BoneIndexType) : 0;
		bytes += texcoordData[0] ? numVertices * sizeof(TexcoordType) : 0;
		return bytes;
	}

	bool MeshFile::VertexData::write_1_0(std::ostream& os) const
	{
		uint32 hasData =
			(positionData ? 1 << 0 : 0) |
			(colorData ? 1 << 1 : 0) |
			(texcoordData[0] ? 1 << 2 : 0) |
			(texcoordData[1] ? 1 << 3 : 0) |
			(texcoordData[2] ? 1 << 4 : 0) |
			(texcoordData[3] ? 1 << 5 : 0) |
			(normalData ? 1 << 6 : 0) |
			(tangentData ? 1 << 7 : 0) |
			(boneWeightData ? 1 << 8 : 0) |
			(boneIndexData ? 1 << 9 : 0);

		os.write((char*)&numVertices, sizeof(numVertices));
		os.write((char*)&hasData, sizeof(hasData));

#define _WRITE_IF(data, type) if (data) os.write((char*)data, numVertices * sizeof(type));
		_WRITE_IF(positionData, MeshFile::PositionType);
		_WRITE_IF(colorData, MeshFile::ColorType);
		_WRITE_IF(texcoordData[0], MeshFile::TexcoordType);
		_WRITE_IF(texcoordData[1], MeshFile::TexcoordType);
		_WRITE_IF(texcoordData[2], MeshFile::TexcoordType);
		_WRITE_IF(texcoordData[3], MeshFile::TexcoordType);
		_WRITE_IF(normalData, MeshFile::NormalType);
		_WRITE_IF(tangentData, MeshFile::TangentType);
		_WRITE_IF(boneWeightData, MeshFile::BoneWeightType);
		_WRITE_IF(boneIndexData, MeshFile::BoneIndexType);
#undef _WRITE_IF

		return true;
	}

	bool MeshFile::VertexData::read_1_0(std::istream& is)
	{
		// Clean up previous shit
		this->~VertexData();

		uint32 hasData = 0;
		is.read((char*)&numVertices, sizeof(numVertices));
		is.read((char*)&hasData, sizeof(hasData));

#define _READ_IF(bit, data, type) \
		if (hasData & (1 << bit)) \
		{ \
		size_t bytes = numVertices * sizeof(type); \
		data = (type*)new uint8[bytes]; \
		if (data) \
		is.read((char*)data, bytes); \
			else \
		{ \
		Log::error("Unable to allocate on line: %s\nFile: %s", __LINE__, __FILE__); \
		return false; \
		}}

		_READ_IF(0, positionData, MeshFile::PositionType);
		_READ_IF(1, colorData, MeshFile::ColorType);
		_READ_IF(2, texcoordData[0], MeshFile::TexcoordType);
		_READ_IF(3, texcoordData[1], MeshFile::TexcoordType);
		_READ_IF(4, texcoordData[2], MeshFile::TexcoordType);
		_READ_IF(5, texcoordData[3], MeshFile::TexcoordType);
		_READ_IF(6, normalData, MeshFile::NormalType);
		_READ_IF(7, tangentData, MeshFile::TangentType);
		_READ_IF(8, boneWeightData, MeshFile::BoneWeightType);
		_READ_IF(9, boneIndexData, MeshFile::BoneIndexType);
#undef _READ_IF

		return true;
	}

	// ===========================================================================
	// MeshFile::SubMesh

	MeshFile::SubMesh::SubMesh()
	{
		count = 0;
		offset = 0;
		lod = 0;
	}

	size_t MeshFile::SubMesh::sizeInBytes() const
	{
		size_t bytes = 0;
		bytes += sizeof(count)+sizeof(lod)+sizeof(offset);
		bytes += sizeof(uint32)+material.length();
		return bytes;
	}

	bool MeshFile::SubMesh::write_1_0(std::ostream& os)
	{
		os.write((char*)&count, sizeof(count));
		os.write((char*)&offset, sizeof(offset));
		os.write((char*)&lod, sizeof(lod));
		uint32 s = (uint32)material.length();
		os.write((char*)&s, sizeof(s));
		os.write(material.c_str(), s);
		return true;
	}

	bool MeshFile::SubMesh::read_1_0(std::istream& is)
	{
		is.read((char*)&count, sizeof(count));
		is.read((char*)&offset, sizeof(offset));
		is.read((char*)&lod, sizeof(lod));
		uint32 s = 0;
		is.read((char*)&s, sizeof(s));
		material.resize(s);
		is.read(&material[0], s);

		return true;
	}

	// ===========================================================================
	// MeshFile

	MeshFile::MeshFile() :
		mIsLoaded(0)
	{
	}

	MeshFile::MeshFile(const std::string& fname) :
		mIsLoaded(0)
	{
		load(fname);
	}

	MeshFile::~MeshFile()
	{
	}

	bool MeshFile::load(const std::string& fname)
	{
		if (isLoaded())
			return false;

		std::ifstream file(fname, std::fstream::binary);
		if (!file.is_open())
			return false;

		Header header;
		file.read((char*)&header, sizeof(Header));

		bool status = false;
		if (header.versionMajor == 1 && header.versionMinor == 0)
			status = _read_1_0(file);
		else
			Log::warning("Unsupported version %i.%i", header.versionMajor, header.versionMinor);

		mIsLoaded = status;

		file.close();
		return status;
	}

	bool MeshFile::save(const std::string& fname) const
	{
		if (!mIsLoaded)
			return false;

		std::ofstream file(fname, std::fstream::binary);
		if (!file.is_open())
			return false;

		bool status = _write_1_0(file);

		file.close();
		return status;
	}

	bool MeshFile::unload()
	{
		mVertexData = VertexData();
		mIndexData = IndexData();
		mSubMeshData.clear();
		return true;
	}

	bool MeshFile::isLoaded() const
	{
		return mIsLoaded;
	}

	const MeshFile::VertexData& MeshFile::getVertexData() const
	{
		return mVertexData;
	}

	const MeshFile::IndexData& MeshFile::getIndexData() const
	{
		return mIndexData;
	}

	const std::vector<MeshFile::SubMesh>& MeshFile::getSubMeshData() const
	{
		return mSubMeshData;
	}

	bool MeshFile::_read_1_0(std::istream& file)
	{
		SubMesh smData;
		Chunk chunk;

		while (!file.eof())
		{
			// Read in a chunk
			file.read((char*)&chunk, sizeof(Chunk));
			size_t g = file.tellg();

			// Load in supported or needed chunks
			switch (chunk.id)
			{
			case INDICES:
				mIndexData.read_1_0(file);
				break;
			case VERTICES:
				mVertexData.read_1_0(file);
				break;
			case SUBMESH:
				smData.read_1_0(file);
				mSubMeshData.push_back(smData);
				break;
			case END_OF_FILE:
				return true;
				break;
			}
			file.seekg(g + chunk.size);
		}

		return true;
	}

	bool MeshFile::_write_1_0(std::ostream& file) const
	{
		Chunk chunk;
		Header header; // Version 1.0
		header.versionMajor = 1;
		header.versionMinor = 0;

#define _WRITE_CHUNK(pid, psize) \
	chunk.id = pid; \
	chunk.size = psize; \
	file.write((char*)&chunk, sizeof(Chunk));


		// Write the header
		file.write((char*)&header, sizeof(Header));

		// Write vertices's
		_WRITE_CHUNK(VERTICES, mVertexData.sizeInBytes());
		mVertexData.write_1_0(file);

		// Write indices
		_WRITE_CHUNK(INDICES, mIndexData.sizeInBytes());
		mIndexData.write_1_0(file);

		// Write all the sub meshes
		for (auto iter : mSubMeshData)
		{
			_WRITE_CHUNK(SUBMESH, iter.sizeInBytes());
			iter.write_1_0(file);
		}

		_WRITE_CHUNK(END_OF_FILE, 0);

		return true;
#undef _WRITE_CHUNK;
	}

	bool MeshFile::_generateTangents(uint32 texIndex)
	{
		// Needs all this data or else it will not work
		assert(texIndex < 4);
		if (!mVertexData.normalData || !mVertexData.positionData || !mVertexData.texcoordData[texIndex])
			return false;

		size_t v = mVertexData.numVertices;
		size_t f = mIndexData.numIndices;

		std::vector<glm::vec3> faceTangent1List;
		std::vector<glm::vec3> faceTangent2List;
		faceTangent1List.resize(v);
		faceTangent2List.resize(v);

		for (size_t i = 0; i < f; i += 3)
		{
			uint32 i1 = 0, i2 = 0, i3 = 0;
			if (mIndexData.has32bitIndices)
			{
				i1 = (uint32)((uint32*)mIndexData.data)[i + 0];
				i2 = (uint32)((uint32*)mIndexData.data)[i + 1];
				i3 = (uint32)((uint32*)mIndexData.data)[i + 2];
			}
			else
			{
				i1 = (uint32)((uint16*)mIndexData.data)[i + 0];
				i2 = (uint32)((uint16*)mIndexData.data)[i + 1];
				i3 = (uint32)((uint16*)mIndexData.data)[i + 2];
			}

			PositionType& v1 = mVertexData.positionData[i1];
			PositionType& v2 = mVertexData.positionData[i2];
			PositionType& v3 = mVertexData.positionData[i3];

			TexcoordType& w1 = mVertexData.texcoordData[texIndex][i1];
			TexcoordType& w2 = mVertexData.texcoordData[texIndex][i2];
			TexcoordType& w3 = mVertexData.texcoordData[texIndex][i3];

			// I think this shit comes from unity, no idea found it on a forum
			// Calculate the tangent
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0f / (s1 * t2 - s2 * t1);
			glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

			if (glm::length2(sdir) > 0 && glm::length2(tdir) > 0)
			{
				faceTangent1List[i1] += sdir;
				faceTangent1List[i2] += sdir;
				faceTangent1List[i3] += sdir;

				faceTangent2List[i1] += tdir;
				faceTangent2List[i2] += tdir;
				faceTangent2List[i3] += tdir;
			}
		}

		if (mVertexData.tangentData)
			delete mVertexData.tangentData;

		mVertexData.tangentData = new TangentType[mVertexData.numVertices];
		if (!mVertexData.tangentData)
		{
			Log::warning("Allocation failure in MeshFile");
			return false;
		}

		for (uint32 i = 0; i<v; i++)
		{
			glm::vec3 t = faceTangent1List[i] - mVertexData.normalData[i] * glm::dot(mVertexData.normalData[i], faceTangent1List[i]);
			float l = glm::dot(t, t);
			if (l > 0) glm::sqrt(l);
			if (l > 0) t /= l;

			float h = (glm::dot(glm::cross(mVertexData.normalData[i], faceTangent1List[i]), faceTangent2List[i]) < 0.0F) ? -1.0F : 1.0F;
			mVertexData.tangentData[i] = glm::vec4(t, h);
		}

		return true;
	}

}