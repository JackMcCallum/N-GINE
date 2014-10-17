#ifndef __NGINE_ENTITY_H_
#define __NGINE_ENTITY_H_

#include "NGineCommon.h"
#include "NGineAABB.h"
#include "NGineRenderSystem.h"
#include "NGineSceneNode.h"
#include "NGineResource.h"

namespace NGine
{
	class SceneNode;
	class Renderable;
	class Camera;
	class RenderTarget;

	RESOURCE_PTR(Material);

	class Component
	{
		friend class SceneNode;

	public:
		Component(const std::type_info* typeId, bool isRenderable = false);
		virtual ~Component();

		SceneNode* getParent() const;
		const std::type_info* getTypeInfo() const;
		
		Renderable* getRenderable();

	protected:
		SceneNode* mParent;
		const std::type_info* mTypeInfo;
		bool mIsRenderable;
	};

	class Renderable : public Component
	{
		friend class RenderTarget;
	public:
		// used to supply a draw call to the render target which will then go into sorting and batching
		struct DrawCall
		{
			union SortKey
			{
				uint64 key;

				struct OpaqueBits
				{
					// Not used
					uint64 padding : 3;
					uint64 translucency : 2;

					// Depth value. front to back for opaque / back to front for transparent
					uint64 depth : 24;
					
					// Prioritize over the depth for opaque geometry so we can get more batched calls
					// mat->getID(); each material has a unique ID
					uint64 submeshID : 16;
					uint64 materialID : 16;

					// Used for forcing draw calls onto other layers
					// opaque1/reserved/background/transparent/reserved/GUI/reserved/reserved
					uint64 layer : 3;
				} opaque;

				struct TransparentBits
				{
					// Not used
					uint64 padding : 3;
					uint64 translucency : 2;

					// Unique IDs to help batch draw calls
					uint64 submeshID : 16;
					uint64 materialID : 16;

					// Depth value. front to back for opaque / back to front for transparent
					uint64 depth : 24;

					// Used for forcing draw calls onto other layers
					// opaque1/reserved/background/transparent/reserved/GUI/reserved/reserved
					uint64 layer : 3;

				} transparent;
			};

			// Operator for helping with batching
			inline bool operator == (const DrawCall& other)
			{
				if (glMesh == other.glMesh &&
					drawMode == other.drawMode &&
					indexType == other.indexType &&
					count == other.count &&
					offset == other.offset &&
					material == other.material)
					return true;
				return false;
			}

			// A pointer to the mesh, this must not be null
			GLMesh* glMesh;
			
			// The draw info of that mesh
			GLenum drawMode;
			GLenum indexType;
			GLuint count;
			GLuint offset;
			
			// A sort key, this is how the order of draw calls is controlled
			SortKey sortKey;

			// A pointer to the parent node so the matrix can be extract,
			// If left nullptr then it will use an identity matrix
			SceneNode* parent;

			// A pointer to the material that will be set active before the draw call
			MaterialPtr material;
		};

	public:
		Renderable(const std::type_info* typeId);
		virtual ~Renderable();

		virtual void _extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera) = 0;

	};

}

#endif // !__NGINE_ENTITY_H_
