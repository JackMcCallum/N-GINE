#ifndef __RENDERER_H_
#define __RENDERER_H_

#include <glm\glm.hpp>
#include <NGineCommon.h>
#include <vector>

namespace NGine
{
	class Camera;
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
	class Shader;
};

class Renderer
{
private:
	struct DirectionalLight
	{
		glm::vec3 colour;
		glm::vec3 direction;
	};

	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 colour;
		float distance;
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 colour;
		float distance;
		float angleinner;
		float angleouter;
	};

public:
	Renderer(uint32 width, uint32 height);
	~Renderer();

	// Prepare the frame buffers for rendering
	void prepare();

	// Finalise the render this includes rendering lights
	// and rendering post processing then rendering to back buffer
	void finalize(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam);

	void resize(uint32 width, uint32 height);

	// ================================================
	// Lighting
	
	// Constant light params
	// Set ambient color
	void setAmbientUpper(const glm::vec3& col);
	void setAmbientLower(const glm::vec3& col);

	// Clear all the lights from the scene
	void clearLights();

	// Add a point light to be rendered
	void addPointLight(const glm::vec3& pos, const glm::vec3& col, float distance);

	// Add a spot light to be rendered
	void addSpotLight(const glm::vec3& pos, const glm::vec3& dir, 
		const glm::vec3& col, float distance, float inner, float outer);

	// Add a directional light to be rendered
	void addDirectionalLight(const glm::vec3& col, const glm::vec3& dir);
	
private:
	void _createAllTextures();
	void _destroyAllTextures();
	
	void _createAllFrameBuffers();
	void _destroyAllFrameBuffers();

	void _createAllGeometry();
	void _destroyAllGeometry();

	void _createAllShaders();
	void _destroyAllShaders();

private:
	// Frame Buffers
	uint32 mGBuffer;
	uint32 mHDRBuffer;
	uint32 mTempBufferA_50P;
	uint32 mTempBufferB_50P;

	// Render Textures
	uint32 mNormalTexture; // RGB 8I >> NormX NormY NormZ
	uint32 mWorldTexture; // RGB 32F >> PosX PosY PosZ
	uint32 mDiffuseTexture; // RGBA 8I >> Red Green Blue Roughness
	uint32 mSpecularTexture; // RGBA 8I >> Red Green Blue Fresnel
	uint32 mDepthTexture; // DEPTH 32F

	uint32 mHDRTexture; // RGB 16F
	uint32 mTempTextureA_50P; // RGB 8I @ 50% Resolution
	uint32 mTempTextureB_50P; // RGB 8I @ 50% Resolution

	uint32 mNoiseTexture;

	// Geometric shapes
	NGine::IndexBuffer* mGeometryIBO;
	NGine::VertexBuffer* mGeometryVBO;
	NGine::VertexArray* mGeometryVAO;

	// Resolution
	uint32 mWidth;
	uint32 mHeight;

	// Lighting
	std::vector<DirectionalLight*> mDirectionalLightList;
	std::vector<PointLight*> mPointLightList;
	std::vector<SpotLight*> mSpotLightList;
	glm::vec3 mAmbientColourLower;
	glm::vec3 mAmbientColourUpper;

	// Shaders
	std::string mShaderDirectory;

	// >>> SSAO Shader
	NGine::Shader* mSSAOShader;
	int32 mSSAOShader_uCameraPosition;

	// >>> SSR Shader
	NGine::Shader* mSSRShader;
	int32 mSSRShader_uCameraPosition;
	int32 mSSRShader_uViewMatrix;

	// >>> Blur shader
	NGine::Shader* mVectorBlurSimpleShader;
	int32 mVectorBlurSimpleShader_uBlurParams;

	// >>> Blur shader
	NGine::Shader* mVectorBlurComplexShader;
	int32 mVectorBlurComplexShader_uBlurParams;

	// >>> Bloom shader
	NGine::Shader* mBloomFilterShader;
	int32 mBloomFilterShader_uBloomParams;

	// >>> Ambient Shader
	NGine::Shader* mAmbientShader;
	int32 mAmbientShader_uAmbientColourLower;
	int32 mAmbientShader_uAmbientColourUpper;

	// >>> Directional Light Shader
	NGine::Shader* mDirLightShader;
	int32 mDirLightShader_uCameraPosition;
	int32 mDirLightShader_uLightColour;
	int32 mDirLightShader_uLightDirection;

	// >>> Point Light Shader
	NGine::Shader* mPointLightShader;
	int32 mPointLightShader_uCameraPosition;
	int32 mPointLightShader_uLightColour;
	int32 mPointLightShader_uLightPosition;

	// >>> Simple Shader
	NGine::Shader* mSimplePassShader;

};

#endif