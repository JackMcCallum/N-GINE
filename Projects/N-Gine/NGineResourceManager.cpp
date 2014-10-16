#include "NGineStable.h"
#include "NGineResourceManager.h"
#include "NGineMesh.h"
#include "NGineShader.h"
#include "NGineMaterial.h"
#include "NGineTexture.h"

namespace NGine
{


	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (auto iter : mMeshList)
			delete iter.get();
		
		for (auto iter : mShaderList)
			delete iter.get();

		for (auto iter : mTextureList)
			delete iter.get();

		for (auto iter : mMaterialList)
			delete iter.get();

		for (auto iter : mResourceDirectories)
			delete iter;
	}

	bool ResourceManager::addResourceLocation(const std::string& location)
	{
		Log::message(" --- Parsing resource directory ---");
		Directory* directory = new Directory(location);
		if (!directory->scanDirectory())
		{
			delete directory;
			return false;
		}
		
		directory->getRootFile()->recursivly([this](Directory::File* file)
		{
			if (!file->isDirectory)
			{
				std::string ext;
				Utilities::splitPath(file->fileName, nullptr, nullptr, &ext);

				if (ext == ".png" || ext == ".tga" || ext == ".jpg" || ext == NGINE_TEXTURE_EXT)
				{
					Log::message("Adding texture: %s", file->fileName.c_str());
					createTexture(file->fileName, file->filePath);
				}
				else if (ext == ".obj" || ext == NGINE_MESH_EXT)
				{
					Log::message("Adding mesh: %s", file->fileName.c_str());
					createMesh(file->fileName, file->filePath);
				}
				else if (ext == ".glsl" || ext == ".shader" || ext == NGINE_SHADER_EXT)
				{
					Log::message("Adding shader: %s", file->fileName.c_str());
					createShader(file->fileName, file->filePath);
				}
			}
			else
			{
				Log::message(file->filePath.c_str());
			}

			return true;
		});

		mResourceDirectories.push_back(directory);
		return true;
	}

	MeshPtr ResourceManager::createMesh(const std::string& name, const std::string& fname)
	{
		MeshPtr mesh(new Mesh(name, fname));
		if (mesh)
		{
			mMeshList.push_back(mesh);
			return mesh;
		}
		Log::error("Failed to create mesh '%s'", name.c_str());
		return MeshPtr();
	}

	MeshPtr ResourceManager::getMesh(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::find_if(mMeshList.begin(), mMeshList.end(),
			[&](MeshPtr texture) { return texture->getNameHash() == hash; });

		if (iterator != mMeshList.end())
			return *iterator;

		Log::error("No mesh with the name '%s' exists", name.c_str());
		return MeshPtr();
	}

	void ResourceManager::removeMesh(MeshPtr handle)
	{
		if (!handle)
		{
			Log::error("ERROR: Tried to remove null mesh.");
			return;
		}
		else
		{
			auto iterator = std::remove_if(mMeshList.begin(), mMeshList.end(),
				[&](const MeshPtr& mesh){ return mesh == handle; });

			if (iterator == mMeshList.end())
				Log::error("ERROR: No mesh by the name of '%s' exists in the resource manager.", handle->getName().c_str());
		}
	}

	void ResourceManager::removeMesh(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::remove_if(mMeshList.begin(), mMeshList.end(),
			[&](const MeshPtr& mesh){ return mesh->getNameHash() == hash; });

		if (iterator == mMeshList.end())
			Log::error("ERROR: No mesh by the name of '%s' exists in the resource manager.", name.c_str());
	}

	ShaderPtr ResourceManager::createShader(const std::string& name, const std::string& fname)
	{
		ShaderPtr shader(new Shader(name, fname));
		if (shader)
		{
			mShaderList.push_back(shader);
			return shader;
		}
		Log::error("Failed to create texture '%s'", name.c_str());
		return ShaderPtr();
	}

	ShaderPtr ResourceManager::getShader(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::find_if(mShaderList.begin(), mShaderList.end(),
			[&](const ShaderPtr& shader) { return shader->getNameHash() == hash; });

		if (iterator != mShaderList.end())
			return *iterator;

		Log::error("No shader with the name '%s' exists", name.c_str());
		return ShaderPtr();
	}

	void ResourceManager::removeShader(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::remove_if(mTextureList.begin(), mTextureList.end(),
			[&](const TexturePtr& mesh){ return mesh->getNameHash() == hash; });

		if (iterator == mTextureList.end())
			Log::error("ERROR: No shader by the name of '%s' exists in the resource manager.", name.c_str());
	}

	void ResourceManager::removeShader(ShaderPtr handle)
	{
		if (!handle)
		{
			Log::error("ERROR: Tried to remove null shader.");
			return;
		}
		else
		{
			auto iterator = std::remove_if(mShaderList.begin(), mShaderList.end(),
				[&](const ShaderPtr& shader){ return shader == handle; });

			if (iterator == mShaderList.end())
				Log::error("ERROR: No shader by the name of '%s' exists in the resource manager.", handle->getName().c_str());
		}
	}

	MaterialPtr ResourceManager::createMaterial(const std::string& name)
	{
		MaterialPtr material(new Material(name));
		if (material)
		{
			mMaterialList.push_back(material);
			return material;
		}
		Log::error("Failed to create material '%s'", name.c_str());
		return MaterialPtr();
	}

	MaterialPtr ResourceManager::getMaterial(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::find_if(mMaterialList.begin(), mMaterialList.end(),
			[&](const MaterialPtr& mesh) { return mesh->mNameHash == hash; });

		if (iterator != mMaterialList.end()) 
			return *iterator;

		Log::error("No material with the name '%s' exists", name.c_str());
		return MaterialPtr();
	}

	void ResourceManager::removeMaterial(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::remove_if(mMaterialList.begin(), mMaterialList.end(), 
			[&](const MaterialPtr& mesh){ return mesh->getNameHash() == hash; });

		if (iterator == mMaterialList.end())
			Log::error("ERROR: No material by the name of '%s' exists in the resource manager.", name.c_str());
	}

	void ResourceManager::removeMaterial(MaterialPtr handle)
	{
		if (!handle)
		{
			Log::error("ERROR: Tried to remove null material.");
			return;
		}
		else
		{
			auto iterator = std::remove_if(mMaterialList.begin(), mMaterialList.end(), 
				[&](const MaterialPtr& mesh){ return mesh == handle; });

			if (iterator == mMaterialList.end())
				Log::error("ERROR: No material by the name of '%s' exists in the resource manager.", handle->getName().c_str());
		}
	}

	TexturePtr ResourceManager::createTexture(const std::string& name, uint32 width, uint32 height, PixelFormat format, uint32 mipLevels)
	{
		TexturePtr texture(new Texture(name, width, height, format, mipLevels));
		if (texture)
		{
			mTextureList.push_back(texture);
			return texture;
		}
		Log::error("Failed to create texture '%s'", name.c_str());
		return TexturePtr();
	}

	TexturePtr ResourceManager::createTexture(const std::string& name, const std::string& fname)
	{
		TexturePtr texture(new Texture(name, fname));
		if (texture)
		{
			mTextureList.push_back(texture);
			return texture;
		}
		Log::error("Failed to create texture '%s'", name.c_str());
		return TexturePtr();
	}

	TexturePtr ResourceManager::getTexture(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		std::vector<TexturePtr>::iterator iterator;
		iterator = std::find_if(mTextureList.begin(), mTextureList.end(),
			[&](const TexturePtr& texture) { return texture->getNameHash() == hash; });

		if (iterator != mTextureList.end())
			return *iterator;

		Log::error("No texture with the name '%s' exists", name.c_str());
		return mDefaultTexture;
	}

	void ResourceManager::removeTexture(const std::string& name)
	{
		uint32 hash = Utilities::hash(name);

		auto iterator = std::remove_if(mTextureList.begin(), mTextureList.end(),
			[&](const TexturePtr& mesh){ return mesh->getNameHash() == hash; });

		if (iterator == mTextureList.end())
			Log::error("ERROR: No texture by the name of '%s' exists in the resource manager.", name.c_str());
	}

	void ResourceManager::removeTexture(TexturePtr handle)
	{
		if (!handle)
		{
			Log::error("ERROR: Tried to remove null texture.");
			return;
		}
		else
		{
			auto iterator = std::remove_if(mTextureList.begin(), mTextureList.end(),
				[&](const TexturePtr& texture){ return texture == handle; });

			if (iterator == mTextureList.end())
				Log::error("ERROR: No texture by the name of '%s' exists in the resource manager.", handle->getName().c_str());
		}
	}

	void ResourceManager::setDefaultTexture(const std::string& name)
	{
		mDefaultTexture = getTexture(name);
	}


}