#include "NGineStable.h"
#include "NGineGLShader.h"
#include "NGineRenderCommand.h"

namespace NGine
{
	const uint32 GLShader::sNumInputBindings = 14;
	const char* GLShader::sInputBindings[] =
	{
		"iPosition",
		"iNormal",
		"iTangent",
		"iColor",
		"iBoneWeights",
		"iBoneIndices",
		"iTexCoord0",
		"iTexCoord1",
		"iTexCoord2",
		"iTexCoord3",
		"iTexCoord4",
		"iTexCoord5",
		"iTexCoord6",
		"iTexCoord7",
	};

	const uint32 GLShader::sNumOutputBindings = 8;
	const char* GLShader::sOutputBindings[] =
	{
		"oColor0",
		"oColor1",
		"oColor2",
		"oColor3",
		"oColor4",
		"oColor5",
		"oColor6",
		"oColor7",
	};

	const uint32 GLShader::sNumTexUnitBindings = 8;
	const char* GLShader::sTexUnitBindings[] =
	{
		"uTexUnit0",
		"uTexUnit1",
		"uTexUnit2",
		"uTexUnit3",
		"uTexUnit4",
		"uTexUnit5",
		"uTexUnit6",
		"uTexUnit7",
	};

	const uint32 GLShader::sNumUniformBlockBindings = 1;
	const char* GLShader::sUniformBlockBindings[] =
	{
		"GlobalUniforms",
	};


	GLShader::GLShader()
	{
		mRenderData = RenderDataPtr(new RenderData);
		mRenderData->programHandle = 0;
		mCompiled = false;
	}

	GLShader::~GLShader()
	{
		if (mCompiled)
		{
			mCompiled = false;
			ENQUEUE_RENDER_COMMAND_1PARAMS(GLShader,
				RenderDataPtr, mRenderData, mRenderData,
				{
				if (mRenderData->programHandle)
				{
					glDeleteProgram(mRenderData->programHandle);
					mRenderData->programHandle = 0;
				}
			});
		}
	}

	void GLShader::addSource(const std::string& source, ShaderType type)
	{
		mShaders.push_back(ShaderSource(source, type));
	}

	void GLShader::compile()
	{
		if (!mCompiled)
		{
			mCompiled = true;
			ENQUEUE_RENDER_COMMAND_2PARAMS(GLShader,
				RenderDataPtr, mRenderData, mRenderData,
				ShaderSourceList, mShaders, mShaders,
				{
				if (!mRenderData->programHandle)
				mRenderData->programHandle = glCreateProgram();

				std::vector<uint32> shaderHandles;
				for (auto iter : mShaders)
				{
					uint32 type = GL_VERTEX_SHADER;
					switch (iter.second)
					{
					case ShaderType::VERTEX: type = GL_VERTEX_SHADER; break;
					case ShaderType::CONTROL: type = GL_TESS_CONTROL_SHADER; break;
					case ShaderType::EVALUATION: type = GL_TESS_EVALUATION_SHADER; break;
					case ShaderType::GEOMETRY: type = GL_GEOMETRY_SHADER; break;
					case ShaderType::FRAGMENT: type = GL_FRAGMENT_SHADER; break;
					}

					uint32 shader = glCreateShader(type);

					// Set and compile shader source
					const char* f = iter.first.c_str();
					glShaderSource(shader, 1, &f, nullptr);
					glCompileShader(shader);

					// Check for errors
					int logSize = 0;
					int status = false;
					glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

					if (logSize > 1)
					{
						char* logInfo = new char[logSize];
						glGetShaderInfoLog(shader, logSize, nullptr, logInfo);
						logInfo[logSize - 1] = '\0';

						if (status)
							Log::warning("Shader compiled with warnings:\n%s", logInfo);
						else
							Log::error("Unable to compile shader:\n%s", logInfo);

						delete[] logInfo;
					}

					// Depending on the status, we'll kill the shader or attach it to the program
					if (status)
					{
						shaderHandles.push_back(shader);
						glAttachShader(mRenderData->programHandle, shader);
					}
					else
					{
						glDeleteShader(shader);
					}
				}

				// Bind fragment locations
				for (uint32 i = 0; i < sNumOutputBindings; i++)
					glBindFragDataLocation(mRenderData->programHandle, i, sOutputBindings[i]);
				
				// Bind attribute locations
				for (uint32 i = 0; i < sNumInputBindings; i++)
					glBindAttribLocation(mRenderData->programHandle, i, sInputBindings[i]);
				
				// Link the program
				glLinkProgram(mRenderData->programHandle);

				// Check for linking problems
				int status = 0;
				glGetProgramiv(mRenderData->programHandle, GL_LINK_STATUS, &status);
				if (status == 0)
				{
					int logSize = 0;
					glGetProgramiv(mRenderData->programHandle, GL_INFO_LOG_LENGTH, &logSize);

					if (logSize > 1)
					{
						char* logInfo = new char[++logSize];
						// Now retrieve the info log.
						glGetProgramInfoLog(mRenderData->programHandle, logSize, NULL, logInfo);
						Log::error("Unable to link shader program:\n%s", logInfo);
						delete[] logInfo;
					}

					glDeleteProgram(mRenderData->programHandle);
					mRenderData->programHandle = 0;

					// Delete shaders
					for (auto iter : shaderHandles)
						glDeleteShader(iter);

					return;
				}

				

				// Success!
				// Time to setup some uniform stuff
				Main::getRenderSystem()._rtBindShader(mRenderData->programHandle);
				
				for (uint32 i = 0; i < sNumUniformBlockBindings; i++)
				{
					int32 loc = glGetUniformBlockIndex(mRenderData->programHandle, sUniformBlockBindings[i]);
					if (loc != -1) glUniformBlockBinding(mRenderData->programHandle, loc, i);
				}

				// Set all the texture units to their needed index since this only needs to happen once
				for (uint32 i = 0; i < sNumTexUnitBindings; i++)
				{
					int32 loc = glGetUniformLocation(mRenderData->programHandle, sTexUnitBindings[i]);
					if (loc != -1) glUniform1i(loc, i);
				}

				// Dont need the shaders anymore
				// Detach and delete
				for (auto iter : shaderHandles)
				{
					glDetachShader(mRenderData->programHandle, iter);
					glDeleteShader(iter);
				}
			});
		}
	}


}