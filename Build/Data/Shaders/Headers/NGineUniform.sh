
// These uniforms need to match the layout of:
// NGineRenderSystem.h - struct NGine::GlobalUniforms

#define uWorldMatrix uWorldMatrixArray[gl_InstanceID]

layout(std140) uniform GlobalUniforms
{
	mat4 uProjMatrix;
	mat4 uViewMatrix;
	mat4 uCameraMatrix;
	mat4 uProjViewMatrix;
	mat4 uWorldMatrixArray[512];
	
	mat4 uTexProjMatrix;
	mat4 uTexViewMatrix;
	mat4 uTexProjViewMatrix;
	
	vec4 uMaterialDiffuse;
	vec4 uMaterialSpecular;
	vec4 uMaterialEmissive;
	vec4 uMaterialPBRParams;
	
	vec4 uCustomParams0;
	vec4 uCustomParams1;
	
	vec4 uLightPosition[8];
	vec4 uLightColor[8];
	vec4 uLightParameters[8];
};



