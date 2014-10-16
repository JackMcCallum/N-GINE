#ifndef __NGINE_SHADER_FILE_H_
#define __NGINE_SHADER_FILE_H_

#include "NGineCommon.h"

namespace NGine
{
	class ShaderFile
	{
	public:
		typedef std::vector<std::pair<std::string, uint32>> TextureUnitList;
		typedef std::pair<std::string, ShaderType> ShaderSource;
		typedef std::vector<ShaderSource> ShaderSourceList;

	public:
		ShaderFile();
		~ShaderFile();

		bool addShader(const std::string& fname, const std::string& define = "");
		
		// Returns a list of the sources
		const ShaderSourceList& getShaderSources();

		// Returns a list of uniform names and unit locations to bind things to
		const TextureUnitList& getTextureUnits();

	private:
		void _split(const std::string& file);
		void _parseShader(std::string& shader, std::vector<std::string> includeList);
		
	private:
		std::string mFileName;
		std::string mDirectory;
		ShaderSourceList mShaders;

		std::string mDefineString;

		TextureUnitList mTextureUnits;
	};
};

#endif // !__NGINE_SHADER_FILE_H_