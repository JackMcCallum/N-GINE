#include "NGineStable.h"
#include "NGineMesh.h"
#include "NGineGLMesh.h"
#include "NGineObjFile.h"
#include "NGineCamera.h"

namespace NGine
{
	Mesh::Mesh(const std::string& name, const std::string& fname) :
		Resource(name),
		mFileName(fname),
		mGLMesh(nullptr),
		mMeshFile(nullptr),
		m32BitIndices(false)
	{
		assert(!fname.empty());
	}

	Mesh::~Mesh()
	{
		unload();
		unprepare();
	}

	bool Mesh::load()
	{
		if (mFailedToLoad)
			return false;

		if (mLoaded)
			return true;

		if (!prepare())
		{
			mFailedToLoad = true;
			mLoaded = false;
			Log::warning("Failed to prepare mesh '%s'", mFileName.c_str());
			return false;
		}

		// =============================================
		// We have successfully loaded a mesh file
		// Time to start organizing the mesh file into our buffer in the most optimal way possible
		// then have it uploaded to the GPU

		// Clone the submesh data into a local 
		mSubMeshData = mMeshFile->getSubMeshData();

		const MeshFile::VertexData& vertData = mMeshFile->getVertexData();
		const MeshFile::IndexData& indData = mMeshFile->getIndexData();

		size_t numVertices = vertData.numVertices;
		size_t numIndices = indData.numIndices;

		// Calculate the vertex stride
		size_t vertexStride = 0;
		if (vertData.positionData) vertexStride += sizeof(MeshFile::PositionType);
		if (vertData.texcoordData[0]) vertexStride += sizeof(MeshFile::TexcoordType);
		if (vertData.texcoordData[1]) vertexStride += sizeof(MeshFile::TexcoordType);
		if (vertData.texcoordData[2]) vertexStride += sizeof(MeshFile::TexcoordType);
		if (vertData.colorData) vertexStride += sizeof(MeshFile::ColorType);
		if (vertData.normalData) vertexStride += sizeof(MeshFile::NormalType);
		if (vertData.tangentData) vertexStride += sizeof(MeshFile::TangentType);
		if (vertData.boneIndexData) vertexStride += sizeof(MeshFile::BoneIndexType);
		if (vertData.boneWeightData) vertexStride += sizeof(MeshFile::BoneWeightType);

		// Allocate memory to upload
		size_t indexDataSize = sizeof(uint16)* numIndices;
		size_t vertexDataSize = vertexStride * numVertices;

		// Defaults to 16 bit, just double if its 32bit
		if (indData.has32bitIndices) indexDataSize *= 2;

		assert(indexDataSize > 0);
		assert(vertexDataSize > 0);

		// Create the GL mesh, we have data to put in now!
		if (!mGLMesh)
			mGLMesh = new GLMesh();
		GLBuffer& indexBuffer = mGLMesh->getIndexBuffer();
		GLBuffer& vertexBuffer = mGLMesh->getVertexBuffer();

		indexBuffer.resize(indexDataSize, BU_STATIC);
		vertexBuffer.resize(vertexDataSize, BU_STATIC);

		// Lock the buffers
		void* vdata = vertexBuffer.lock();
		void* idata = indexBuffer.lock();
		if (vdata && idata)
		{
#define _COPY_TO_BUFFER(ptr, semantic, count, type, sizeType) \
			if (ptr) \
			{ \
			Utilities::memcpy(ptr, vdata, vertData.numVertices, vertexStride, offset); \
			mGLMesh->setAttribute((int)semantic, true, count, type, offset, vertexStride); \
			offset += sizeof(sizeType); \
		}

			size_t offset = 0;
			_COPY_TO_BUFFER(vertData.positionData, Semantic::SM_POSITION, 3, GL_FLOAT, MeshFile::PositionType);
			_COPY_TO_BUFFER(vertData.texcoordData[0], Semantic::SM_TEXCOORD0, 2, GL_FLOAT, MeshFile::TexcoordType);
			_COPY_TO_BUFFER(vertData.texcoordData[1], Semantic::SM_TEXCOORD1, 2, GL_FLOAT, MeshFile::TexcoordType);
			_COPY_TO_BUFFER(vertData.texcoordData[2], Semantic::SM_TEXCOORD2, 2, GL_FLOAT, MeshFile::TexcoordType);
			_COPY_TO_BUFFER(vertData.texcoordData[3], Semantic::SM_TEXCOORD3, 2, GL_FLOAT, MeshFile::TexcoordType);
			_COPY_TO_BUFFER(vertData.colorData, Semantic::SM_COLOR, 4, GL_FLOAT, MeshFile::ColorType);
			_COPY_TO_BUFFER(vertData.normalData, Semantic::SM_NORMAL, 3, GL_FLOAT, MeshFile::NormalType);
			_COPY_TO_BUFFER(vertData.tangentData, Semantic::SM_TANGENT, 4, GL_FLOAT, MeshFile::TangentType);
			_COPY_TO_BUFFER(vertData.boneIndexData, Semantic::SM_BONEINDICES, 4, GL_FLOAT, MeshFile::BoneIndexType);
			_COPY_TO_BUFFER(vertData.boneWeightData, Semantic::SM_BONEWEIGHTS, 4, GL_FLOAT, MeshFile::BoneWeightType);
#undef _COPY_TO_BUFFER

			memcpy(idata, indData.data, indexDataSize);
			m32BitIndices = indData.has32bitIndices;
			numIndicies = indData.numIndices;
		}
		else
		{
			// If buffers failed to lock, resource failed to load
			vertexBuffer.unlock();
			indexBuffer.unlock();
			mFailedToLoad = true;
			mLoaded = false;
			return false;
		}

		vertexBuffer.unlock();
		indexBuffer.unlock();

		mLoaded = true;
		return true;
	}

