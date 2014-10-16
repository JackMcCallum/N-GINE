#shader VERTEX
// ========================================
// Vertex Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 iPosition;
in vec4 iColor;

out vec4 vColor;

void main()
{
	vColor = iColor;
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec4 vColor;

out vec4 oColor0; // Albedo / AO
out vec4 oColor1; // Normal
out vec4 oColor2; // Emmisive
out vec4 oColor3; // Surface

void main()
{
	oColor0 = vec4(vColor.xyz, 1);
	oColor1 = vec4(0.5,1,0.5,1);
	oColor2 = vColor;
	oColor3 = vec4(0.5);
}










