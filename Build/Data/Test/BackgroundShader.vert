#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;

out vec3 fPosition;
out vec3 fNormal;


uniform mat4 uProjViewModel;

void main()
{
	fPosition = iPosition;
	fNormal = iNormal;
	
	gl_Position = uProjViewModel * vec4(iPosition, 1);
}