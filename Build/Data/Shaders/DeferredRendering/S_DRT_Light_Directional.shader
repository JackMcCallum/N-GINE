#shader VERTEX
// ========================================
// Vertex Shader
#version 330

in vec3 iPosition;

out vec2 vTexCoord;

void main()
{
	vTexCoord = iPosition.xy * 0.5f + 0.5f;
	gl_Position = vec4(iPosition, 1);
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330
#include "../Headers/NGineUniform.sh"
#include "../Headers/NGineLighting.sh"

in vec2 vTexCoord;

uniform sampler2D uTexUnit0; // Albedo / AO
uniform sampler2D uTexUnit1; // Normals
uniform sampler2D uTexUnit3; // Surface
uniform sampler2D uTexUnit5; // Depth

out vec4 oColor0;

void main()
{
	vec4 AlbedoSamp = texture(uTexUnit0, vTexCoord);
	vec4 NormalSamp = texture(uTexUnit1, vTexCoord);
	vec4 SurfaceSamp = texture(uTexUnit3, vTexCoord);
	
	vec4 lightColor = uCustomParams1;
	
	vec4 NDC = vec4(vTexCoord*2-1, texture(uTexUnit5, vTexCoord).r*2-1, 1);
	vec4 projectedNDC = inverse(uProjViewMatrix) * NDC;
	vec3 worldPos = projectedNDC.xyz / projectedNDC.w;
	
	
	
	vec3 N = normalize(NormalSamp.xyz);
	vec3 L = normalize(uCustomParams0.xyz);
	vec3 E = normalize(uCameraMatrix[3].xyz - worldPos);
	vec3 H = normalize(L + E);
	
	
	//float shade = pow(max(dot(N, L) * 0.8 + 0.2, 0), 1.2);
	
	float roughness = SurfaceSamp.y;
	float metalness = SurfaceSamp.x;
	
	float shade = Lighting_Diffuse(N, L, E, roughness);
	//shade += pow(max(dot(N, H),0),32);
	shade += shade * Lighting_Specular(N, L, H, E, metalness, roughness);
	
	oColor0 = lightColor * AlbedoSamp * shade;
	//oColor0 = vec4(pos.xyz*0.1 + 0.5, 1);
}










