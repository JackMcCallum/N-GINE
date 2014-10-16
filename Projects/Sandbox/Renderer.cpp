#include "Precompiled.h"
#include "Renderer.h"

Renderer::Renderer(uint32 width, uint32 height)
{
	mShaderDirectory = "Data/Shaders/Renderer/";

	mWidth = width;
	mHeight = height;

	// Frame buffers
	mGBuffer = 0;
	mHDRBuffer = 0;
	mTempBufferA_50P = 0;
	mTempBufferB_50P = 0;

	// Textures
	mNormalTexture = 0;
	mWorldTexture = 0;
	mDiffuseTexture = 0;
	mSpecularTexture = 0;
	mDepthTexture = 0;
	mHDRTexture = 0;
	mTempTextureA_50P = 0;
	mTempTextureB_50P = 0;
	
	mAmbientColourUpper = glm::vec3(66, 115, 132) / 255.0f;
	mAmbientColourLower = glm::vec3(65, 48, 37) / 255.0f;

	// Geometry
	mGeometryIBO = nullptr;
	mGeometryVBO = nullptr;
	mGeometryVAO = nullptr;

	// Shaders
	mSSAOShader = nullptr;
	mSimplePassShader = nullptr;
	
	_createAllTextures();
	_createAllFrameBuffers();
	_createAllGeometry();
	_createAllShaders();
}

Renderer::~Renderer()
{
	_destroyAllFrameBuffers();
	_destroyAllShaders();
	_destroyAllTextures();
	_destroyAllGeometry();
	clearLights();
}

void Renderer::prepare()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);
	glViewport(0, 0, mWidth, mHeight);
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void Renderer::finalize(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& cam)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Put GBuffer into texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mNormalTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mWorldTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mDiffuseTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mSpecularTexture);
	
	mGeometryVAO->bind();
	// HDR Buffer 100% 32F
	// Buffer A 50%
	// Buffer B 50%

	// ==========================================
	// Render SSAO to Buffer A
	glActiveTexture(GL_TEXTURE4);
	glViewport(0, 0, mWidth / 2, mHeight / 2);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferA_50P);
	glBindTexture(GL_TEXTURE_2D, mNoiseTexture);
	mSSAOShader->bind();
	NGine::UniformUploader::upload<glm::vec3>(mSSAOShader_uCameraPosition, cam[3].xyz);
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Blur Buffer A vertically to B
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferB_50P);
	glBindTexture(GL_TEXTURE_2D, mTempTextureA_50P);
	mVectorBlurSimpleShader->bind();
	NGine::UniformUploader::upload<glm::vec4>(mVectorBlurSimpleShader_uBlurParams, glm::vec4(0, 1, 1 / (mWidth / 2.0f), 1 / (mHeight / 2.0f)));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Blur Buffer B horizontally to A
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferA_50P);
	glBindTexture(GL_TEXTURE_2D, mTempTextureB_50P);
	NGine::UniformUploader::upload<glm::vec4>(mVectorBlurSimpleShader_uBlurParams, glm::vec4(1, 0, 1 / (mWidth / 2.0f), 1 / (mHeight / 2.0f)));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Render AmbientPass & SSAO to HDR Buffer
	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mHDRBuffer);
	glBindTexture(GL_TEXTURE_2D, mTempTextureA_50P);
	mAmbientShader->bind();
	NGine::UniformUploader::upload<glm::vec3>(mAmbientShader_uAmbientColourLower, mAmbientColourLower);
	NGine::UniformUploader::upload<glm::vec3>(mAmbientShader_uAmbientColourUpper, mAmbientColourUpper);
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Render Lights to HDR Buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	mPointLightShader->bind();
	for (auto iter : mPointLightList)
	{
		NGine::UniformUploader::upload<glm::vec3>(mPointLightShader_uCameraPosition, cam[3].xyz);
		NGine::UniformUploader::upload<glm::vec3>(mPointLightShader_uLightColour, iter->colour);
		NGine::UniformUploader::upload<glm::vec4>(mPointLightShader_uLightPosition, glm::vec4(iter->position, iter->distance));
		mGeometryIBO->draw(NGine::TRIANGLE_LIST);
	}
	
	mDirLightShader->bind();
	for (auto iter : mDirectionalLightList)
	{
		NGine::UniformUploader::upload<glm::vec3>(mDirLightShader_uCameraPosition, cam[3].xyz);
		NGine::UniformUploader::upload<glm::vec3>(mDirLightShader_uLightColour, iter->colour);
		NGine::UniformUploader::upload<glm::vec3>(mDirLightShader_uLightDirection, iter->direction);
		mGeometryIBO->draw(NGine::TRIANGLE_LIST);
	}
	
	glDisable(GL_BLEND);

	// ==========================================
	// Filter HDR map into buffer A
	glViewport(0, 0, mWidth / 2, mHeight / 2);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferA_50P);
	glBindTexture(GL_TEXTURE_2D, mHDRTexture);
	mBloomFilterShader->bind();
	NGine::UniformUploader::upload<glm::vec4>(mBloomFilterShader_uBloomParams, glm::vec4(0));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Blur Buffer A vertically to B
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferB_50P);
	glBindTexture(GL_TEXTURE_2D, mTempTextureA_50P);
	mVectorBlurComplexShader->bind();
	NGine::UniformUploader::upload<glm::vec4>(mVectorBlurSimpleShader_uBlurParams, glm::vec4(0, 1.2, 1 / (mWidth / 2.0f), 1 / (mHeight / 2.0f)));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Blur Buffer B horizontally to A
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mTempBufferA_50P);
	glBindTexture(GL_TEXTURE_2D, mTempTextureB_50P);
	NGine::UniformUploader::upload<glm::vec4>(mVectorBlurSimpleShader_uBlurParams, glm::vec4(1.2, 0, 1 / (mWidth / 2.0f), 1 / (mHeight / 2.0f)));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);

	// ==========================================
	// Output
	
	// Combine to backbuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, mWidth, mHeight);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, mTempTextureA_50P);
	glBindTexture(GL_TEXTURE_2D, mHDRTexture);
	
	/*
	mSimplePassShader->bind();
	NGine::UniformUploader::upload<glm::vec4>(mBloomFilterShader_uBloomParams, glm::vec4(0));
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);
	*/

	mSSRShader->bind();
	NGine::UniformUploader::upload<glm::vec3>(mBloomFilterShader_uBloomParams, cam[3].xyz);
	NGine::UniformUploader::upload<glm::mat4>(mSSRShader_uViewMatrix, view);
	mGeometryIBO->draw(NGine::TRIANGLE_LIST);
	
}

