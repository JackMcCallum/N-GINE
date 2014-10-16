#ifndef __NGINE_LIGHT_H_
#define __NGINE_LIGHT_H_

#include "NGineComponent.h"

namespace NGine
{
	class Light : public Renderable
	{
	public:
		Light(const std::type_info* typeId);
		~Light();
		
		void setColor(const glm::vec3& color);
		const glm::vec3& getColor() const;

		virtual void _extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera) {}

	private:
		glm::vec3 mColor;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const glm::vec3& direction);
		DirectionalLight();
		~DirectionalLight();

		void setDirection(const glm::vec3& direction);
		const glm::vec3& getDirection() const;

	private:
		glm::vec3 mDirection;
	};

	class PointLight : public Light
	{
	public:
		PointLight();
		~PointLight();

		void setPosition(const glm::vec3& position);
		const glm::vec3& getPosition() const;

		void setAttenuation(float attenuation);
		float getAttenuation() const;

	private:
		float mAttenuation;
		glm::vec3 mPosition;
	};

}

#endif // !__NGINE_LIGHT_H_
