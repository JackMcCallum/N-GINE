#include "NGineStable.h"
#include "NGineCamera.h"

namespace NGine
{

	Camera::Camera() :
		Component(&typeid(Camera))
	{
		setProjection();
	}

	void Camera::setProjection(float fov, float aspect, float nearclip, float farclip, glm::vec2 offset)
	{
		mProjMatrix = glm::perspective(glm::radians(fov), aspect, nearclip, farclip);
		// Offset the Z row X and Y to skew the cameras vision
		mProjMatrix[2].xy += offset;
		mFarClip = farclip;
		mNearClip = nearclip;
	}

	void Camera::setOrtho(float left, float right, float bottom, float top, float nearclip, float farclip)
	{
		mProjMatrix = glm::ortho(left, right, bottom, top, nearclip, farclip);
	}

	const glm::mat4& Camera::getProjMatrix() const
	{
		return mProjMatrix;
	}

	bool Camera::canSee(const AABB& aabb) const
	{
		return true;
	}

	float Camera::getFarClip() const
	{
		return mFarClip;
	}

	float Camera::getNearClip() const
	{
		return mNearClip;
	}








	CameraOld::CameraOld(Scene* scene) :
		mScene(scene)
	{
		setProjection();
		setPosition(glm::vec3(10,10,10));
		lookAt(glm::vec3(0,0,0));
	}

	void CameraOld::setProjection(float fov, float aspect, float nearclip, float farclip, glm::vec2 offset)
	{
		mProjMatrix = glm::perspective(glm::radians(fov), aspect, nearclip, farclip);
		// Offset the Z row X and Y to skew the cameras vision
		mProjMatrix[2].xy += offset;
		_invalidate();
	}

	void CameraOld::setOrtho(float left, float right, float bottom, float top, float nearclip, float farclip)
	{
		mProjMatrix = glm::ortho(left, right, bottom, top, nearclip, farclip);
		_invalidate();
	}
	
	void CameraOld::translateLocal(const glm::vec3& vec)
	{
		glm::vec3 trans = mOrientation * vec;
		mPosition += trans;
		_invalidate();
	}

	void CameraOld::translateGlobal(const glm::vec3& vec)
	{
		mPosition += vec;
		_invalidate();
	}

	void CameraOld::yawLocal(float deg)
	{
		glm::quat rot = glm::angleAxis(glm::radians(deg), mOrientation * glm::vec3(0,1,0));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	void CameraOld::pitchLocal(float deg)
	{
		glm::quat rot = glm::angleAxis(glm::radians(deg), mOrientation * glm::vec3(1,0,0));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	void CameraOld::rollLocal(float deg)
	{
		glm::quat rot = glm::angleAxis(deg, mOrientation * glm::vec3(0,0,1));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	void CameraOld::yawGlobal(float deg)
	{
		glm::quat rot = glm::angleAxis(glm::radians(deg), glm::vec3(0,1,0));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	void CameraOld::pitchGlobal(float deg)
	{
		glm::quat rot = glm::angleAxis(glm::radians(deg), glm::vec3(1,0,0));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	void CameraOld::rollGlobal(float deg)
	{
		glm::quat rot = glm::angleAxis(glm::radians(deg), glm::vec3(0,0,1));
		mOrientation = rot * mOrientation;
		mOrientation = glm::normalize(mOrientation);
		_invalidate();
	}

	const glm::mat4& CameraOld::getViewMatrix()
	{
		_validate();
		return mViewMatrix;
	}

	const glm::mat4& CameraOld::getProjMatrix()
	{
		_validate();
		return mProjMatrix;
	}

	const glm::mat4& CameraOld::getCamMatrix()
	{
		_validate();
		return mCamMatrix;
	}

	void CameraOld::setPosition(glm::vec3 pos)
	{
		mPosition = pos;
		_invalidate();
	}

	void CameraOld::lookAt(glm::vec3 lookat, glm::vec3 up)
	{
		glm::vec3 xAxis, yAxis, zAxis;
		zAxis = glm::normalize(mPosition-lookat);
		xAxis = glm::normalize(glm::cross(up, zAxis));
		yAxis = glm::cross(zAxis, xAxis);

		glm::mat4 mat;
		mat[0] = glm::vec4(xAxis, 0);
		mat[1] = glm::vec4(yAxis, 0);
		mat[2] = glm::vec4(zAxis, 0);
		mat[3] = glm::vec4(0,0,0, 1);

		mOrientation = glm::toQuat(mat);

		_invalidate();
	}

	glm::vec3 CameraOld::getPosition()
	{
		return mPosition;
	}

	void CameraOld::setOrientation(glm::quat orient)
	{
		mOrientation = orient;
	}

	glm::quat CameraOld::getOrientation()
	{
		return mOrientation;
	}

	glm::vec3 CameraOld::getForward()
	{
		_validate();
		return mCamMatrix[2].xyz;
	}

	glm::vec3 CameraOld::getRight()
	{
		_validate();
		return mCamMatrix[0].xyz;
	}

	glm::vec3 CameraOld::getUp()
	{
		_validate();
		return mCamMatrix[1].xyz;
	}

	void CameraOld::_validate()
	{
		if (mInvalid)
		{
			mCamMatrix = glm::toMat4(mOrientation);
			mCamMatrix[3] = glm::vec4(mPosition, 1);
			mViewMatrix = glm::inverse(mCamMatrix);
			mInvalid = false;
		}
	}

	void CameraOld::_invalidate()
	{
		mInvalid = true;
	}

	Scene* CameraOld::getScene() const
	{
		return mScene;
	}


}