void Renderer::resize(uint32 width, uint32 height)
{
	mWidth = width;
	mHeight = height;
	_destroyAllFrameBuffers();
	_destroyAllTextures();
	_createAllTextures();
	_createAllFrameBuffers();
}

// ==============================================================================
// Lighting methods

void Renderer::setAmbientUpper(const glm::vec3& col)
{
	mAmbientColourUpper = col;
}

void Renderer::setAmbientLower(const glm::vec3& col)
{
	mAmbientColourLower = col;
}

void Renderer::clearLights()
{
	for (auto iter : mDirectionalLightList)
		delete iter;

	for (auto iter : mPointLightList)
		delete iter;

	for (auto iter : mSpotLightList)
		delete iter;

	mDirectionalLightList.clear();
	mPointLightList.clear();
	mSpotLightList.clear();
}

void Renderer::addPointLight(const glm::vec3& pos, const glm::vec3& col, float distance)
{
	PointLight* light = new PointLight;
	light->position = pos;
	light->distance = distance;
	light->colour = col;
	mPointLightList.push_back(light);
}

void Renderer::addSpotLight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& col, float distance, float inner, float outer)
{
	SpotLight* light = new SpotLight;
	light->position = pos;
	light->direction = dir;
	light->colour = col;
	light->distance = distance;
	light->angleinner = inner;
	light->angleouter = outer;
	mSpotLightList.push_back(light);
}

void Renderer::addDirectionalLight(const glm::vec3& col, const glm::vec3& dir)
{
	DirectionalLight* light = new DirectionalLight;
	light->colour = col;
	light->direction = dir;
	mDirectionalLightList.push_back(light);
}

// ==============================================================================
// Internal methods

