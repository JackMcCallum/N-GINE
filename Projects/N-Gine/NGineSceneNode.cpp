#include "NGineStable.h"
#include "NGineSceneNode.h"
#include "NGineUtilities.h"
#include "NGineCamera.h"
#include "NGineComponent.h"

namespace NGine
{
	SceneNode::SceneNode(const std::string& name, glm::vec3 pos, glm::vec3 scale, glm::quat orient) :
		mLocalPosition(pos),
		mLocalScale(scale),
		mLocalOrientation(orient),
		mParent(nullptr),
		mName(name),
		mProperties(0),
		mNameHash(Utilities::hash(name))
	{
		mNameHash = 0;
		_setProperty(INHERIT_POSITION | INHERIT_SCALE | INHERIT_ORIENTATION |
			UPDATE_GLOBAL_MATRIX | UPDATE_LOCAL_MATRIX | UPDATE_FROM_PARENT, true);
	}

	SceneNode::~SceneNode()
	{
		destroyAllComponents();
		destroyAllChildren();

		if (mParent)
			mParent->removeChild(this);

	}

	const std::string& SceneNode::getName() const
	{
		return mName;
	}

	uint32 SceneNode::getNameHash() const
	{
		return mNameHash;
	}

	SceneNode* SceneNode::createChild(const std::string& name, glm::vec3 pos, glm::vec3 scale, glm::quat orient)
	{
		SceneNode* node = new SceneNode(name, pos, scale, orient);
		if (node) addChild(node);
		return node;
	}

	void SceneNode::addChild(SceneNode* node)
	{
		// Remove from previous if needed
		if (node->mParent)
			node->mParent->removeChild(node);
		
		node->mParent = this;
		mChildren.push_back(node);
	}

	void SceneNode::removeChild(SceneNode* node)
	{
		for (auto iter = mChildren.begin(); iter != mChildren.end(); iter++)
		{
			if (*iter == node)
			{
				node->mParent = nullptr;
				mChildren.erase(iter);
				break;
			}
		}
	}

	void SceneNode::removeChild(const std::string& name, bool useHash)
	{
		SceneNode* node = nullptr;
		if (node = getChild(name, useHash))
			removeChild(node);
	}

	void SceneNode::destroyChild(SceneNode* node)
	{
		for (auto iter = mChildren.begin(); iter != mChildren.end(); iter++)
		{
			if (*iter == node)
			{
				node->mParent = nullptr;
				mChildren.erase(iter);
				delete node;
				break;
			}
		}
	}

	void SceneNode::destroyChild(const std::string& name, bool useHash)
	{
		SceneNode* node = nullptr;
		if (node = getChild(name, useHash))
			destroyChild(node);
	}

	void SceneNode::destroyAllChildren()
	{
		for (auto iter : mChildren)
		{
			iter->mParent = nullptr;
			iter->destroyAllChildren();
			delete iter;
		}
		mChildren.clear();
	}

	SceneNode* SceneNode::getChild(const std::string& name, bool useHash)
	{
		if (useHash)
		{
			uint32 hash = Utilities::hash(name);

			for (auto iter : mChildren)
			{
				if (iter->mNameHash == hash)
					return iter;
			}
		}
		else
		{
			for (auto iter : mChildren)
			{
				if (iter->mName == name)
					return iter;
			}
		}
	}

	const std::vector<SceneNode*>& SceneNode::getChildren() const
	{
		return mChildren;
	}

	void SceneNode::setPosition(const glm::vec3 val, bool inherited)
	{
		mLocalPosition = val;
		_setProperty(INHERIT_POSITION, inherited);
		_notifyNeedsUpdate();
		_notifyAABBNeedsUpdate();
	}

	const glm::vec3& SceneNode::getPosition() const
	{
		return mLocalPosition;
	}

	const glm::vec3& SceneNode::getGlobalPosition() const
	{
		_updateFromParent();
		return mGlobalPosition;
	}

	void SceneNode::setScale(const glm::vec3 val, bool inherited)
	{
		mLocalScale = val;
		_setProperty(INHERIT_SCALE, inherited);
		_notifyNeedsUpdate();
		_notifyAABBNeedsUpdate();
	}

