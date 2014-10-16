#include "NGineStable.h"
#include "NGineLight.h"

namespace NGine
{

	Light::Light(const std::type_info* typeId) :
		Renderable(typeId),
		mColor(1, 1, 1) // White by default
	{
	}

	Light::~Light()
	{
	}

	void Light::setColor(const glm::vec3& color)
	{
		mColor = color;
	}

	const glm::vec3& Light::getColor() const
	{
		return mColor;
	}

	DirectionalLight::DirectionalLight() : 
		Light(&typeid(DirectionalLight))
	{
	}

	DirectionalLight::DirectionalLight(const glm::vec3& direction) :
		Light(&typeid(DirectionalLight))
	{
		setDirection(direction);
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::setDirection(const glm::vec3& direction)
	{
		mDirection = direction;
	}

	const glm::vec3& DirectionalLight::getDirection() const
	{
		return mDirection;
	}

	PointLight::PointLight() :
		Light(&typeid(PointLight))
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::setPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	const glm::vec3& PointLight::getPosition() const
	{
		return mPosition;
	}

	void PointLight::setAttenuation(float attenuation)
	{
		mAttenuation = attenuation;
	}

	float PointLight::getAttenuation() const
	{
		return mAttenuation;
	}

}