#ifndef __NGINE_RENDER_SYSTEM_H_
#define __NGINE_RENDER_SYSTEM_H_

#include "NGineCommon.h"
#include "NGineRenderThread.h"
#include "NGineResource.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>

namespace NGine
{
	struct GlobalUniforms
	{
		glm::mat4 uProjMatrix;
		glm::mat4 uViewMatrix;
		glm::mat4 uCameraMatrix;
		glm::mat4 uProjViewMatrix;
		glm::mat4 uWorldMatrixArray[512];

		glm::mat4 uTexProjMatrix;
		glm::mat4 uTexViewMatrix;
		glm::mat4 uTexProjViewMatrix;

		glm::vec4 uMaterialDiffuse;
		glm::vec4 uMaterialSpecular;
		glm::vec4 uMaterialEmissive;
		glm::vec4 uMaterialPBRParams;
		
		// Arbitrary usage parameters
		// If more are needed look into shader listeners
		glm::vec4 uCustomParams0;
		glm::vec4 uCustomParams1;

		// Matrices used for hardware skinning
		glm::mat4 uBoneWorldMatrix[256];
	};

	// Helper struct to upload a block of matrices
	struct GlobalUniforms_CameraMatrices
	{
		glm::mat4 uProjMatrix;
		glm::mat4 uViewMatrix;
		glm::mat4 uCameraMatrix;
		glm::mat4 uProjViewMatrix;
		glm::mat4 uProjViewWorldMatrix;
	};

	// Helper struct to upload a block of matrices
	struct GlobalUniforms_WorldMatrices
	{
		glm::mat4 uProjViewWorldMatrix;
		glm::mat4 uWorldMatrix;
	};

	// Helper struct to upload a block of matrices
	struct GlobalUniforms_MaterialParams
	{
		glm::vec4 uMaterialDiffuse;
		glm::vec4 uMaterialSpecular;
		glm::vec4 uMaterialEmissive;
		glm::vec4 uMaterialPBRParams;
	};

	struct RenderSystemCapabilities
	{
		// GPU stuff
		std::string vendor;
		std::string renderer;
		int32 totalGPUMemory;
		int32 availableGPUMemory;// Available memory as of last capability update

		// Uniform buffers
		int32 maxUniformBufferSize;

		// Texture units
		int32 maxTextureImageUnits;
		int32 maxVertexImageUnits;
		int32 maxCombinedImageUnits;
		int32 maxTextureSize;
		int32 maxCubeTextureSize;

		// Buffers
		uint32 maxVertexAttribs;
		
		// Debugging
		bool supportsDebugOuput;
		bool supportsDirectStateAccess;
		bool supportsNvidiaMemoryInfo;
		bool supportsAtiMemoryInfo;

		// OpenGL versions (Minimum OpenGL 3.3 - DirectX10ish)
		bool supportsGL_3_3;
		bool supportsGL_4_0;
		bool supportsGL_4_1;
		bool supportsGL_4_2;
		bool supportsGL_4_3;
		bool supportsGL_4_4;
	};

	class MemoryBlock;

	class RenderSystem : public RenderThread
	{
	private:
		friend class Main;

		RenderSystem(RenderWindow* window);
		virtual ~RenderSystem();

	public:
		virtual void onThreadStart();
		virtual void onThreadEnd();



		void setCameraMatrices(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam);
		void setWorldMatrix(const glm::mat4& world, uint32 offset);
		void setWorldMatrices(MemoryBlock& matrices, uint32 count, uint32 offset);
		void setTexProjMatrix(const glm::mat4& proj, const glm::mat4& view);
		void setSurfaceParams(const glm::vec4& diff, const glm::vec4& spec, const glm::vec4& emiss, const glm::vec4& pbr);
		void setCustomParams(const glm::vec4& cparams0, const glm::vec4& cparams1);
		void clear(const glm::vec4& color, GLbitfield buffers);

		// GL_COPY_WRITE_BUFFER is used to upload buffer data, to avoid interfering with VAOs
		void bindCopyWriteBuffer(GLBuffer* buffer);
		void bindFrameBuffer(GLFrameBuffer* buffer);
		void bindTexture2D(GLTexture2D* texture2D, uint32 unit);
		void bindTextureCube(GLTextureCube* textureCube, uint32 unit);
		void bindShader(GLShader* shader);
		void bindMesh(GLMesh* mesh);

		void setCullingMode(bool enabled, GLenum face);
		void setBlendingMode(bool enabled, GLenum src, GLenum dest, GLenum func);
		void setViewportMode(const glm::vec4& viewport);
		void setDepthMode(bool check, bool write, GLenum func);

		// Calls that should only ever be called from the render thread
		void _rtSetCameraMatrices(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam);
		void _rtSetWorldMatrix(const glm::mat4& world, uint32 offset);
		void _rtSetWorldMatrices(MemoryBlock& matrices, uint32 count, uint32 offset);
		void _rtSetTexProjMatrix(const glm::mat4& proj, const glm::mat4& view);
		void _rtSetSurfaceParams(const glm::vec4& diff, const glm::vec4& spec, const glm::vec4& emiss, const glm::vec4& pbr);
		void _rtSetCustomParams(const glm::vec4& cparams0, const glm::vec4& cparams1);
		void _rtClear(const glm::vec4& color, GLbitfield buffers);
		
		void _rtBindCopyWriteBuffer(GLuint buffer);
		void _rtBindFrameBuffer(GLuint buffer);
		void _rtBindTexture2D(GLuint texture2D, uint32 unit);
		void _rtBindTextureCube(GLuint textureCube, uint32 unit);
		void _rtBindShader(GLuint shader);
		void _rtBindMesh(GLuint mesh);

		void _rtSetCullingMode(bool enabled, GLenum face);
		void _rtSetBlendingMode(bool enabled, GLenum src, GLenum dest, GLenum func);
		void _rtSetViewportMode(const glm::vec4& viewport);
		void _rtSetDepthMode(bool check, bool write, GLenum func);

		const RenderSystemCapabilities& getRenderSystemCapabilities();

	private:
		void _queryForRenderSystemCapabilities();

	private:
		RenderSystemCapabilities mCapabilities;

		GlobalUniforms mGlobalUniforms;
		uint32 mGlobalUniformBuffer;

		// Render system state
		GLuint mBoundProgram;
		GLuint mBoundVAO;
		GLuint mBoundCopyWriteBuffer;
		bool mDepthMask;
		bool mDepthCheck;
		GLenum mDepthFunc;
		bool mCullfaceEnabled;
		GLenum mCullface;
		bool mBlendEnabled;
		GLenum mBlendSrcFunc;
		GLenum mBlendDestFunc;
		GLenum mBlendEquasion;


	};
}

#endif // !__NGINE_RENDER_SYSTEM_H_
