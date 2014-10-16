#version 330

#define SAMPLE_COUNT 32
#define VERT_COUNT 128


layout (points) in;
layout (triangle_strip, max_vertices = VERT_COUNT) out;

uniform mat4 projViewModel;
uniform mat4 camMatrix;

out vec3 fPosition;

void main()
{
	float scale = 1.415; // sqrt(2);
	mat3 camAngle = mat3(camMatrix);
	camAngle[2] = normalize(camMatrix[3].xyz);
	camAngle[0] = -cross(camAngle[1], camAngle[2]);
	camAngle[1] = -cross(camAngle[0], camAngle[2]);
	
	
    for(int i = 0; i < SAMPLE_COUNT; i++)
    {
		float offset = (float(i) / SAMPLE_COUNT) * 2 - 1;
		
		fPosition = camAngle * (vec3(-1,-1,offset) * scale);
        gl_Position = projViewModel * (vec4(fPosition, 1) + gl_in[0].gl_Position);
        EmitVertex();
		
		fPosition = camAngle * (vec3(1,-1,offset) * scale);
        gl_Position = projViewModel * (vec4(fPosition, 1) + gl_in[0].gl_Position);
        EmitVertex();
		
		fPosition = camAngle * (vec3(-1,1,offset) * scale);
        gl_Position = projViewModel * (vec4(fPosition, 1) + gl_in[0].gl_Position);
        EmitVertex();
		
		fPosition = camAngle * (vec3(1,1,offset) * scale);
        gl_Position = projViewModel * (vec4(fPosition, 1) + gl_in[0].gl_Position);
        EmitVertex();
		
		EndPrimitive();
    }
}