#ifndef __NGINE_SHADER_H_
#define __NGINE_SHADER_H_

#include "NGineCommon.h"
#include "NGineResource.h"

namespace NGine
{
	class GLShader;
	class ShaderFile;
	
	RESOURCE_PTR(Shader);
	class Shader final : public Resource, public TPoolAllocator<Shader>
	{
		friend class ResourceManager;
	private:
		Shader(const std::string& name, const std::string& fname, const std::string& define = "");
		~Shader();

	public:
		bool prepare();
		bool unprepare();
		bool load();
		bool unload();

		GLShader* getGLShader();

	private:
		GLShader* mGLShader;
		ShaderFile* mShaderFile;

		std::string mFileName;
		std::string mDefine;

		static std::vector<ShaderPtr> sShaderList;
		static ShaderPtr sFallbackShader;
	};
}

#endif // !__NGINE_SHADER_H_
