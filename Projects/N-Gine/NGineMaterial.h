#ifndef __NGINE_MATERIAL_H_
#define __NGINE_MATERIAL_H_

#include "NGineCommon.h"
#include "NGineResource.h"
#include "NGineComponent.h"

namespace NGine
{
	class ResourceManager;

	class Material final : public Resource
	{
		friend class ResourceManager;
	private:
		Material(const std::string& name);
		~Material();

	public:
		void setShader(const ShaderPtr& shader);
		void setShader(const std::string& name);

		void setTexture(const TexturePtr& texture, uint32 unit);
		void setTexture(const std::string& name, uint32 unit);
		
		void setDiffuse(const glm::vec4& color = glm::vec4(0.7f, 0.7f, 0.7f, 1));
		void setSpecular(const glm::vec4& color = glm::vec4(0.3f, 0.3f, 0.3f, 1));
		void setEmisive(const glm::vec4& color = glm::vec4(0, 0, 0, 0));
		void setPBRParams(const glm::vec4& color = glm::vec4(0, 0, 0, 0));

		void setDepthMode(bool check = true, bool write = true, DepthFunc func = DF_LESS);
		void setBlendMode(BlendMode blendMode = BM_DISABLED);
		void setCullMode(CullMode cullMode = CM_BACK);

		// Get the unique material ID
		uint16 getID();

		// Returns true if this material is of transparent type
		bool isTransparent();

		// Sets all the contents of this material active in the render system
		// Don't call this unless you know well how N-GINE works
		void _setActive();

		Renderable::DrawCall::SortKey _generateSortKey(uint32 depth24, uint32 submeshID, RenderLayer forceLayer = (RenderLayer)-1);

	private:
		ShaderPtr mShader;

		glm::vec4 mDiffuse;
		glm::vec4 mSpecular;
		glm::vec4 mEmissive;
		glm::vec4 mPBRParams;

		bool mDepthCheck;
		bool mDepthWrite;
		DepthFunc mDepthFunc;

		BlendMode mBlendMode;
		CullMode mCullMode;


		// A unique (unless you make more than 65k materials) identifier used
		// for sorting the draw calls to reduce calls
		uint16 mMaterialID;

		//ShaderPtr mDepthShader;
		TexturePtr mTexture[8];

		static std::vector<MaterialPtr> sMaterialList;
		static MaterialPtr sFallbackMaterial;
		static uint16 mMaterialIDCounter;
	};
}

#endif // !__NGINE_MATERIAL_H_
