#include "NGineStable.h"
#include "NGineRenderSystem.h"
#include "NGineRenderCommand.h"
#include "NGineMain.h"
#include "NGineMaterial.h"

#include "NGineGLFrameBuffer.h"
#include "NGineGLBuffer.h"
#include "NGineGLTexture2D.h"
#include "NGineGLShader.h"
#include "NGineGLMesh.h"
#include "NGineGLTextureCube.h"
#include "NGineMemoryBlock.h"

namespace NGine
{
	void APIENTRY glErrorCallback(GLenum source,
		GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message,
		GLvoid* userParam)
	{
		(void)source;
		(void)type;
		(void)id;
		(void)severity;
		(void)length;
		(void)userParam;

		Log::error(message);
	}

	RenderSystem::RenderSystem(RenderWindow* window) :
		RenderThread(window),
		mGlobalUniformBuffer(0)
	{
		mBoundCopyWriteBuffer = 0;
		mBoundVAO = 0;
		mBoundProgram = 0;

		mDepthMask = true;
		mDepthCheck = false;
		mDepthFunc = 0;

		mBlendEnabled = false;
		mBlendSrcFunc = 0;
		mBlendDestFunc = 0;
		mBlendEquasion = 0;

	}

	RenderSystem::~RenderSystem()
	{
	}

	void RenderSystem::onThreadStart()
	{
		assert(isRenderThread());

		if (glewInit())
		{
			std::cout << "GLEW not initialized" << std::endl;
		}

		_queryForRenderSystemCapabilities();

		// Setup OpenGL error callbacks if supported
		if (glewIsSupported("GL_ARB_debug_output"))
		{
			Log::message("OpenGL error callbacks initialized successfully");
			glDebugMessageCallbackARB(glErrorCallback, nullptr);
			glEnable(GL_DEBUG_OUTPUT);
		}
		else
		{
			Log::message("OpenGL error callbacks are not supported");
		}

		// Create the global uniforms buffer
		glGenBuffers(1, &mGlobalUniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, mGlobalUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUniforms), &mGlobalUniforms, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, mGlobalUniformBuffer, 0, sizeof(GlobalUniforms));

