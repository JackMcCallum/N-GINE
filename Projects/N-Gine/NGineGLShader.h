#ifndef __NGINE_HARDWARE_SHADER_H_
#define __NGINE_HARDWARE_SHADER_H_

#include "NGineCommon.h"
#include "NGineShaderFile.h"

namespace NGine
{
	class RenderSystem;

	class GLShader final : public TPoolAllocator<GLShader>
	{
		friend class RenderSystem;

		typedef std::pair<std::string, ShaderType> ShaderSource;
		typedef std::vector<ShaderSource> ShaderSourceList;

	public:
		GLShader();
		~GLShader();
		
		void addSource(const std::string& source, ShaderType type);

		// Compiles and links the shader
		void compile();

		// Note that this corresponds with NGine::Semantic
		static const uint32 sNumInputBindings;
		static const char* sInputBindings[];

		// Note that this corrensponds with NGine::Output
		static const uint32 sNumOutputBindings;
		static const char* sOutputBindings[];
		
		static const uint32 sNumTexUnitBindings;
		static const char* sTexUnitBindings[];

		static const uint32 sNumUniformBlockBindings;
		static const char* sUniformBlockBindings[];

	private:
		bool mCompiled;

		ShaderSourceList mShaders;

		// Data owned by the render thread, this is garbage collected
		// so that it will outlive the class if it needs to
		struct RenderData
		{
			uint32 programHandle;
			std::vector<IntIntPair> uniformLocations; // Hash/Location
		};
		// Garbage collected render data
		typedef std::shared_ptr<RenderData> RenderDataPtr;
		RenderDataPtr mRenderData;
	};

	
}



#endif // !__NGINE_HARDWARE_SHADER_H_

