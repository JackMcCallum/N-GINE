// ========================================
#shader VERTEX
#version 330
#include "Headers/NGineUniform.sh"

in vec3 iPosition;

out vec3 vPosition;

void main()
{
	vPosition = iPosition;
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
}

// ========================================
#shader FRAGMENT
#version 330
#include "Headers/NGineUniform.sh"

in vec3 vPosition;

uniform sampler2D uTexUnit0;

out vec4 oColor0; // Albedo / AO
out vec4 oColor1; // Normal
out vec4 oColor2; // Emmisive
out vec4 oColor3; // Surface

void main()
{
	const float pi = 3.1415926535897932384626433832f;

	vec3 p = normalize(vPosition);
	float v = abs(dot(p, vec3(0,1,0)));
	float u = (atan(p.z, p.x) / (pi * 2)) + 0.5;
	
	vec4 skySamp = textureLod(uTexUnit0, vec2(u, v), 0);
	
	oColor0 = vec4(0);
	oColor1 = vec4(0.5,0.5,1,1);
	oColor2 = skySamp;
	oColor3 = vec4(0.5,0.5,1,1);
}










