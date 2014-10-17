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
#include "Headers/NGineUniform.sh"

in vec2 vTexCoord;

uniform sampler2D uTexUnit0;

out vec4 oColor0;

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	oColor0 = (texture(uTexUnit0, vTexCoord) + vec4(rand(vTexCoord) / 255.0f)) ;
}










