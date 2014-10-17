#include "NGineStable.h"
#include "NGineMaterial.h"
#include "NGineShader.h"
#include "NGineMain.h"
#include "NGineRenderSystem.h"
#include "NGineTexture.h"
#include "NGineGLTexture.h"

namespace NGine
{
	std::vector<MaterialPtr> Material::sMaterialList;
	NGine::MaterialPtr Material::sFallbackMaterial;
	uint16 Material::mMaterialIDCounter = 0;


	Material::Material(const std::string& name) :
		Resource(name)
	{
		setDiffuse();
		setSpecular();
		setEmisive();
		setPBRParams();

		setDepthMode();
		setCullMode();
		setBlendMode();

		mMaterialID = mMaterialIDCounter++;
	}

	Material::~Material()
	{
	}

	void Material::setShader(const ShaderPtr& shader)
	{
		mShader = shader;
	}

	void Material::setShader(const std::string& name)
	{
		mShader = ngResourceMgr.getShader(name);
	}

	void Material::setActive()
	{
		RenderSystem& rs = Main::getRenderSystem();
		if (mShader)
		{
			mShader->load();
			rs.bindShader(mShader->getGLShader());
		}
		else
		{
			rs.bindShader(nullptr);
		}

		rs.setSurfaceParams(mDiffuse, mSpecular, mEmissive, mPBRParams);

		GLenum depthMode = GL_LESS;
		switch (mDepthFunc)
		{
		case DF_NEVER: depthMode = GL_NEVER; break;
		case DF_LESS: depthMode = GL_LESS; break;
		case DF_EQUAL: depthMode = GL_EQUAL; break;
		case DF_LEQUAL: depthMode = GL_LEQUAL; break;
		case DF_GREATER: depthMode = GL_GREATER; break;
		case DF_NOTEQUAL: depthMode = GL_NOTEQUAL; break;
		case DF_GEQUAL: depthMode = GL_GEQUAL; break;
		case DF_ALWAYS: depthMode = GL_ALWAYS; break;
		default: depthMode = GL_LESS; break;
		}
		rs.setDepthMode(mDepthCheck, mDepthWrite, depthMode);
		
		for (int i = 0; i < 8; i++)
		{
			GLTexture* tex;
			if (mTexture[i])
			{
				// Load if not already loaded
				mTexture[i]->load();
				
				if ((tex = mTexture[i]->getGLTexture()) && tex->getTexture2D())
					Main::getRenderSystem().bindTexture2D(tex->getTexture2D(), i);
			}
		}

		switch (mBlendMode)
		{
		case BM_DISABLED:
			rs.setBlendingMode(false, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);
			break;

		case BM_ADDITIVE:
			rs.setBlendingMode(true, GL_ONE, GL_ONE, GL_FUNC_ADD);
			break;

		case BM_ALPHA_BLEND:
			rs.setBlendingMode(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD);
			break;

		case BM_MODULATIVE:
			rs.setBlendingMode(true, GL_DST_COLOR, GL_ZERO, GL_FUNC_ADD);
			break;
		}
		
		switch (mCullMode)
		{
		case CM_DISABLED:
			rs.setCullingMode(false, GL_BACK);
			break;

		case CM_FRONT:
			rs.setCullingMode(true, GL_FRONT);
			break;

		case CM_BACK:
			rs.setCullingMode(true, GL_BACK);
			break;

		case CM_FRONT_AND_BACK:
			rs.setCullingMode(true, GL_FRONT_AND_BACK);
			break;
		}

	}

	void Material::setDiffuse(const glm::vec4& color)
	{
		mDiffuse = color;
	}

	void Material::setSpecular(const glm::vec4& color)
	{
		mSpecular = color;
	}

	void Material::setEmisive(const glm::vec4& color)
	{
		mEmissive = color;
	}

	void Material::setPBRParams(const glm::vec4& color)
	{
		mPBRParams = color;
	}

	uint16 Material::getID()
	{
		return mMaterialID;
	}

	void Material::setDepthMode(bool check, bool write, DepthFunc func)
	{
		mDepthCheck = check;
		mDepthWrite = write;
		mDepthFunc = func;
	}

	bool Material::isTransparent()
	{
		if (mBlendMode != BM_DISABLED)
			return true;
		return false;
	}

	void Material::setBlendMode(BlendMode blendMode)
	{
		mBlendMode = blendMode;
	}

	void Material::setCullMode(CullMode cullMode)
	{
		mCullMode = cullMode;
	}

	Renderable::DrawCall::SortKey Material::_generateSortKey(uint32 depth24, uint32 submeshID, RenderLayer forceLayer)
	{
		Renderable::DrawCall::SortKey sortKey;

		// Don't need too much info here, just make it draw late
		sortKey.key = 0;
		if (isTransparent())
		{
			sortKey.transparent.depth = 0x0fffff - depth24;
			sortKey.transparent.materialID = getID();
			sortKey.transparent.submeshID = submeshID;
			sortKey.transparent.translucency = mBlendMode;

			if (forceLayer != (RenderLayer)-1)
				sortKey.transparent.layer = forceLayer;
			else
				sortKey.transparent.layer = RL_TRANSPARENT;
		}
		else
		{
			sortKey.opaque.depth = depth24;
			sortKey.opaque.materialID = getID();
			sortKey.opaque.submeshID = submeshID;
			sortKey.opaque.translucency = mBlendMode;

			if (forceLayer != (RenderLayer)-1)
				sortKey.opaque.layer = forceLayer;
			else
				sortKey.opaque.layer = RL_OPAQUE;
		}

		return sortKey;
	}

	void Material::setTexture(const TexturePtr& texture, uint32 unit)
	{
		assert(unit < 8);
		mTexture[unit] = texture;
	}

	void Material::setTexture(const std::string& name, uint32 unit)
	{
		assert(unit < 8);
		mTexture[unit] = ngResourceMgr.getTexture(name);
	}

}