	bool Mesh::prepare()
	{
		// Preparation or load was attempted already but failed
		if (mFailedToLoad) return false;

		// Already prepared?
		if (mPrepared) return true;

		// If no file name was specified something with curiously wrong
		if (mFileName.empty())
		{
			mFailedToLoad = true;
			mPrepared = false;
			Log::error("Failed to load mesh '%s'", mFileName.c_str());
			return false;
		}

		// The mesh file should not be here but just in case
		if (!mMeshFile)
		{
			// Load the model
			std::string ext;
			Utilities::splitPath(mFileName, nullptr, nullptr, &ext);

			if (ext == NGINE_MESH_EXT)
				mMeshFile = new MeshFile(mFileName);
			else if (ext == ".obj")
				mMeshFile = new ObjFile(mFileName);
			else
				Log::error("ERROR: Unsupported mesh format '%s' in mesh '%s'", ext.c_str(), getName().c_str());

			if (!mMeshFile)
			{
				mFailedToLoad = true;
				return false;
			}
		}

		if (!mMeshFile->isLoaded())
		{
			mFailedToLoad = true;
			mPrepared = false;
			return false;
		}

		mPrepared = true;
		return true;
	}

	bool Mesh::unprepare()
	{
		mPrepared = false;
		_DELETE_IF(mMeshFile);
		return true;
	}

	bool Mesh::unload()
	{
		mLoaded = false;
		_DELETE_IF(mGLMesh);
		return true;
	}

	MeshRenderer::MeshRenderer(const std::string& name) :
		Renderable(&typeid(MeshRenderer)),
		mMesh(ngResourceMgr.getMesh(name))
	{
		if (!mMesh)
			Log::warning("Null mesh '%s'", name.c_str());

		if (mMesh)
		{
			mMesh->load();
			for (auto iter : mMesh->mSubMeshData)
			{
				DrawCallData d;
				d.count = iter.count;
				d.offset = iter.offset;
				d.lod = iter.lod;
				d.material = ngResourceMgr.getMaterial(iter.material);
				mDrawCallData.push_back(d);
			}
		}
	}

	MeshRenderer::MeshRenderer(MeshPtr mesh) :
		Renderable(&typeid(MeshRenderer)),
		mMesh(mesh)
	{
		if (!mMesh)
			Log::warning("Null mesh");

		if (mMesh)
		{
			mMesh->load();
			for (auto iter : mMesh->mSubMeshData)
			{
				DrawCallData d;
				d.count = iter.count;
				d.offset = iter.offset;
				d.lod = iter.lod;
				d.material = ngResourceMgr.getMaterial(iter.material);
				mDrawCallData.push_back(d);
			}
		}
	}

	MeshRenderer::MeshRenderer(const std::string& mesh, const std::string& material) :
		Renderable(&typeid(MeshRenderer)),
		mMesh(ngResourceMgr.getMesh(mesh))
	{
		if (!mMesh)
			Log::warning("Null mesh '%s'", mesh.c_str());

		MaterialPtr mat = ngResourceMgr.getMaterial(material);

		if (mMesh)
		{
			mMesh->load();
			for (auto iter : mMesh->mSubMeshData)
			{
				DrawCallData d;
				d.count = iter.count;
				d.offset = iter.offset;
				d.lod = iter.lod;
				d.material = mat;
				mDrawCallData.push_back(d);
			}
		}
	}

