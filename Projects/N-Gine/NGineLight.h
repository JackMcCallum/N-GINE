#ifndef __NGINE_LIGHT_H_
#define __NGINE_LIGHT_H_

#include "NGineComponent.h"

namespace NGine
{
	class Light : public Renderable
	{
	public:
		enum Type
		{
			POINT,
			DIRECTIONAL,
		};

		Light(const std::type_info* typeId, Type type);
		~Light();
		
		void setColor(const glm::vec3& color);
		const glm::vec3& getColor() const;

		virtual void _extractDrawCalls(std::vector<DrawCall>& cachedDrawCallList, const Camera& camera) {}

		inline Type getType() const { return mType; }

	private:
		glm::vec3 mColor;
		Type mType;
	};

	class DirectionalLight : public Light, public TPoolAllocator<DirectionalLight>
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

	class PointLight : public Light, public TPoolAllocator<PointLight>
	{
	public:
		PointLight(float attenuation = 16);
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