	const glm::vec3& SceneNode::getScale() const
	{
		return mLocalScale;
	}

	const glm::vec3& SceneNode::getGlobalScale() const
	{
		_updateFromParent();
		return mGlobalScale;
	}

	void SceneNode::setOrientation(const glm::quat val, bool inherited)
	{
		mLocalOrientation = glm::normalize(val);
		_setProperty(INHERIT_ORIENTATION, inherited);
		_notifyNeedsUpdate();
		_notifyAABBNeedsUpdate();
	}

	const glm::quat& SceneNode::getOrientation() const
	{
		return mLocalOrientation;
	}

	const glm::quat& SceneNode::getGlobalOrientation() const
	{
		_updateFromParent();
		return mGlobalOrientation;
	}

	const glm::mat4& SceneNode::getGlobalTransform() const
	{
		if (_getProperty(UPDATE_GLOBAL_MATRIX))
		{
			_updateFromParent();
			mGlobalTransform = glm::scale(glm::toMat4(mGlobalOrientation), mGlobalScale);
			mGlobalTransform[3].xyz = mGlobalPosition;
			_setProperty(UPDATE_GLOBAL_MATRIX, false);
		}
		return mGlobalTransform;
	}

	const glm::mat4& SceneNode::getLocalTransform() const
	{
		if (_getProperty(UPDATE_LOCAL_MATRIX))
		{
			mLocalTransform = glm::scale(glm::toMat4(mLocalOrientation), mLocalScale);
			mLocalTransform[3].xyz = mLocalPosition;
			_setProperty(UPDATE_LOCAL_MATRIX, false);
		}
		return mLocalTransform;
	}

	void SceneNode::translate(const glm::vec3& move, Space space)
	{
		switch (space)
		{
		case Space::LOCAL:
			mLocalPosition += mLocalOrientation * move;
			break;
		case Space::WORLD:
			if (mParent)
			{
				mLocalPosition += (glm::inverse(mParent->getGlobalOrientation()) * move) / mParent->mGlobalScale;
			}
			else
			{
				mLocalPosition += move;
			}
			break;
		case Space::PARENT:
			mLocalPosition += move;
			break;
		}
		_notifyNeedsUpdate();
	}

	void SceneNode::rotate(const glm::quat& rot, Space space)
	{
		switch (space)
		{
		case Space::LOCAL:
			mLocalOrientation = mLocalOrientation * glm::normalize(rot);
			break;
		case Space::WORLD:
			_updateFromParent();
			mLocalOrientation = mLocalOrientation * glm::inverse(getGlobalOrientation()) * glm::normalize(rot) * mGlobalOrientation;
			break;
		case Space::PARENT:
			mLocalOrientation = glm::normalize(rot) * mLocalOrientation;
			break;
		}
		_notifyNeedsUpdate();
	}

	void SceneNode::rotate(const glm::vec3& axis, float radians, Space space)
	{
		rotate(glm::angleAxis(radians, axis), space);
	}

	void SceneNode::yaw(float radians, Space space)
	{
		rotate(glm::vec3(0, 1, 0), radians, space);
	}

	void SceneNode::pitch(float radians, Space space)
	{
		rotate(glm::vec3(1, 0, 0), radians, space);
	}

	void SceneNode::roll(float radians, Space space)
	{
		rotate(glm::vec3(0, 0, 1), radians, space);
	}

	void SceneNode::lookAt(const glm::vec3& lookat, const glm::vec3& up, Space space)
	{
		assert(false);
		
		//glm::vec3 xAxis, yAxis, zAxis;
		//zAxis = glm::normalize(mLocalPosition - lookat);
		//xAxis = glm::normalize(glm::cross(up, zAxis));
		//yAxis = glm::cross(zAxis, xAxis);
		//
		//glm::mat4 mat;
		//mat[0] = glm::vec4(xAxis, 0);
		//mat[1] = glm::vec4(yAxis, 0);
		//mat[2] = glm::vec4(zAxis, 0);
		//mat[3] = glm::vec4(0, 0, 0, 1);

		//mLocalOrientation = glm::toQuat(mat);
	}

