
/**
 * Oren-Nayar shader copied directly from the AIE tutorial
 */
float Lighting_Diffuse(in vec3 N, in vec3 L, in vec3 E, in float roughness)
{
	/*float NdL = max(0.0f, dot(N, L));
	float NdE = max(0.0f, dot(N, E));
	float R2 = roughness * roughness;
	
	// Oren-Nayar Diffuse Term
	float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
	float B = 0.45f * R2 / (R2 + 0.09f);
	
	// CX = Max(0, cos(l,e))
	vec3 lightProjected = normalize(L - N * NdL);
	vec3 viewProjected = normalize(E - N * NdE);
	float CX = max(0.0f, dot(lightProjected, viewProjected ));
	
	// DX = sin(alpha) * tan(beta)
	float alpha = sin(max(acos(NdE), acos(NdL)));
	float beta = tan(min(acos(NdE), acos(NdL)));
	float DX = alpha * beta;
	
	// Calculate Oren-Nayar, replaces the Phong Lambertian Term
	return NdL * (A + B * CX * DX);*/
	
	
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
	
/*
	float NdL = max(0.0f, dot(N, L));
	float NdE = max(0.0f, dot(N, E));
	float R2 = roughness * roughness;
	
	const float e = 2.71828182845904523536028747135f;
	const float pi = 3.1415926535897932384626433832f;
	
	float NdH = max(0.0f, dot(N, H));
	float NdH2 = NdH * NdH;
	
	// Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow(e, exponent) / (R2 * NdH2 * NdH2);
	
	// Fresnel Term F
	float F = mix(pow(1 - max(dot(H, E), 0), 5), 1, fresnel);
	
	// Geometric Attenuation Factor G
	float X = 2.0f * NdH / dot(E, H);
	float G = min(1, min(X * NdL, X * NdE));
	
	// Calculate Cook-Torrance
	return max((D*G*F) / (NdE * pi), 0.0f);
*/
}



