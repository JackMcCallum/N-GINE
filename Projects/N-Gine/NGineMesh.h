#ifndef __NGINE_MESH_H_
#define __NGINE_MESH_H_

#include "NGineCommon.h"
#include "NGineResource.h"
#include "NGineComponent.h"
#include "NGineMaterial.h"
#include "NGineMeshFile.h"

namespace NGine
{
	class GLMesh;
	class MeshFile;
	class MeshRenderer;

	class Mesh final : public Resource
	{
		friend class ResourceManager;
		friend class MeshRenderer;

		// Keeping construction/destruction private, use static factory functions bellow to construct
		Mesh(const std::string& name, const std::string& fname);
		~Mesh();

	public:
		bool prepare();
		bool unprepare();
		bool load();
		bool unload();

		uint32 numIndicies;

	private:
		GLMesh* mGLMesh;
		MeshFile* mMeshFile;
		std::string mFileName;

		bool m32BitIndices;
		std::vector<MeshFile::SubMesh> mSubMeshData;

	};

	class MeshRenderer : public Renderable
	{
	public:
		MeshRenderer(const std::string& mesh);
		MeshRenderer(const std::string& mesh, const std::string& material);
		MeshRenderer(MeshPtr mesh);
		MeshRenderer(MeshPtr mesh, MaterialPtr material);
		~MeshRenderer();

		virtual void _extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera);

	private:
		MeshPtr mMesh;
		struct DrawCallData
		{
			GLuint count;
			GLuint offset;
			uint8 lod;
			MaterialPtr material;
		};

		MaterialPtr mMaterial;
		std::vector<DrawCallData> mDrawCallData;
	};

}

#endif // !__NGINE_MESH_H_
