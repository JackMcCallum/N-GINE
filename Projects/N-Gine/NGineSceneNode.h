#ifndef __NGINE_SCENE_NODE_H_
#define __NGINE_SCENE_NODE_H_

#include "NGineCommon.h"
#include "NGineAABB.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace NGine
{
	class Component;
	class Renderable;
	class Camera;

	enum class Space
	{
		LOCAL,
		WORLD,
		PARENT,
	};

	class SceneNode
	{
	public:
		friend class Component;

		// ========================================================================
		// ==== Construction & Destruction
		
		SceneNode(const std::string& name,
			glm::vec3 pos = glm::vec3(),
			glm::vec3 scale = glm::vec3(1),
			glm::quat orient = glm::quat());

		virtual ~SceneNode();

		// ========================================================================
		// ==== Naming and organizing

		// Return the name as a string
		const std::string& getName() const;
		// Return the hash value of the name
		uint32 getNameHash() const;


		// ========================================================================
		// ==== Handling children and parents

		// Create a child of this node
		SceneNode* createChild(const std::string& name,
			glm::vec3 pos = glm::vec3(),
			glm::vec3 scale = glm::vec3(1),
			glm::quat orient = glm::quat());
		// Add an existing child to this node
		void addChild(SceneNode* node);
		// Remove a child by node
		void removeChild(SceneNode* node);
		// Remove a child by name
		void removeChild(const std::string& name, bool useHash = true);
		// Destroy a child by pointer
		void destroyChild(SceneNode* node);
		// Destroy a child by name
		void destroyChild(const std::string& name, bool useHash = true);
		// Destroy all children attached to this
		void destroyAllChildren();
		// Get child by name
		SceneNode* getChild(const std::string& name, bool useHash = true);
		// Return a const vector to the children of this node
		const std::vector<SceneNode*>& getChildren() const;
		

		template<typename T>
		T* getComponent(bool recursive = false);
		template<typename T>
		T* getComponent(const std::type_info* type, bool recursive = false);

		template<typename T>
		std::vector<T*>& getComponents(std::vector<T*>& list, bool recursive = false);
		template<typename T>
		std::vector<T*>& getComponents(const std::type_info* type, std::vector<T*>& list, bool recursive = false);

		const std::vector<Component*>& getComponents() const;
		void attachComponent(Component* component);
		bool detachComponent(Component* component);

		void destroyAllComponents();

		// Method used by the engine to obtain all the renderables in a scene for rendering
		void _getRenderables(std::vector<Renderable*>& list, Camera* camera = nullptr, bool recursive = true);

		// Gets the root node for this node
		SceneNode* getRootNode();

		// Return true if you want to iterate the nodes children, or false to skip to the next sibling
		typedef std::function<bool(SceneNode*)> RecurseFunction;
		void recursivly(RecurseFunction function);

		// ========================================================================
		// ==== Manual transform data
		
		// Set the local position
		void setPosition(const glm::vec3 val, bool inherited = true);
		// Get the local position
		const glm::vec3& getPosition() const;
		// Get the derived position
		const glm::vec3& getGlobalPosition() const;
		
		// Set the local scale
		void setScale(const glm::vec3 val, bool inherited = true);
		// Get the local scale
		const glm::vec3& getScale() const;
		// Get the derived scale
		const glm::vec3& getGlobalScale() const;

		// Set the local orientation
		void setOrientation(const glm::quat val, bool inherited = true);
		// Get the local orientation
		const glm::quat& getOrientation() const;
		// Get the derived orientation
		const glm::quat& getGlobalOrientation() const;

		// Get transform matrices
		const glm::mat4& getGlobalTransform() const;
		const glm::mat4& getLocalTransform() const;

		// @to-do
		// glm::vec3 getGlobalForward();
		// glm::vec3 getGlobalRight();
		// glm::vec3 getGlobalUp();


		// ========================================================================
		// ==== Motion and rotation

		// Scale the scene node relative previous scale
		
		// Translate the scene node
		void translate(const glm::vec3& move, Space space = Space::WORLD);
		// Rotate the scene node by quaternion
		void rotate(const glm::quat& rot, Space space = Space::WORLD);
		// Rotate the scene node on a given axis by an amount of radians
		void rotate(const glm::vec3& axis, float radians, Space space = Space::WORLD);
		// Yaw the scene node by an amount of radians
		void yaw(float radians, Space space = Space::WORLD);
		// Pitch the scene node by an amount of radians
		void pitch(float radians, Space space = Space::WORLD);
		// Roll the scene node by an amount of radians
		void roll(float radians, Space space = Space::WORLD);
		// Look at a specified point
		void lookAt(const glm::vec3& lookat, const glm::vec3& up = glm::vec3(0,1,0), Space space = Space::WORLD);

		// Get a reference to the bounds for this node
		const AABB& getAABB() const;

	private:
		void _notifyNeedsUpdate() const;
		void _notifyAABBNeedsUpdate() const;
		void _setProperty(uint32 prop, bool val) const;
		bool _getProperty(uint32 prop) const;
		
		void _updateFromParent() const;
		void _updateBounds() const;

	private:
		std::string mName;
		uint32 mNameHash;

		// Structure
		SceneNode* mParent;
		std::vector<SceneNode*> mChildren;

		// Component attachments
		std::vector<Component*> mComponents;

		// Local transformation
		glm::vec3 mLocalPosition;
		glm::vec3 mLocalScale;
		glm::quat mLocalOrientation;
		mutable glm::mat4 mLocalTransform;

		// Global transformation
		mutable glm::vec3 mGlobalPosition;
		mutable glm::vec3 mGlobalScale;
		mutable glm::quat mGlobalOrientation;
		mutable glm::mat4 mGlobalTransform;

		enum Properties
		{
			UPDATE_GLOBAL_MATRIX = (1 << 0),
			UPDATE_LOCAL_MATRIX = (1 << 1),
			UPDATE_FROM_PARENT = (1 << 2),
			UPDATE_BOUNDS = (1 << 3),
			UPDATE_ALL = UPDATE_GLOBAL_MATRIX | UPDATE_LOCAL_MATRIX | UPDATE_FROM_PARENT | UPDATE_BOUNDS,

			INHERIT_POSITION = (1 << 10),
			INHERIT_SCALE = (1 << 11),
			INHERIT_ORIENTATION = (1 << 12),
		};
		mutable uint32 mProperties;
		
		// Bounding volume
		mutable AABB mAABB;
		mutable AABB mLocalAABB;

	};

	template<typename T>
	T* SceneNode::getComponent(bool recursive)
	{
		return getComponent<T>(&typeid(T), recursive);
	}

	template<typename T>
	T* SceneNode::getComponent(const std::type_info* type, bool recursive)
	{
		// Find if the requested component is in the list
		for (auto iter : mComponents)
		{
			if (iter->getTypeInfo() == type)
				return (T*)iter;
		}

		// If requested recursive, check children
		if (recursive)
		{
			for (auto iter : mChildren)
			{
				T* ptr = getComponent<T>(type, recursive);
				if (ptr) return ptr;
			}
		}
	}

	template<typename T>
	std::vector<T*>& SceneNode::getComponents(std::vector<T*>& list, bool recursive)
	{
		return getComponents<T>(&typeid(T), list, recursive);
	}

	template<typename T>
	std::vector<T*>& SceneNode::getComponents(const std::type_info* type, std::vector<T*>& list, bool recursive)
	{
		// Find if the requested component is in the list
		for (auto iter : mComponents)
		{
			if (iter->getTypeInfo() == type)
				list.push_back((T*)iter);
		}

		// If requested recursive, check children
		if (recursive)
		{
			for (auto iter : mChildren)
				getComponents<T>(type, recursive);
		}

		return list;
	}

}

#endif // !__NGINE_SCENE_NODE_H_


