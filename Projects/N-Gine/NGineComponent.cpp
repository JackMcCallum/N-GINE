#include "NGineStable.h"
#include "NGineComponent.h"

namespace NGine
{

	Component::Component(const std::type_info* typeInfo, bool isRenderable) :
		mParent(nullptr),
		mTypeInfo(typeInfo),
		mIsRenderable(isRenderable)
	{
	}

	Component::~Component()
	{
		if (mParent)
		{
			mParent->detachComponent(this);
			mParent = nullptr;
		}
	}

	SceneNode* Component::getParent() const
	{
		return mParent;
	}

	const std::type_info* Component::getTypeInfo() const
	{
		return mTypeInfo;
	}

	Renderable* Component::getRenderable()
	{
		if (mIsRenderable)
			return static_cast<Renderable*>(this);
		else
			return nullptr;
	}

	Renderable::Renderable(const std::type_info* typeId) : Component(typeId, true)
	{

	}

	Renderable::~Renderable()
	{

	}

}