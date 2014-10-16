#shader VERTEX
// ========================================
// Vertex Shader
#version 330
#include "Headers/NGineUniform.sh"

in vec3 iPosition;

out vec3 vPosition;

void main()
{
	vPosition = (uWorldMatrix * vec4(iPosition, 1)).xyz;
	gl_Position = uProjViewMatrix * uWorldMatrix * vec4(iPosition, 1);
}

#shader FRAGMENT
// ========================================
// Fragment Shader
#version 330

in vec3 vPosition;

out vec4 oColor0;

void main()
{
	oColor0 = vec4(vPosition * 0.5 + 0.5, 1);
}










