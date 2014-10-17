#ifndef __NGINE_RESOURCE_MANAGER_H_
#define __NGINE_RESOURCE_MANAGER_H_

#include "NGineResource.h"
#include "NGineCommon.h"
#include "NGineDirectory.h"

namespace NGine
{
	class Main;

	class ResourceManager
	{
	private:
		friend class Main;
		ResourceManager();
		~ResourceManager();

		struct BasicGeometry;

	public:
		bool addResourceLocation(const std::string& location);

		void setDefaultTexture(const std::string& name);

		// Note that there is not delete function, only remove, this is for a reason
		// Resources use the c++11 shared_ptr system, a resource will only be deleted
		// fully when all references of it have been removed

		MeshPtr createMesh(const std::string& name, const std::string& fname);
		MeshPtr getMesh(const std::string& name);
		void removeMesh(const std::string& name);
		void removeMesh(MeshPtr handle);

		ShaderPtr createShader(const std::string& name, const std::string& fname);
		ShaderPtr getShader(const std::string& name);
		void removeShader(const std::string& name);
		void removeShader(ShaderPtr handle);

		MaterialPtr createMaterial(const std::string& name);
		MaterialPtr getMaterial(const std::string& name);
		void removeMaterial(const std::string& name);
		void removeMaterial(MaterialPtr handle);

		TexturePtr createTexture(const std::string& name, uint32 width, uint32 height, PixelFormat format, uint32 mipLevels = 1);
		TexturePtr createTexture(const std::string& name, const std::string& fname);
		TexturePtr getTexture(const std::string& name);
		void removeTexture(const std::string& name);
		void removeTexture(TexturePtr handle);

		const BasicGeometry& getBasicGeometry();

		void renderQuad(uint32 num = 1);
		void renderCube(uint32 num = 1);
		void renderSphere(uint32 num = 1);

	private:
		void _setupBasicPrimitiveGeometry();

	private:
		std::vector<MeshPtr> mMeshList;
		std::vector<ShaderPtr> mShaderList;
		std::vector<MaterialPtr> mMaterialList;
		std::vector<TexturePtr> mTextureList;

		std::vector<Directory*> mResourceDirectories;

		TexturePtr mDefaultTexture;

		struct BasicGeometry
		{
			// Basic primitive geometry
			GLMesh* mQuadMesh;
			uint32 mQuadIndices;
			GLMesh* mCubeMesh;
			uint32 mCubeIndices;
			GLMesh* mSphereMesh;
			uint32 mSphereIndices;
		} mBasicGeometry;
		

	};
}

#endif // !__NGINE_RESOURCE_MANAGER_H_