	void SceneNode::_notifyNeedsUpdate() const
	{
		_setProperty(UPDATE_ALL, true);

		for (auto iter : mChildren)
			iter->_setProperty(UPDATE_ALL, true);
	}

	void SceneNode::_notifyAABBNeedsUpdate() const
	{
		_setProperty(UPDATE_BOUNDS, true);
		if (mParent) mParent->_notifyAABBNeedsUpdate();
	}

	void SceneNode::_updateFromParent() const
	{
		if (_getProperty(UPDATE_FROM_PARENT))
		{
			if (mParent)
			{
				_setProperty(UPDATE_FROM_PARENT, false);

				const glm::quat& parentOrient = mParent->getGlobalOrientation();
				const glm::vec3& parentPosition = mParent->getGlobalPosition();
				const glm::vec3& parentScale = mParent->getGlobalScale();

				if (_getProperty(INHERIT_ORIENTATION))
					mGlobalOrientation = glm::normalize(parentOrient * mLocalOrientation);
				else
					mGlobalOrientation = glm::normalize(mGlobalOrientation);;
				

				if (_getProperty(INHERIT_SCALE))
					mGlobalScale = parentScale * mLocalScale;
				else
					mGlobalScale = mLocalScale;

				if (_getProperty(INHERIT_POSITION))
					mGlobalPosition = parentPosition + parentOrient * (parentScale * mLocalPosition);
				else
					mGlobalPosition = mLocalPosition;
			}
			else
			{
				mGlobalPosition = mLocalPosition;
				mGlobalScale = mLocalScale;
				mGlobalOrientation = mLocalOrientation;
			}
		}
	}

	void SceneNode::_setProperty(uint32 prop, bool val) const
	{
		if (val)
			mProperties |= prop;
		else
			mProperties &= ~prop;
	}

	bool SceneNode::_getProperty(uint32 prop) const
	{
		return (mProperties & prop) > 0;
	}

	const AABB& SceneNode::getAABB() const
	{
		_updateBounds();
		return mAABB;
	}

	void SceneNode::_updateBounds() const
	{
		if (_getProperty(UPDATE_BOUNDS))
		{
			for (auto iter : mChildren)
				mAABB.merge(iter->getAABB());
			
			_setProperty(UPDATE_BOUNDS, false);
		}
	}

	const std::vector<Component*>& SceneNode::getComponents() const
	{
		return mComponents;
	}

	void SceneNode::attachComponent(Component* component)
	{
		assert(component);
		if (!component->mParent)
		{
			mComponents.push_back(component);
			component->mParent = this;
		}
		else
		{
			component->mParent->detachComponent(component);
			component->mParent = nullptr;
			attachComponent(component);
		}
	}

	bool SceneNode::detachComponent(Component* component)
	{
		for (auto iter = mComponents.begin(); iter != mComponents.end(); iter++)
		{
			if (*iter == component)
			{
				mComponents.erase(iter);
				return true;
			}
		}
		return false;
	}

	void SceneNode::destroyAllComponents()
	{
		for (auto iter : mComponents)
		{
			iter->mParent = nullptr;
			delete iter;
		}
		mComponents.clear();
	}

	SceneNode* SceneNode::getRootNode()
	{
		if (mParent) 
			return mParent->getRootNode();
		return this;
	}

	void SceneNode::_getRenderables(std::vector<Renderable*>& list, Camera* camera, bool recursive)
	{
		//if (camera && !camera->canSee(getAABB()));
		//	return;

		for (auto iter : mComponents)
		{
			Renderable* renderable = iter->getRenderable();
			if (renderable) list.push_back(renderable);
		}

		if (recursive)
		{
			for (auto iter : mChildren)
				iter->_getRenderables(list, camera, recursive);
		}
	}

	void SceneNode::recursivly(RecurseFunction function)
	{
		if (function(this))
		{
			for (auto iter : mChildren)
				iter->recursivly(function);
		}
	}

}