	MeshRenderer::MeshRenderer(MeshPtr mesh, MaterialPtr material) :
		Renderable(&typeid(MeshRenderer)),
		mMesh(mesh),
		mMaterial(material)
	{
		if (!mMesh)
			Log::warning("Null mesh");

		if (!mMaterial)
			Log::warning("Null material");

		if (mMesh)
		{
			mMesh->load();
			for (auto iter : mMesh->mSubMeshData)
			{
				DrawCallData d;
				d.count = iter.count;
				d.offset = iter.offset;
				d.lod = iter.lod;
				d.material = ngResourceMgr.getMaterial(iter.material);
				mDrawCallData.push_back(d);
			}
		}
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::_extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera)
{
		// Return if mesh is invalid
		if (!mMesh) return;

		// If the mesh is not yet loaded and it hasn't tried yet, load it
		if (!mMesh->mFailedToLoad && !mMesh->mLoaded) mMesh->load();

		// Finally, if the mesh fails to load, theres nothing we can do :(
		if (mMesh->mFailedToLoad) return;

		// Calculate the depth of the object in the scene for sorting
		SceneNode* parent = getParent();
		uint32 depth = 0;
		if (parent)
		{
			// Get the global position of the 
			const glm::vec3& position = parent->getGlobalPosition();
			const glm::vec3& camPos = camera.getParent()->getGlobalPosition();
			float n = camera.getNearClip();
			float f = camera.getFarClip();
			float depthNormalized = glm::length(position - camPos) / (f - n);
			depth = uint32(double(depthNormalized) * 0x0fffff);
		}

		DrawCall drawCall;
		for (auto iter : mDrawCallData)
		{
			// If theres no material, dont make the draw call
			if (iter.material)
			{
				drawCall.glMesh = mMesh->mGLMesh;
				drawCall.indexType = mMesh->m32BitIndices ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
				drawCall.drawMode = GL_TRIANGLES;
				drawCall.count = iter.count;
				drawCall.offset = iter.offset;

				// Hacky submesh ID
				uint16 subMeshID = (drawCall.count + (drawCall.offset << 12) + ((uint32)drawCall.glMesh << 6)) % USHRT_MAX;

				// Needs parent for transform matrix
				drawCall.parent = parent;
				drawCall.material = iter.material;
				
				drawCall.sortKey.key = iter.material->_generateSortKey(depth, subMeshID).key;
				cachedDrawCallList.push_back(drawCall);
			}
			
		}
	}

	SphereRenderer::SphereRenderer(MaterialPtr material) :
		Renderable(&typeid(SphereRenderer)),
		mMaterial(material)
	{
	}

	SphereRenderer::SphereRenderer(const std::string& material) :
		Renderable(&typeid(SphereRenderer)),
		mMaterial(ngResourceMgr.getMaterial(material))
	{
	}

	SphereRenderer::~SphereRenderer()
	{
	}

	void SphereRenderer::_extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera)
	{
		// Calculate the depth of the object in the scene for sorting
		SceneNode* parent = getParent();
		uint32 depth = 0;
		if (parent)
		{
			// Get the global position of the 
			const glm::vec3& position = parent->getGlobalPosition();
			const glm::vec3& camPos = camera.getParent()->getGlobalPosition();
			float n = camera.getNearClip();
			float f = camera.getFarClip();
			float depthNormalized = glm::length(position - camPos) / (f - n);
			depth = uint32(double(depthNormalized) * 0x0fffff);
		}

		// If theres no material, dont make the draw call
		if (mMaterial)
		{
			DrawCall drawCall;

			drawCall.glMesh = ngResourceMgr.getBasicGeometry().mSphereMesh;
			drawCall.indexType = GL_UNSIGNED_SHORT;
			drawCall.drawMode = GL_TRIANGLES;
			drawCall.count = ngResourceMgr.getBasicGeometry().mSphereIndices;
			drawCall.offset = 0;

			// Hacky submesh ID
			uint16 subMeshID = 0;

			// Needs parent for transform matrix
			drawCall.parent = parent;
			drawCall.material = mMaterial;

			drawCall.sortKey.key = mMaterial->_generateSortKey(depth, subMeshID).key;
			cachedDrawCallList.push_back(drawCall);
		}
	}
}