void Renderer::_createAllTextures()
{
	// Quick little macro for easy texture creation
#define __SHORTCUT__GL_INIT_TEXTURE(handle, width, height, filter, wrap, format, format2, type, data) \
	GL_CHECK(glGenTextures(1, &handle)); \
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle)); \
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format2, type, data)); \
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter)); \
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter)); \
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap)); \
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap)); \
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	// Create them textures
	__SHORTCUT__GL_INIT_TEXTURE(mNormalTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGB32F, GL_RGB, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mWorldTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA32F, GL_RGBA, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mDiffuseTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_RGB, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mSpecularTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_RGB, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mDepthTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	__SHORTCUT__GL_INIT_TEXTURE(mHDRTexture, mWidth, mHeight, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGB32F, GL_RGB, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mTempTextureA_50P, mWidth / 2, mHeight / 2, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGB, GL_RGB, GL_FLOAT, nullptr);
	__SHORTCUT__GL_INIT_TEXTURE(mTempTextureB_50P, mWidth / 2, mHeight / 2, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RGB, GL_RGB, GL_FLOAT, nullptr);

	uint8* data = new uint8[128 * 128 * 3];
	for (uint32 i = 0; i < 128 * 128 * 3; i++)
		data[i] = rand() % 256;
	__SHORTCUT__GL_INIT_TEXTURE(mNoiseTexture, 128, 128, GL_LINEAR, GL_REPEAT, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete[] data;
}

void Renderer::_destroyAllTextures()
{
	GL_CHECK(glDeleteTextures(1, &mNormalTexture));
	GL_CHECK(glDeleteTextures(1, &mWorldTexture));
	GL_CHECK(glDeleteTextures(1, &mDiffuseTexture));
	GL_CHECK(glDeleteTextures(1, &mSpecularTexture));
	GL_CHECK(glDeleteTextures(1, &mDepthTexture));
	GL_CHECK(glDeleteTextures(1, &mHDRTexture));
	GL_CHECK(glDeleteTextures(1, &mTempTextureA_50P));
	GL_CHECK(glDeleteTextures(1, &mTempTextureB_50P));
	GL_CHECK(glDeleteTextures(1, &mNoiseTexture));
}

void Renderer::_createAllFrameBuffers()
{
	uint32 drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	
	GL_CHECK(glGenFramebuffers(1, &mGBuffer));
	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer));
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mNormalTexture, 0));
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mWorldTexture, 0));
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, mDiffuseTexture, 0));
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, mSpecularTexture, 0));
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0));
	GL_CHECK(glDrawBuffers(4, drawBuffers));

#define __SHORTCUT__GL_INIT_FRAMEBUFFER(handle, texture) \
	GL_CHECK(glGenFramebuffers(1, &handle)); \
	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle)); \
	GL_CHECK(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0)); \
	GL_CHECK(glDrawBuffers(1, drawBuffers)); \

	__SHORTCUT__GL_INIT_FRAMEBUFFER(mHDRBuffer, mHDRTexture);
	__SHORTCUT__GL_INIT_FRAMEBUFFER(mTempBufferA_50P, mTempTextureA_50P);
	__SHORTCUT__GL_INIT_FRAMEBUFFER(mTempBufferB_50P, mTempTextureB_50P);

	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void Renderer::_destroyAllFrameBuffers()
{
	GL_CHECK(glDeleteFramebuffers(1, &mGBuffer));
	GL_CHECK(glDeleteFramebuffers(1, &mHDRBuffer));
	GL_CHECK(glDeleteFramebuffers(1, &mTempBufferA_50P));
	GL_CHECK(glDeleteFramebuffers(1, &mTempBufferB_50P));
}

void Renderer::_createAllGeometry()
{
	// Vertices
	glm::vec3 verts[] = { glm::vec3(-1,-1,0), glm::vec3(1,-1,0), glm::vec3(1,1,0), glm::vec3(-1,1,0) };

	// Indices
	uint32 inds[] = { 0, 1, 2, 0, 2, 3 };

	// Init geometry objects
	mGeometryVAO = new NGine::VertexArray(true);
	mGeometryVBO = new NGine::VertexBuffer(sizeof(verts), &verts);
	mGeometryIBO = new NGine::IndexBuffer(sizeof(inds), &inds);
	mGeometryVBO->setAttribute<float, 3>(0, sizeof(glm::vec3), 0);
	NGine::VertexArray::GLrevert();
}

void Renderer::_destroyAllGeometry()
{
	// Clean up all geometry objects
	delete mGeometryVAO;
	delete mGeometryVBO;
	delete mGeometryIBO;
}

