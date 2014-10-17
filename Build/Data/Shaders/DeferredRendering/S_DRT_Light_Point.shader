#shader VERTEX
// ========================================
// Vertex Shader
#version 330
#include "../Headers/NGineUniform.sh"

in vec3 iPosition;

out vec4 vScreenPos;

void main()
{
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
	vScreenPos = gl_Position;
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330
#include "../Headers/NGineUniform.sh"
#include "../Headers/NGineLighting.sh"

in vec4 vScreenPos;

uniform sampler2D uTexUnit0; // Albedo / AO
uniform sampler2D uTexUnit1; // Normals
uniform sampler2D uTexUnit3; // Surface
uniform sampler2D uTexUnit5; // Depth

out vec4 oColor0;

void main()
{
	vec2 texCoord = (vScreenPos.xy / vScreenPos.w) * 0.5 + 0.5;
	
	vec4 NDC = vec4(texCoord*2-1, texture(uTexUnit5, texCoord).r*2-1, 1);
	vec4 projectedNDC = inverse(uProjViewMatrix) * NDC;
	vec3 worldPos = projectedNDC.xyz / projectedNDC.w;
	
	vec3 lightPos = uCustomParams0.xyz;
	vec4 lightColor = uCustomParams1;
	float lightAtten = uCustomParams0.w;
	float lightAtten2 = lightAtten * lightAtten;
	
	// Early discard pixels we _know_ are not going to affect outcome
	vec3 difference = lightPos - worldPos;
	float sqrLength = dot(difference, difference);
	if (sqrLength > lightAtten2) discard;
	
	float attenuation = max(0, 1 - (sqrt(sqrLength) / lightAtten));
	attenuation *= attenuation;
	
	vec4 AlbedoSamp = texture(uTexUnit0, texCoord);
	vec4 NormalSamp = texture(uTexUnit1, texCoord);
	vec4 SurfaceSamp = texture(uTexUnit3, texCoord);
	
	vec3 N = normalize(NormalSamp.xyz);
	vec3 L = normalize(difference);
	vec3 E = normalize(uCameraMatrix[3].xyz - worldPos);
	vec3 H = normalize(L + E);
	
	float roughness = SurfaceSamp.y;
	float metalness = SurfaceSamp.x;
	
	float shade = Lighting_Diffuse(N, L, E, roughness);
	shade += shade * Lighting_Specular(N, L, H, E, metalness, roughness);
	
	oColor0 = lightColor * AlbedoSamp * shade * attenuation;
}










