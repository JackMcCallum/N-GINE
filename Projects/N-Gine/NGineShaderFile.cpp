#include "NGineStable.h"
#include "NGineShaderFile.h"
#include "NGineUtilities.h"

namespace NGine
{
	ShaderFile::ShaderFile()
	{
	}

	ShaderFile::~ShaderFile()
	{
	}

	bool ShaderFile::addShader(const std::string& fname, const std::string& define)
	{
		// Clear last stuff if there is any
		mFileName.clear();
		mDirectory.clear();
		
		Log::message("Parsing shader file '%s'", fname.c_str());

		// Split the string into dir/filename/extension
		std::string ext, dir, filen;
		Utilities::splitPath(fname, &dir, &filen, &ext);

		mFileName = filen + ext;
		mDirectory = dir;
		mDefineString = define;

		std::string source;
		if (!Utilities::fileToString(mDirectory + mFileName, source))
		{
			Log::error("Unable to open file '%s'", fname.c_str());
			return false;
		}
		
		_split(source);
		
		return true;
	}

	void ShaderFile::_split(const std::string& file)
	{
		mShaders.clear();
		std::stringstream fileStream(file);
		std::string line, token, shader;
		uint32 currentLine = 0;

		ShaderType currentType = (ShaderType)-1;
		while (!fileStream.eof())
		{
			currentLine++;

			std::getline(fileStream, line, '\n');
			std::stringstream lineStream(line);

			std::getline(lineStream, token, ' ');
			if (token == "#shader")
			{
				std::getline(lineStream, token, ' ');
				if (token == "VERTEX")
				{
					if (currentType != (ShaderType)-1)
					{
						std::vector<std::string> includeList;
						includeList.push_back(mFileName);
						_parseShader(shader, includeList);
						shader = mDefineString + shader;
						mShaders.push_back(ShaderSource(shader, currentType));
					}
					shader.clear();

					currentType = ShaderType::VERTEX;
				}
				else if (token == "CONTROL")
				{
					if (currentType != (ShaderType)-1)
					{
						std::vector<std::string> includeList;
						includeList.push_back(mFileName);
						_parseShader(shader, includeList);
						shader = mDefineString + shader;
						mShaders.push_back(ShaderSource(shader, currentType));
					}
					shader.clear();

					currentType = ShaderType::CONTROL;
				}
				else if (token == "EVALUATION")
				{
					if (currentType != (ShaderType)-1)
					{
						std::vector<std::string> includeList;
						includeList.push_back(mFileName);
						_parseShader(shader, includeList);
						shader = mDefineString + shader;
						mShaders.push_back(ShaderSource(shader, currentType));
					}
					shader.clear();

					currentType = ShaderType::EVALUATION;
				}
				else if (token == "GEOMETRY")
				{
					if (currentType != (ShaderType)-1)
					{
						std::vector<std::string> includeList;
						includeList.push_back(mFileName);
						_parseShader(shader, includeList);
						shader = mDefineString + shader;
						mShaders.push_back(ShaderSource(shader, currentType));
					}
					shader.clear();

					currentType = ShaderType::GEOMETRY;
				}
				else if (token == "FRAGMENT")
				{
					if (currentType != (ShaderType)-1)
					{
						std::vector<std::string> includeList;
						includeList.push_back(mFileName);
						_parseShader(shader, includeList);
						shader = mDefineString + shader;
						mShaders.push_back(ShaderSource(shader, currentType));
					}
					shader.clear();

					currentType = ShaderType::FRAGMENT;
				}
				else
					Log::error("Shader parsing error on line %i: Unexpected token '%s'\nFile: %s", currentLine, token.c_str(), (mDirectory + mFileName).c_str());
			}
			else
			{
				shader += line + '\n';
			}
		}

		if (currentType != (ShaderType)-1)
		{
			std::vector<std::string> includeList;
			includeList.push_back(mFileName);
			_parseShader(shader, includeList);
			shader = mDefineString + shader;
			mShaders.push_back(ShaderSource(shader, currentType));
		}
		shader.clear();

	}

	void ShaderFile::_parseShader(std::string& shader, std::vector<std::string> includeList)
	{
		// Copy source
		std::string source = shader;
		shader.clear();

		std::stringstream sourceStream(source);
		std::string line, token;

		while (!sourceStream.eof())
		{
			std::getline(sourceStream, line, '\n');
			std::stringstream lineStream(line);
			std::getline(lineStream, token, ' ');
			
			if (token == "#include")
			{
				std::getline(lineStream, token, '"');
				std::getline(lineStream, token, '"');

				if (!token.empty())
				{
					bool found = false;
					for (auto iter : includeList)
					{
						if (iter == token)
							found = true;
					}

					if (!found)
					{
						includeList.push_back(token);

						std::string incldeFile;
						if (!Utilities::fileToString(mDirectory + token, incldeFile))
							Log::error("Cannot include file '%s' in '%s'", token.c_str(), (mDirectory + mFileName).c_str());
						else
						{
							_parseShader(incldeFile, includeList);
							shader += incldeFile;
						}
					}
				}
			}
			else
			{
				shader += line + '\n';
			}
		}


	}

	const ShaderFile::TextureUnitList& ShaderFile::getTextureUnits()
	{
		return mTextureUnits;
	}

	const ShaderFile::ShaderSourceList& ShaderFile::getShaderSources()
	{
		return mShaders;
	}

	
}