void Renderer::_createAllShaders()
{
#define __SHORTCUT__QUAD_SHADER(handle, file) \
	handle = new NGine::Shader(); \
	handle->compileShaderFile(mShaderDirectory + "Quad.vert", NGine::VERTEX); \
	handle->compileShaderFile(mShaderDirectory + file, NGine::FRAGMENT); \
	handle->bindAttribute(0, "iPosition"); \
	handle->bindOutput(0, "oColor"); \
	handle->linkProgram();

	__SHORTCUT__QUAD_SHADER(mSimplePassShader, "SimplePass.frag");
	
	// SSAO
	__SHORTCUT__QUAD_SHADER(mSSAOShader, "ScreenSpaceAmbientOcclusion.frag");
	mSSAOShader_uCameraPosition = mSSAOShader->getUniformLocation("uCameraPosition");
	
	// SSR
	__SHORTCUT__QUAD_SHADER(mSSRShader, "ScreenSpaceReflections.frag");
	mSSRShader_uCameraPosition = mSSRShader->getUniformLocation("uCameraPosition");
	mSSRShader_uViewMatrix = mSSRShader->getUniformLocation("uViewMatrix");
	
	// Blur
	__SHORTCUT__QUAD_SHADER(mVectorBlurSimpleShader, "VectorBlurSimple.frag");
	mVectorBlurSimpleShader_uBlurParams = mVectorBlurSimpleShader->getUniformLocation("uBlurParams");

	__SHORTCUT__QUAD_SHADER(mVectorBlurComplexShader, "VectorBlurComplex.frag");
	mVectorBlurComplexShader_uBlurParams = mVectorBlurComplexShader->getUniformLocation("uBlurParams");

	// Bloom
	__SHORTCUT__QUAD_SHADER(mBloomFilterShader, "BloomFilter.frag");
	mBloomFilterShader_uBloomParams = mBloomFilterShader->getUniformLocation("uBloomParams");

	// Ambient 
	__SHORTCUT__QUAD_SHADER(mAmbientShader, "LightAmbient.frag");
	mAmbientShader_uAmbientColourLower = mAmbientShader->getUniformLocation("uAmbientColorLower");
	mAmbientShader_uAmbientColourUpper = mAmbientShader->getUniformLocation("uAmbientColorUpper");
	
	// Directional Light
	__SHORTCUT__QUAD_SHADER(mDirLightShader, "LightDirectional.frag");
	mDirLightShader_uCameraPosition = mDirLightShader->getUniformLocation("uCameraPosition");
	mDirLightShader_uLightColour = mDirLightShader->getUniformLocation("uLightColour");
	mDirLightShader_uLightDirection = mDirLightShader->getUniformLocation("uLightDirection");

	// Point Light
	__SHORTCUT__QUAD_SHADER(mPointLightShader, "LightPoint.frag");
	mPointLightShader_uCameraPosition = mPointLightShader->getUniformLocation("uCameraPosition");
	mPointLightShader_uLightColour = mPointLightShader->getUniformLocation("uLightColour");
	mPointLightShader_uLightPosition = mPointLightShader->getUniformLocation("uLightPosition");


	// Set texture uniform constants
	NGine::Shader* shaders[] = 
	{ 
		mSSAOShader, 
		mSSRShader,
		mSimplePassShader,
		mVectorBlurSimpleShader,
		mVectorBlurComplexShader,
		mBloomFilterShader,
		mAmbientShader,
		mDirLightShader,
		mPointLightShader
	};

	for (int i = 0; i < sizeof(shaders)/sizeof(void*); i++)
	{
		shaders[i]->bind();
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexNormals"), 0);
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexWorld"), 1);
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexDiffuse"), 2);
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexSpecular"), 3);

		// Noise texture
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexNoise"), 4);
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexSource"), 4);
		NGine::UniformUploader::upload<int>(shaders[i]->getUniformLocation("uTexSSAO"), 4);
	}

}

void Renderer::_destroyAllShaders()
{
	delete mSSAOShader;
	delete mSimplePassShader;
	delete mVectorBlurSimpleShader;
	delete mVectorBlurComplexShader;
	delete mAmbientShader;
	delete mDirLightShader;
	delete mPointLightShader;
	delete mBloomFilterShader;
}


