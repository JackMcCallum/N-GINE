#shader VERTEX
// ========================================
// Vertex Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 iPosition;
in vec3 iNormal;

out vec3 vPosition;
out vec3 vNormal;

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 rand3(float seed)
{
	return vec3(
		rand(vec2(seed, 23423.12312-seed)),
		rand(vec2(12342.334+seed, 84623.235-seed)),
		rand(vec2(2132.43354+seed, 8651.856-seed)));
}

void main()
{
	
	vPosition = (uWorldMatrix * vec4(iPosition, 1)).xyz;
	vNormal = normalize(mat3(uWorldMatrix) * iNormal);
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 vPosition;
in vec3 vNormal;

out vec4 oColor0;

void main()
{
	float S = pow(1 - max(dot(vNormal, normalize(uCameraMatrix[3].xyz-vPosition)) - 0.3, 0), 8);
	
	oColor0 = mix(uMaterialDiffuse, vec4(0), 1-S);
}










