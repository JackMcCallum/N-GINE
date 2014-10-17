#ifndef __NGINE_COMMON_H_
#define __NGINE_COMMON_H_

// N-Gine Mesh is a binary mesh format
#define NGINE_MESH_EXT ".ngm"
#define NGINE_SHADER_EXT ".ngs"
#define NGINE_TEXTURE_EXT ".ngt"

// Disable annoying warnings
#pragma warning(disable: 4099)
#pragma warning(disable: 4127)
#pragma warning(disable: 4049)

// SWIZZLING!!
#define GLM_SWIZZLE

// Delete if the data is not nullptr and sets the pointer back to nullptr
#define _DELETE_IF(data) if (data != nullptr) { delete data; data = nullptr; }
#define _DELETE_IF_CODE(data, code) if (data != nullptr) { code; delete data; data = nullptr; }
#define _DELETE_ARRAY_IF(data) if (data != nullptr) { delete[] data; data = nullptr; }

// Fixed integer formats
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>

typedef std::pair<std::string, int32> StringIntPair;
typedef std::pair<int32, int32> IntIntPair;

typedef std::vector<std::string> StringList;
typedef std::vector<StringIntPair> StringIntList;
typedef std::vector<IntIntPair> IntIntList;

#include <sdl/SDL_keycode.h>
#include <sdl/SDL_mouse.h>

// Forward declarations
namespace NGine
{
	class Main;
	class RenderThread;
	class RenderWindow;
	class RenderSystem;
	class SDLRenderWindow;
	class ResourceManager;
	class Texture;
	class Mesh;
	class Shader;
	class DeferredRenderTarget;
	class RenderTarget;
	class MeshRenderer;
	class Camera;
	class Gizmos;
	class Light;
	class DirectionalLight;
	class PointLight;

	class GLBuffer;
	class GLMesh;
	class GLShader;
	class GLTexture2D;
	class GLTextureCube;
	class GLTexture;
	class GLFrameBuffer;
}

// Enum declarations
namespace NGine
{

	enum BufferBit : uint8
	{
		COLOR_BUFFER = (1 << 0),
		DEPTH_BUFFER = (1 << 1),
		STENCIL_BUFFER = (1 << 2),
	};
	inline int operator | (BufferBit a, BufferBit f)
	{
		return (int)a | (int)f;
	}


	enum TextureType : uint8
	{
		TEXTURE_2D,
		TEXTURE_CUBE,
	};

	enum BlendMode
	{
		BM_DISABLED,
		BM_ALPHA_BLEND,
		BM_ADDITIVE,
		BM_MODULATIVE,
	};

	enum CullMode : uint8
	{
		CM_DISABLED,
		CM_FRONT,
		CM_BACK,
		CM_FRONT_AND_BACK,
	};

	enum DepthFunc : uint8
	{
		DF_NEVER,
		DF_LESS,
		DF_EQUAL,
		DF_LEQUAL,
		DF_GREATER,
		DF_NOTEQUAL,
		DF_GEQUAL,
		DF_ALWAYS,
	};

	enum BufferUsage : uint8
	{
		BU_STATIC,
		BU_DYNAMIC,
		BU_STREAM,
	};

	enum ShaderType : uint8
	{
		VERTEX,
		CONTROL,
		EVALUATION,
		GEOMETRY,
		FRAGMENT,
		_COUNT
	};

	enum Semantic : uint8
	{
		SM_POSITION,
		SM_NORMAL,
		SM_TANGENT,
		SM_COLOR,
		SM_BONEWEIGHTS,
		SM_BONEINDICES,
		SM_TEXCOORD0,
		SM_TEXCOORD1,
		SM_TEXCOORD2,
		SM_TEXCOORD3,
		SM_TEXCOORD4,
		SM_TEXCOORD5,
		SM_TEXCOORD6,
		SM_TEXCOORD7,
		__SEMANTIC_COUNT,
	};

	enum RenderLayer
	{
		RL_OPAQUE,
		RL_BACKGROUND,
		RL_TRANSPARENT,
		RL_GUI,

		__RL_RESERVED1,
		__RL_RESERVED2,
		__RL_RESERVED3,
		__RL_RESERVED4,
	};

	enum PixelFormat : uint8
	{
		PF_UBYTE_R,
		PF_UBYTE_RG,
		PF_UBYTE_RGB,
		PF_UBYTE_RGBA,
		PF_FLOAT_R,
		PF_FLOAT_RG,
		PF_FLOAT_RGB,
		PF_FLOAT_RGBA,
		PF_DEPTH_16,
		PF_DEPTH_24,
		PF_DEPTH_32,

		//PF_DTX1,
		//PF_DTX3,
		//PF_DTX5,
	};
}


#endif // !__NGINE_COMMON_H_
