#ifndef __NGINE_GIZMOS_H_
#define __NGINE_GIZMOS_H_

#include "NGineCommon.h"
#include "NGineGLMesh.h"
#include "NGineComponent.h"
#include "NGineMaterial.h"

namespace NGine
{
	class Gizmos : public Renderable
	{
	public:
		Gizmos(MaterialPtr material, MaterialPtr behindMaterial = MaterialPtr(), uint32 maxLines = 16384U);
		~Gizmos();

		void clear();
		
		void addLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& col);
		void addTransform(const glm::mat4& transform, float scale = 1.0f);
		void addCube(const glm::vec3& center, const glm::vec3& extents, const glm::vec3& color);
		void addCube(const glm::mat4& transform, const glm::vec3& color);
		void addSphere(const glm::vec3& center, float radius, const glm::vec3& color);

		virtual void _extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera);
		
	private:
		void _addLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
		
		struct Vertex
		{
			glm::vec3 position;
			struct { uint8 r, g, b, a; } color;
		};

		struct Line
		{
			Vertex v0;
			Vertex v1;
		};

		uint32 mMaxLines;
		uint32 mNumLines;
		Line* mLines;
		GLMesh mLineMesh;
		MaterialPtr mMaterial;
		MaterialPtr mBehindMaterial;

		bool mOutOfDate;

	};
}

#endif // !__NGINE_GIZMOS_H_
