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

in vec2 vTexCoord;

uniform sampler2D uTexUnit0; // Albedo / AO
uniform sampler2D uTexUnit1; // Normals
uniform sampler2D uTexUnit2; // Emissive

out vec4 oColor0;

void main()
{
	vec4 AlbedoSamp = texture(uTexUnit0, vTexCoord);
	vec4 NormalSamp = texture(uTexUnit1, vTexCoord);
	vec4 EmissSamp = texture(uTexUnit2, vTexCoord);
	
	vec3 ambientColor = mix(vec3(0.1), vec3(0.3), NormalSamp.y * 0.5 + 0.5);
	
	oColor0 = vec4(ambientColor, 1) * AlbedoSamp + EmissSamp;
}










