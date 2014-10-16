#shader VERTEX
// ========================================
// Vertex Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 iPosition;
in vec3 iNormal;
in vec4 iTangent;
in vec2 iTexCoord0;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord0;
out vec4 vTangent;

void main()
{

	vTexCoord0 = iTexCoord0;
	vPosition = (uWorldMatrix * vec4(iPosition, 1)).xyz;
	vNormal = normalize(mat3(uWorldMatrix) * iNormal);
	vTangent = iTangent;
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;
in vec4 vTangent;

uniform sampler2D uTexUnit0;
uniform sampler2D uTexUnit1;

out vec4 oColor0; // Albedo / AO
out vec4 oColor1; // Normal
out vec4 oColor2; // Emmisive
out vec4 oColor3; // Surface

void main()
{
	float p = 0.8 + ((int(gl_FragCoord.x) / 2 + int(gl_FragCoord.y) / 2) % 2) * 0.2;
	//float S = abs(dot(vNormal, normalize(uCameraMatrix[3].xyz-vPosition))) * 0.4 + 0.6;
	//float S = max(dot(vNormal, normalize(vec3(1,1,1))) * 0.8 + 0.2, 0) + 0.5;
	
	
	vec4 samp = texture(uTexUnit0, vTexCoord0 * 20);
	vec4 samp2 = texture(uTexUnit1, vTexCoord0) * 0.7;
	
	oColor0 = vec4(samp2.xyz, 1) * uMaterialDiffuse;
	oColor1 = vec4(vNormal, 1);
	oColor2 = vec4(uMaterialEmissive);
	oColor3 = vec4(samp2.a * samp.x * 0.8 + 0.4, samp2.a * samp.x * 0.8 + 0.4, 1, 1);
	
}










