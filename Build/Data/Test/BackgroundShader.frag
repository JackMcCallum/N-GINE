#version 330

in vec3 fPosition;
in vec3 fNormal;

layout(location = 0) out vec4 oColor;

uniform samplerCube uCubeMap;
uniform mat4 uCameraMatrix;

void main()
{
	vec3 N = normalize(fNormal);
	
	oColor = texture(uCubeMap, N);
}




