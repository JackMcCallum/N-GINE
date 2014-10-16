#ifndef __NGINE_GL_MESH_H_
#define __NGINE_GL_MESH_H_

#include "NGineResource.h"
#include "NGineGLBuffer.h"
#include "NGineGLShader.h"

namespace NGine
{
	class RenderSystem;

	class GLMesh
	{
		friend class RenderSystem;

	public:
		GLMesh();
		~GLMesh();

		GLBuffer& getVertexBuffer();
		const GLBuffer& getVertexBuffer() const;

		GLBuffer& getIndexBuffer();
		const GLBuffer& getIndexBuffer() const;

		void setAttribute(uint32 idx, bool enabled, uint32 count, GLenum type, uint32 offset, uint32 stride, bool normalized = false);
		
		void draw(GLenum mode, GLenum type, uint32 offset = 0, uint32 count = 0, uint32 instances = 1);

	private:
		void _initResource();
		void _releaseResource();

	private:
		// GPU Handle to the Vertex Array Object
		struct RenderData
		{
			uint32 vaoHandle;
			uint32 iboHandle;
			uint32 vboHandle;
		};
		typedef std::shared_ptr<RenderData> RenderDataPtr;
		RenderDataPtr mRenderData;

		GLBuffer mIndexBuffer;
		GLBuffer mVertexBuffer;

		// Vertex buffer properties
		struct Attribute
		{
			uint32 count;
			uint32 offset;
			uint32 stride;
			GLenum type;
			bool normalized;
			bool enabled;
		};

		struct Attributes
		{
			Attribute attribute[__SEMANTIC_COUNT];
			bool outOfDate;
		} mAttributes;

		struct DrawData
		{
			GLenum mode;
			GLenum type;
			uint32 offset;
			uint32 count;
			uint32 instances;
		};

	};
}

#endif // !__NGINE_GL_MESH_H_
