#version 330

in vec3 fPosition;
in vec3 fNormal;

layout(location = 0) out vec4 oColor;

uniform samplerCube uCubeMap;
uniform mat4 uCameraMatrix;

uniform float uReflectivity;
uniform float uMicrosurface;
uniform float uFresnel;
uniform vec3 uAlbedo = vec3(1);
uniform int uSampleCount;

float Lighting_Diffuse(in vec3 N, in vec3 L, in vec3 E, in float roughness)
{
	float r = min(1, max(0, (roughness - 0.2) / 0.8));
	
	// calculate intermediary values
    float NdotL = dot(N, L);
    float NdotV = dot(N, E); 

    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);
    
    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = dot(E - N * dot(E, N), L - N * dot(L, N));
    
    float roughnessSquared = r * r;
    
    // calculate A and B
    float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));

    float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));
 
    float C = sin(alpha) * tan(beta);
    
    // put it all together
    return max(0.0, NdotL) * (A + B * max(0.0, gamma) * C);
	
	
}


/**
 * Cook torrance shader copied directly from the AIE tutorial
 */
float Lighting_Specular(in vec3 N, in vec3 L, in vec3 H, in vec3 E, in float fresnel, in float roughness)
{
	float f = fresnel;//min(1, max(0, (fresnel - 0.2) / 0.8));
	float r = roughness;//min(1, max(0, (roughness - 0.2) / 0.8));
	
	float NdL = max(0.0f, dot(N, L));
	float NdE = max(0.0f, dot(N, E));
	float NdH = max(0.0f, dot(N, H));
	
	float m = r * r;
	float m2 = m * m;
	float NdH2 = NdH * NdH;
	
	// Beckman's Distribution Function D
	const float e = 2.71828182f;
	float exponent = -(1 - NdH2) / (NdH2 * m2);
	float Beckmann = max(0, pow(e, exponent) / (m2 * NdH2 * NdH2));
	
	// Schlick
	float k = m * 0.5;
	float G_SchlickV = NdE * (1 - k) + k;
	float G_SchlickL = NdL * (1 - k) + k;
	float Schlick = max(0, 0.25 / (G_SchlickV * G_SchlickL));
	
	// Schlick
	float Fresnel = max(0, f + (1 - f) * pow(1 - max(NdH, 0), 5));
	
	return Schlick * Fresnel * Beckmann;
}

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
 
vec3 domeRand(vec3 seed, float angle)
{
	const float pi = 3.14159265358979;
	const float hpi = 1.57079632679489;
	
	float y = (rand(seed.xy) * pi - hpi) / 180;
	float p = (rand(-seed.yz) * pi - hpi) / 180;
	
	vec3 ret;
	ret.x = sin(y * angle) * cos(p * angle);
	ret.y = sin(p * angle);
	ret.z = cos(y * angle) * cos(p * angle);
	return ret;
}

void main()
{
	vec3 E = normalize(uCameraMatrix[3].xyz - fPosition); // Eye
	vec3 L = normalize(vec3(1,1,1)); // Light
	vec3 N = normalize(fNormal); // Normal
	vec3 H = normalize(L + E); // Half
	
	const float e = 2.71828;
	
	float reflectivity = 1;//fPosition.y > 0 ? 0.5 : 0.9;
	float glossiness = 3;//fPosition.z > 0 ? 3 : 16;
	float metalness = 1;//fPosition.x > 0 ? 0.2 : 0.8;
	
	float fresnel = mix(pow(1-max(0, dot(-E, N)), 3 + uFresnel*6), 1, uFresnel);
	vec3 reflected = reflect(E, N);
	
	mat3 tbn;
	tbn[2] = reflected;
	tbn[0] = normalize(cross(tbn[2], vec3(0,1,0)));
	tbn[1] = normalize(cross(tbn[0], tbn[2]));
	
	vec3 col = vec3(0);
	for (int i=0; i<uSampleCount; i++)
	{
		vec3 randVec = domeRand(fPosition + i, uMicrosurface * 20);
		col += texture(uCubeMap, tbn * randVec).xyz / uSampleCount;
	}
	
	vec4 albedo = vec4(uAlbedo, 1);	
	
	float ambientShade = dot(N, normalize(vec3(0,1,0))) * 0.1 + 0.3;
	float diffShade = Lighting_Diffuse(N, L, E, uMicrosurface);
	
	
	float specShade = Lighting_Specular(N, L, H, E, fresnel, uMicrosurface);//pow(max(0, dot(E, reflect(L, N)) + 0.01), 1 + (1 / (uMicrosurface + 0.05))) * 0.5;
	
	vec4 specFinal = vec4(col, 1) + (albedo * specShade * diffShade);
	vec4 diffFinal = albedo * (diffShade);
	
	oColor = mix(diffFinal, specFinal, uReflectivity * fresnel);
}




