
#include "NGineStable.h"
#include "NGineGizmos.h"
#include "NGineLog.h"
#include "NGineRenderCommand.h"
#include <glm/ext.hpp>

namespace NGine
{
	Gizmos::Gizmos(MaterialPtr material, MaterialPtr behindMaterial, uint32 maxLines) :
		Renderable(&typeid(Gizmos)),
		mMaxLines(maxLines),
		mNumLines(0),
		mOutOfDate(true),
		mMaterial(material),
		mBehindMaterial(behindMaterial)
	{
		mLineMesh.getVertexBuffer().resize(sizeof(Line)* maxLines, BU_STREAM);
		mLineMesh.setAttribute(SM_POSITION, true, 3, GL_FLOAT, 0, sizeof(Vertex));
		mLineMesh.setAttribute(SM_COLOR, true, 4, GL_UNSIGNED_BYTE, sizeof(float)*3, sizeof(Vertex), true);
		mLines = new Line[mMaxLines];
	}

	Gizmos::~Gizmos()
	{
		delete[] mLines;
	}

	void Gizmos::addLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& col)
	{
		_addLine(p1, p2, col);
	}

	void Gizmos::addTransform(const glm::mat4& transform, float scale)
	{
		glm::vec3 pos = transform[3].xyz;
		_addLine(pos, pos + transform[0].xyz * scale, glm::vec3(1,0,0));
		_addLine(pos, pos + transform[1].xyz * scale, glm::vec3(0,1,0));
		_addLine(pos, pos + transform[2].xyz * scale, glm::vec3(0,0,1));

	}

	void Gizmos::addCube(const glm::vec3& center, const glm::vec3& extents, const glm::vec3& color)
	{
		glm::vec3 p100(-extents.x, extents.y, extents.z);
		glm::vec3 p101(-extents.x, extents.y, -extents.z);
		glm::vec3 p110(-extents.x, -extents.y, extents.z);
		glm::vec3 p111(-extents.x, -extents.y, -extents.z);
		glm::vec3 p000(extents.x, extents.y, extents.z);
		glm::vec3 p001(extents.x, extents.y, -extents.z);
		glm::vec3 p010(extents.x, -extents.y, extents.z);
		glm::vec3 p011(extents.x, -extents.y, -extents.z);
		p100 += center;
		p101 += center;
		p110 += center;
		p111 += center;
		p000 += center;
		p001 += center;
		p010 += center;
		p011 += center;

		_addLine(p100, p101, color);
		_addLine(p110, p111, color);
		_addLine(p100, p110, color);
		_addLine(p101, p111, color);

		_addLine(p000, p001, color);
		_addLine(p010, p011, color);
		_addLine(p000, p010, color);
		_addLine(p001, p011, color);

		_addLine(p000, p100, color);
		_addLine(p010, p110, color);
		_addLine(p011, p111, color);
		_addLine(p001, p101, color);
	}

	void Gizmos::addCube(const glm::mat4& transform, const glm::vec3& color)
	{
		glm::vec3 extents(1,1,1);

		glm::vec3 p100(-extents.x, extents.y, extents.z);
		glm::vec3 p101(-extents.x, extents.y, -extents.z);
		glm::vec3 p110(-extents.x, -extents.y, extents.z);
		glm::vec3 p111(-extents.x, -extents.y, -extents.z);
		glm::vec3 p000(extents.x, extents.y, extents.z);
		glm::vec3 p001(extents.x, extents.y, -extents.z);
		glm::vec3 p010(extents.x, -extents.y, extents.z);
		glm::vec3 p011(extents.x, -extents.y, -extents.z);
		p100 = glm::vec3(transform * glm::vec4(p100, 0));
		p101 = glm::vec3(transform * glm::vec4(p101, 0));
		p110 = glm::vec3(transform * glm::vec4(p110, 0));
		p111 = glm::vec3(transform * glm::vec4(p111, 0));
		p000 = glm::vec3(transform * glm::vec4(p000, 0));
		p001 = glm::vec3(transform * glm::vec4(p001, 0));
		p010 = glm::vec3(transform * glm::vec4(p010, 0));
		p011 = glm::vec3(transform * glm::vec4(p011, 0));

		_addLine(p100, p101, color);
		_addLine(p110, p111, color);
		_addLine(p100, p110, color);
		_addLine(p101, p111, color);

		_addLine(p000, p001, color);
		_addLine(p010, p011, color);
		_addLine(p000, p010, color);
		_addLine(p001, p011, color);

		_addLine(p000, p100, color);
		_addLine(p010, p110, color);
		_addLine(p011, p111, color);
		_addLine(p001, p101, color);
	}

	void Gizmos::addSphere(const glm::vec3& center, float rad, const glm::vec3& color)
	{
		const float incr = (glm::pi<float>() * 2) / 32;

		{
			glm::vec3 prev(glm::sin(0.0f) * rad, glm::cos(0.0f) * rad, 0);
			for (float i = 0; i < (glm::pi<float>() * 2); i += incr)
			{
				glm::vec3 next(glm::sin(i + incr) * rad, glm::cos(i + incr) * rad, 0);
				_addLine(prev + center, next + center, color);
				prev = next;
			}
		}

		{
			glm::vec3 prev(glm::sin(0.0f) * rad, 0, glm::cos(0.0f) * rad);
			for (float i = 0; i < (glm::pi<float>() * 2); i += incr)
			{
				glm::vec3 next(glm::sin(i + incr) * rad, 0, glm::cos(i + incr) * rad);
				_addLine(prev + center, next + center, color);
				prev = next;
			}
		}

		{
			glm::vec3 prev(0, glm::sin(0.0f) * rad, glm::cos(0.0f) * rad);
			for (float i = 0; i < (glm::pi<float>() * 2); i += incr)
			{
				glm::vec3 next(0, glm::sin(i + incr) * rad, glm::cos(i + incr) * rad);
				_addLine(prev + center, next + center, color);
				prev = next;
			}
		}
		
	}

	void Gizmos::_addLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		if (mNumLines >= mMaxLines)
			return;

		mOutOfDate = true;
		Line& line = mLines[mNumLines++];
		line.v0.position = p1;
		line.v1.position = p2;
		
		line.v0.color.r = color.r * 255;
		line.v0.color.g = color.g * 255;
		line.v0.color.b = color.b * 255;
		line.v0.color.a = 1 * 255;

		line.v1.color.r = color.r * 255;
		line.v1.color.g = color.g * 255;
		line.v1.color.b = color.b * 255;
		line.v1.color.a = 1 * 255;

	}

	void Gizmos::clear()
	{
		mOutOfDate = true;
		mNumLines = 0;
	}

	void Gizmos::_extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera)
	{
		if (mOutOfDate)
		{
			mOutOfDate = false;
			// Upload the data to the buffer
			size_t size = sizeof(Line)* mNumLines;
			void* data = mLineMesh.getVertexBuffer().lock(0, size);
			if (data) memcpy(data, mLines, size);
			mLineMesh.getVertexBuffer().unlock();
		}

		DrawCall drawCall;
		drawCall.indexType = GL_UNSIGNED_INT; // Not using this
		
		drawCall.glMesh = &mLineMesh;
		drawCall.drawMode = GL_LINES;
		drawCall.count = mNumLines * 2;
		drawCall.offset = 0;
		
		// Needs parent for transform matrix
		drawCall.parent = getParent();
		drawCall.material = mMaterial;

		if (mMaterial)
		{
			drawCall.sortKey = mMaterial->_generateSortKey(0, 0);
			cachedDrawCallList.push_back(drawCall);
		}

		if (mBehindMaterial)
		{
			drawCall.material = mBehindMaterial;
			drawCall.sortKey = mBehindMaterial->_generateSortKey(0, 0);
			cachedDrawCallList.push_back(drawCall);
		}
	}


}