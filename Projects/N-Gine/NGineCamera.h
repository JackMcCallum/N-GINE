#ifndef __NGINE_CAMERA_H_
#define __NGINE_CAMERA_H_

#include "NGineCommon.h"
#include "NGineComponent.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace NGine
{
	class Scene;
	class AABB;

	class Camera : public Component
	{
	public:
		Camera();
		void setProjection(float fov = 50.0f, float aspect = 1.0f, float nearclip = 0.1f, float farclip = 1024.0f, glm::vec2 offset = glm::vec2(0.0f));
		void setOrtho(float left = 50.0f, float right = -50.0f, float bottom = 50.0f, float top = -50.0f, float nearclip = 0.0f, float farclip = 1024.0f);

		const glm::mat4& getProjMatrix() const;

		float getFarClip() const;
		float getNearClip() const;

		bool canSee(const AABB& aabb) const;

	private:
		glm::mat4 mProjMatrix;
		float mFarClip;
		float mNearClip;
	};








	class CameraOld
	{
	public:
		CameraOld(Scene* scene = nullptr);

		// Projection Matrix
		// Field of view in degrees
		// Offset for skewing the projection
		void setProjection(float fov = 50.0f, float aspect = 1.0f, float nearclip = 0.1f, float farclip = 1024.0f, glm::vec2 offset = glm::vec2(0.0f));
		void setOrtho(float left = 50.0f, float right = -50.0f, float bottom = 50.0f, float top = -50.0f, float nearclip = 0.0f, float farclip = 1024.0f);

		// Set/Get the position of the camera
		void setPosition(glm::vec3 pos);
		glm::vec3 getPosition();

		// Set/get the orientation of the camera
		void setOrientation(glm::quat orient);
		glm::quat getOrientation();

		void lookAt(glm::vec3 lookat, glm::vec3 up = glm::vec3(0,1,0));

		// Translate the camera in local space
		void translateLocal(const glm::vec3& vec);

		// Translate the camera in world space
		void translateGlobal(const glm::vec3& vec);

		// Rotate the camera in local space
		void yawLocal(float deg);
		void pitchLocal(float deg);
		void rollLocal(float deg);

		// Rotate the camera in world space
		void yawGlobal(float deg);
		void pitchGlobal(float deg);
		void rollGlobal(float deg);

		// Get the matrices
		const glm::mat4& getViewMatrix();
		const glm::mat4& getProjMatrix();
		const glm::mat4& getCamMatrix();

		// Get the vectors of the camera matrix
		glm::vec3 getForward();
		glm::vec3 getRight();
		glm::vec3 getUp();

		Scene* getScene() const;

	private:
		void _validate();
		void _invalidate();

	private:
		glm::quat mOrientation;
		glm::vec3 mPosition;

		glm::mat4 mProjMatrix;
		glm::mat4 mCamMatrix;
		glm::mat4 mViewMatrix;

		bool mInvalid;
		Scene* mScene;
	};

};

#endif