		_setupBasicPrimitiveGeometry();
	}

	void RenderSystem::onThreadEnd()
	{
		assert(isRenderThread());

		delete mQuadMesh;

		glDeleteBuffers(1, &mGlobalUniformBuffer);
	}

	void RenderSystem::setCameraMatrices(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam)
	{
		ENQUEUE_RENDER_COMMAND_4PARAMS(RenderSystem,
			glm::mat4, proj, proj,
			glm::mat4, view, view,
			glm::mat4, cam, cam,
			RenderSystem*, rs, this,
			rs->_rtSetCameraMatrices(proj, view, cam));
	}

	void RenderSystem::setWorldMatrix(const glm::mat4& world, uint32 offset)
	{
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			glm::mat4, world, world,
			uint32, offset, offset,
			RenderSystem*, rs, this,
			rs->_rtSetWorldMatrix(world, offset));
	}

	void RenderSystem::setWorldMatrices(MemoryBlock& matrices, uint32 count, uint32 offset)
	{
		assert(matrices.size() == sizeof(glm::mat4) * count);
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			uint32, offset, offset,
			uint32, count, count,
			MemoryBlock, matrices, matrices,
			ngRenderSys._rtSetWorldMatrices(matrices, count, offset));
	}

	void RenderSystem::setTexProjMatrix(const glm::mat4& proj, const glm::mat4& view)
	{
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			glm::mat4, proj, proj,
			glm::mat4, view, view,
			RenderSystem*, rs, this,
			rs->_rtSetTexProjMatrix(proj, view));
	}

	void RenderSystem::setSurfaceParams(const glm::vec4& diff, const glm::vec4& spec, const glm::vec4& emiss, const glm::vec4& pbr)
	{
		ENQUEUE_RENDER_COMMAND_5PARAMS(RenderSystem,
			glm::vec4, diff, diff,
			glm::vec4, spec, spec,
			glm::vec4, emiss, emiss,
			glm::vec4, pbr, pbr,
			RenderSystem*, rs, this,
			rs->_rtSetSurfaceParams(diff, spec, emiss, pbr));
	}

	void RenderSystem::setCustomParams(const glm::vec4& cparams0, const glm::vec4& cparams1)
	{
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			glm::vec4, cparams0, cparams0,
			glm::vec4, cparams1, cparams1,
			RenderSystem*, rs, this,
			rs->_rtSetCustomParams(cparams0, cparams1));
	}

	void RenderSystem::clear(const glm::vec4& color, GLbitfield buffers)
	{
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			glm::vec4, color, color,
			GLbitfield, buffers, buffers,
			RenderSystem*, rs, this,
			rs->_rtClear(color, buffers));
	}

	void RenderSystem::bindCopyWriteBuffer(GLBuffer* buffer)
	{
		if (buffer)
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				GLBuffer::RenderDataPtr, data, buffer->mRenderData,
				RenderSystem*, rs, this,
				rs->_rtBindCopyWriteBuffer(data->bufferHandle));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_1PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				rs->_rtBindCopyWriteBuffer(0));
		}
	}

	void RenderSystem::bindFrameBuffer(GLFrameBuffer* buffer)
	{
		if (buffer)
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				GLFrameBuffer::RenderDataPtr, data, buffer->mRenderData,
				RenderSystem*, rs, this,
				rs->_rtBindFrameBuffer(data->frameBufferHandle));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_1PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				rs->_rtBindFrameBuffer(0));
		}
	}

	void RenderSystem::bindTexture2D(GLTexture2D* texture2D, uint32 unit)
	{
		if (texture2D)
		{
			ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
				GLTexture2D::RenderDataPtr, data, texture2D->mRenderData,
				RenderSystem*, rs, this,
				uint32, unit, unit,
				rs->_rtBindTexture2D(data->texHandle, unit));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				uint32, unit, unit,
				rs->_rtBindTexture2D(0, unit));
		}
	}

	void RenderSystem::bindTextureCube(GLTextureCube* textureCube, uint32 unit)
	{
		if (textureCube)
		{
			ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
				GLTextureCube::RenderDataPtr, data, textureCube->mRenderData,
				RenderSystem*, rs, this,
				uint32, unit, unit,
				rs->_rtBindTextureCube(data->texHandle, unit));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				uint32, unit, unit,
				rs->_rtBindTextureCube(0, unit));
		}
	}

	void RenderSystem::bindShader(GLShader* shader)
	{
		if (shader)
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				GLShader::RenderDataPtr, data, shader->mRenderData,
				RenderSystem*, rs, this,
				rs->_rtBindShader(data->programHandle));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_1PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				rs->_rtBindShader(0));
		}
	}

	void RenderSystem::bindMesh(GLMesh* mesh)
	{
		if (mesh)
		{
			ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
				GLMesh::RenderDataPtr, data, mesh->mRenderData,
				RenderSystem*, rs, this,
				rs->_rtBindMesh(data->vaoHandle));
		}
		else
		{
			ENQUEUE_RENDER_COMMAND_1PARAMS(RenderSystem,
				RenderSystem*, rs, this,
				rs->_rtBindMesh(0));
		}
	}

	void RenderSystem::setCullingMode(bool enabled, GLenum face)
	{
		ENQUEUE_RENDER_COMMAND_3PARAMS(RenderSystem,
			GLenum, face, face,
			bool, enabled, enabled,
			RenderSystem*, rs, this,
			rs->_rtSetCullingMode(enabled, face));
	}

	void RenderSystem::setBlendingMode(bool enabled, GLenum src, GLenum dest, GLenum func)
	{
		ENQUEUE_RENDER_COMMAND_5PARAMS(RenderSystem,
			GLenum, src, src,
			GLenum, dest, dest,
			GLenum, func, func,
			bool, enabled, enabled,
			RenderSystem*, rs, this,
			rs->_rtSetBlendingMode(enabled, src, dest, func));
	}

	void RenderSystem::setViewportMode(const glm::vec4& viewport)
	{
		ENQUEUE_RENDER_COMMAND_2PARAMS(RenderSystem,
			glm::vec4, viewport, viewport,
			RenderSystem*, rs, this,
			rs->_rtSetViewportMode(viewport));
	}

	void RenderSystem::setDepthMode(bool check, bool write, GLenum func)
	{
		ENQUEUE_RENDER_COMMAND_4PARAMS(RenderSystem,
			bool, check, check,
			bool, write, write,
			GLenum, func, func,
			RenderSystem*, rs, this,
			rs->_rtSetDepthMode(check, write, func));
	}

	void RenderSystem::_rtSetCameraMatrices(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam)
	{
		mGlobalUniforms.uProjMatrix = proj;
		mGlobalUniforms.uViewMatrix = view;
		mGlobalUniforms.uCameraMatrix = cam;
		mGlobalUniforms.uProjViewMatrix = proj * view;
		
		glBufferSubData(GL_UNIFORM_BUFFER, 
			offsetof(GlobalUniforms, uProjMatrix), 
			4 * sizeof(glm::mat4), 
			&mGlobalUniforms.uProjMatrix);
	}

	void RenderSystem::_rtSetWorldMatrix(const glm::mat4& world, uint32 offset)
	{
		mGlobalUniforms.uWorldMatrixArray[offset] = world;
		
		glBufferSubData(GL_UNIFORM_BUFFER,
			offsetof(GlobalUniforms, uWorldMatrixArray[offset]),
			1 * sizeof(glm::mat4),
			&mGlobalUniforms.uWorldMatrixArray[offset]);
	}

	void RenderSystem::_rtSetWorldMatrices(MemoryBlock& matrices, uint32 count, uint32 offset)
	{
		memcpy(&mGlobalUniforms.uWorldMatrixArray[offset], matrices.data(), sizeof(glm::mat4) * count);
		matrices.free();

		glBufferSubData(GL_UNIFORM_BUFFER,
			offsetof(GlobalUniforms, uWorldMatrixArray[offset]),
			count * sizeof(glm::mat4),
			&mGlobalUniforms.uWorldMatrixArray[offset]);
	}

	void RenderSystem::_rtSetTexProjMatrix(const glm::mat4& proj, const glm::mat4& view)
	{
		mGlobalUniforms.uTexProjMatrix = proj;
		mGlobalUniforms.uTexViewMatrix = view;
		mGlobalUniforms.uTexProjViewMatrix = proj * view;

		glBufferSubData(GL_UNIFORM_BUFFER,
			offsetof(GlobalUniforms, uTexProjMatrix),
			3 * sizeof(glm::mat4),
			&mGlobalUniforms.uTexProjMatrix);
	}

	void RenderSystem::_rtSetSurfaceParams(const glm::vec4& diff, const glm::vec4& spec, const glm::vec4& emiss, const glm::vec4& pbr)
	{
		mGlobalUniforms.uMaterialDiffuse = diff;
		mGlobalUniforms.uMaterialSpecular = spec;
		mGlobalUniforms.uMaterialEmissive = emiss;
		mGlobalUniforms.uMaterialPBRParams = pbr;

		glBufferSubData(GL_UNIFORM_BUFFER,
			offsetof(GlobalUniforms, uMaterialDiffuse),
			4 * sizeof(glm::vec4),
			&mGlobalUniforms.uMaterialDiffuse);
	}

	void RenderSystem::_rtSetCustomParams(const glm::vec4& cparams0, const glm::vec4& cparams1)
	{
		mGlobalUniforms.uCustomParams0 = cparams0;
		mGlobalUniforms.uCustomParams1 = cparams1;

		glBufferSubData(GL_UNIFORM_BUFFER,
			offsetof(GlobalUniforms, uCustomParams0),
			2 * sizeof(glm::vec4),
			&mGlobalUniforms.uCustomParams0);
	}

	void RenderSystem::_rtClear(const glm::vec4& color, GLbitfield buffers)
	{
		glDepthMask(true);
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(buffers);
	}

	void RenderSystem::_rtBindCopyWriteBuffer(GLuint buffer)
	{
		if (mBoundCopyWriteBuffer != buffer)
		{
			glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
			mBoundCopyWriteBuffer = buffer;
		}
	}

	void RenderSystem::_rtBindFrameBuffer(GLuint buffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	}

	void RenderSystem::_rtBindTexture2D(GLuint texture2D, uint32 unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture2D);
	}

	void RenderSystem::_rtBindTextureCube(GLuint textureCube, uint32 unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
	}

	void RenderSystem::_rtBindShader(GLuint shader)
	{
		if (shader != mBoundProgram)
		{
			mBoundProgram = shader;
			glUseProgram(shader);
		}
	}

	void RenderSystem::_rtBindMesh(GLuint mesh)
	{
		if (mBoundVAO != mesh)
		{
			mBoundVAO = mesh;
			glBindVertexArray(mesh);
		}
	}

	void RenderSystem::_rtSetCullingMode(bool enabled, GLenum face)
	{
		if (mCullface != enabled)
		{
			mCullface = enabled;
			if (enabled)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
		}

		if (mBlendEnabled && mCullface != face)
		{
			mCullface = face;
			glCullFace(face);
		}
	}

	void RenderSystem::_rtSetBlendingMode(bool enabled, GLenum src, GLenum dest, GLenum func)
	{
		if (mBlendEnabled != enabled)
		{
			mBlendEnabled = enabled;
			if (enabled)
				glEnable(GL_BLEND);
			else
				glDisable(GL_BLEND);
		}

		if (mBlendEnabled)
		{
			if (mBlendSrcFunc != src || mBlendDestFunc != dest)
			{
				mBlendSrcFunc = src;
				mBlendDestFunc = dest;
				glBlendFunc(src, dest);
			}

			if (mBlendEquasion != func)
			{
				mBlendEquasion = func;
				glBlendEquation(func);
			}
		}
		
	}

	void RenderSystem::_rtSetViewportMode(const glm::vec4& viewport)
	{
		glViewport((GLint)viewport.x, (GLint)viewport.y, (GLsizei)viewport.z, (GLsizei)viewport.w);
	}

	void RenderSystem::_rtSetDepthMode(bool check, bool write, GLenum func)
	{
		if (check != mDepthCheck)
		{
			mDepthCheck = check;
			if (check)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}
		
		if (write != mDepthMask)
		{
			mDepthMask = write;
			if (write)
				glDepthMask(true);
			else
				glDepthMask(false);
		}

		if (func != mDepthFunc)
		{
			mDepthFunc = func;
			glDepthFunc(func);
		}
	}

	void RenderSystem::_queryForRenderSystemCapabilities()
	{
		Log::message(" --- Querying OpenGL Capabilities --- ");

		// Check for version support
		mCapabilities.supportsGL_3_3 = GLEW_VERSION_3_3 > 0;
		mCapabilities.supportsGL_4_0 = GLEW_VERSION_4_0 > 0;
		mCapabilities.supportsGL_4_1 = GLEW_VERSION_4_1 > 0;
		mCapabilities.supportsGL_4_2 = GLEW_VERSION_4_2 > 0;
		mCapabilities.supportsGL_4_3 = GLEW_VERSION_4_3 > 0;
		mCapabilities.supportsGL_4_4 = GLEW_VERSION_4_4 > 0;

		// Bail out now if the computer doest support the minimum version we need
		if (!mCapabilities.supportsGL_3_3)
			Utilities::showMessage("N-GINE requires a minimum of OpenGL 3.3 support, this PC does not support it");
		
		// Display version
		if (mCapabilities.supportsGL_4_4)
			Log::message("OpenGL version 4.4");
		else if (mCapabilities.supportsGL_4_3)
			Log::message("OpenGL version 4.3");
		else if (mCapabilities.supportsGL_4_2)
			Log::message("OpenGL version 4.2");
		else if (mCapabilities.supportsGL_4_1)
			Log::message("OpenGL version 4.1");
		else if (mCapabilities.supportsGL_4_0)
			Log::message("OpenGL version 4.0");
		else if (mCapabilities.supportsGL_3_3)
			Log::message("OpenGL version 3.3");

		// Query for support
		mCapabilities.supportsDebugOuput = glewIsSupported("GL_ARB_debug_output") > 0;
		Log::message("Supports debug output: %s", mCapabilities.supportsDebugOuput ? "Yes" : "No");

		mCapabilities.supportsDirectStateAccess = glewIsSupported("GL_EXT_direct_state_access") > 0;
		Log::message("Supports direct state access: %s", mCapabilities.supportsDirectStateAccess ? "Yes" : "No");

		mCapabilities.supportsNvidiaMemoryInfo = glewIsSupported("GL_NVX_gpu_memory_info") > 0;
		Log::message("Supports query NVIDIA memory info: %s", mCapabilities.supportsNvidiaMemoryInfo ? "Yes" : "No");

		mCapabilities.supportsAtiMemoryInfo = glewIsSupported("GL_ATI_meminfo") > 0;
		Log::message("Supports query ATI memory info: %s", mCapabilities.supportsAtiMemoryInfo ? "Yes" : "No");

		// GPU query
		mCapabilities.vendor = std::string((const char*)glGetString(GL_VENDOR));
		Log::message("Device vendor: %s", mCapabilities.vendor.c_str());

		mCapabilities.renderer = std::string((const char*)glGetString(GL_RENDERER));
		Log::message("Renderer device: %s", mCapabilities.renderer.c_str());

		if (mCapabilities.supportsNvidiaMemoryInfo)
		{
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &mCapabilities.totalGPUMemory);
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &mCapabilities.availableGPUMemory);
			Log::message("Total available video memory: %iMB", mCapabilities.totalGPUMemory / 1024);
			Log::message("Current available video memory: %iMB", mCapabilities.availableGPUMemory / 1024);
		}
		else
		{
			mCapabilities.totalGPUMemory = -1;
			mCapabilities.availableGPUMemory = -1;
		}

		// Query for limits
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mCapabilities.maxTextureImageUnits);
		Log::message("Max texture image units: %i", mCapabilities.maxTextureImageUnits);

		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &mCapabilities.maxVertexImageUnits);
		Log::message("Max vertex texture image units: %i", mCapabilities.maxVertexImageUnits);

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mCapabilities.maxCombinedImageUnits);
		Log::message("Max combined texture image units: %i", mCapabilities.maxCombinedImageUnits);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mCapabilities.maxTextureSize);
		Log::message("Max texture size: %i", mCapabilities.maxTextureSize);

		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &mCapabilities.maxUniformBufferSize);
		Log::message("Max uniform block size: %i", mCapabilities.maxUniformBufferSize);


	}

	const RenderSystemCapabilities& RenderSystem::getRenderSystemCapabilities()
	{
		return Main::getRenderSystem().mCapabilities;
	}

	void RenderSystem::renderQuad(MaterialPtr material)
	{
		if (material)
		{
			material->_setActive();
			mQuadMesh->draw(GL_TRIANGLES, GL_BYTE, 0, 4);
		}
	}

	void RenderSystem::renderCube(MaterialPtr material)
	{
		if (material)
		{
			material->_setActive();
			ENQUEUE_RENDER_COMMAND_2PARAMS(Quad,
				uint32, handle, mCubeVAO,
				uint32, indices, mCubeIndices,
				{
				ngRenderSys._rtBindMesh(handle);
				glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, nullptr);
			});
		}
	}

	void RenderSystem::renderSphere(MaterialPtr material)
	{
		if (material)
		{
			material->_setActive();
			ENQUEUE_RENDER_COMMAND_2PARAMS(Quad,
				uint32, handle, mSphereVAO,
				uint32, indices, mSpereIndices,
				{
				ngRenderSys._rtBindMesh(handle);
				glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, nullptr);
			});
		}
	}

	void RenderSystem::_setupBasicPrimitiveGeometry()
	{
		glm::vec3 quadGeom[] =
		{
			glm::vec3(-1, -1, 0),
			glm::vec3(1, -1, 0),
			glm::vec3(-1, 1, 0),
			glm::vec3(1, 1, 0),
		};

		glm::vec3 cubeVerts[] =
		{
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(-1, 1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(-1, 1, 1),
			glm::vec3(1, 1, 1),
		};

		// 0246 1357

		uint16 cubeInds[] =
		{
			1, 3, 5, 3, 7, 5, // +X
			2, 0, 4, 6, 2, 4, // -X
			3, 2, 6, 7, 3, 6, // +Y
			0, 1, 4, 1, 5, 4, // -Y
			4, 5, 6, 5, 7, 6, // +Z
			1, 0, 2, 3, 1, 2, // -Z
		};
		mCubeIndices = sizeof(cubeInds) / sizeof(uint16);


#define offset(xx, yy) ((y+yy) * segments + (x+xx >= segments ? 0 : x+xx))

		const uint32 segments = 24;
		const uint32 rings = 16;
		glm::vec3 sphereVerts[segments*(rings + 1)];
		uint16 sphereInds[segments*(rings + 1) * 6];
		mSpereIndices = segments*(rings + 1) * 6;

		for (uint32 x = 0; x < segments; x++)
		for (uint32 y = 0; y < (rings + 1); y++)
		{
			sphereVerts[y * segments + x] = glm::vec3(
				glm::sin((x / float(segments)) * glm::pi<float>() * 2) * glm::sin((y / float(rings)) * glm::pi<float>()),
				-glm::cos((y / float(rings)) * glm::pi<float>()),
				glm::cos((x / float(segments)) * glm::pi<float>() * 2) * glm::sin((y / float(rings)) * glm::pi<float>()));
		}

		for (uint32 x = 0; x < segments; x++)
		for (uint32 y = 0; y < rings; y++)
		{
			uint32 i = y * 6 * segments + x * 6;
			sphereInds[i + 0] = offset(0, 0);
			sphereInds[i + 1] = offset(1, 0);
			sphereInds[i + 2] = offset(0, 1);
			sphereInds[i + 3] = offset(1, 0);
			sphereInds[i + 4] = offset(1, 1);
			sphereInds[i + 5] = offset(0, 1);
		}

		mQuadMesh = new GLMesh();
		mQuadMesh->getVertexBuffer().resize(sizeof(quadGeom), BU_STATIC);
		void* data = mQuadMesh->getVertexBuffer().lock();
		memcpy(data, quadGeom, sizeof(quadGeom));
		mQuadMesh->getVertexBuffer().unlock();
		mQuadMesh->setAttribute(Semantic::SM_POSITION, true, 3, GL_FLOAT, 0, sizeof(glm::vec3));
		mQuadMesh->setAttribute(Semantic::SM_NORMAL, true, 3, GL_FLOAT, 0, sizeof(glm::vec3));

		glGenVertexArrays(1, &mCubeVAO);
		_rtBindMesh(mCubeVAO);
		glGenBuffers(1, &mCubeVBO);
		glGenBuffers(1, &mCubeIBO);
		glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCubeIBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInds), cubeInds, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);

		glGenVertexArrays(1, &mSphereVAO);
		_rtBindMesh(mSphereVAO);
		glGenBuffers(1, &mSphereVBO);
		glGenBuffers(1, &mSphereIBO);
		glBindBuffer(GL_ARRAY_BUFFER, mSphereVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSphereIBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVerts), sphereVerts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereInds), sphereInds, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);
		_rtBindMesh(0);

	}

}