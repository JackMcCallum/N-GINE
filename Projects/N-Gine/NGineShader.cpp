#include "NGineStable.h"
#include "NGineShader.h"
#include "NGineShaderFile.h"
#include "NGineGLShader.h"

namespace NGine
{
	std::vector<ShaderPtr> Shader::sShaderList;
	ShaderPtr Shader::sFallbackShader;

	Shader::Shader(const std::string& name, const std::string& fname, const std::string& define) :
		Resource(name),
		mFileName(fname),
		mDefine(define),
		mGLShader(nullptr),
		mShaderFile(nullptr)
	{
	}

	Shader::~Shader()
	{
		unprepare();
		unload();
	}

	bool Shader::prepare()
	{
		// Preparation or load was attempted already but failed
		if (mFailedToLoad) return false;

		// Already prepared?
		if (mPrepared) return true;

		// If no file name was specified something with curiously wrong
		if (mFileName.empty())
		{
			mFailedToLoad = true;
			Log::error("Failed to load mesh '%s'", mFileName.c_str());
			return false;
		}

		// Unload any previous stuff just in case
		unprepare();

		mShaderFile = new ShaderFile();
		if (!mShaderFile)
		{
			mFailedToLoad = true;
			return false;
		}

		if (!mShaderFile->addShader(mFileName, mDefine))
		{
			unprepare();
			mFailedToLoad = true;
			return false;
		}

		mPrepared = true;
		return true;
	}

	bool Shader::unprepare()
	{
		mPrepared = false;
		_DELETE_IF(mShaderFile);
		return true;
	}

	bool Shader::load()
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

		// Unload just in case
		unload();

		mGLShader = new GLShader();
		if (!mGLShader)
		{
			mFailedToLoad = true;
			return false;
		}

		for (auto i : mShaderFile->getShaderSources())
			mGLShader->addSource(i.first, i.second);
		mGLShader->compile();

		mLoaded = true;
		return true;
	}

	bool Shader::unload()
	{
		mLoaded = false;
		_DELETE_IF(mGLShader);
		return true;
	}

	GLShader* Shader::getGLShader()
	{
		return mGLShader;
	}

}
