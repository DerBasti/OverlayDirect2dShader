#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>
#include "random_texture.hlsli"

cbuffer Constants : register(b0) {
	float2 dimensions : packoffset(c0);
	uint timePassed : packoffset(c0.z);
	float distortionStrength : packoffset(c0.w);
	float particleSizeVariation : packoffset(c1.x);
	float particleBrightness : packoffset(c1.y);
}

#define FREQ_STEP (0.001953125 * 3.0)

#define PI 3.1415927
#define TWO_PI 6.283185

float pow3(in float x)
{
 	return x*x*x;   
}

//Simple interpolated noise
float2 noise2_3(float3 coord)
{
    //float3 f = frac(coord);
    float3 f = smoothstep(0.0, 1.0, frac(coord));
 	
    float3 uv000 = floor(coord);
    float3 uv001 = uv000 + float3(0,0,1);
    float3 uv010 = uv000 + float3(0,1,0);
    float3 uv011 = uv000 + float3(0,1,1);
    float3 uv100 = uv000 + float3(1,0,0);
    float3 uv101 = uv000 + float3(1,0,1);
    float3 uv110 = uv000 + float3(1,1,0);
    float3 uv111 = uv000 + float3(1,1,1);
    
    float2 v000 = hash23(uv000);
    float2 v001 = hash23(uv001);
    float2 v010 = hash23(uv010);
    float2 v011 = hash23(uv011);
    float2 v100 = hash23(uv100);
    float2 v101 = hash23(uv101);
    float2 v110 = hash23(uv110);
    float2 v111 = hash23(uv111);
    
    float2 v00 = lerp(v000, v001, f.z);
    float2 v01 = lerp(v010, v011, f.z);
    float2 v10 = lerp(v100, v101, f.z);
    float2 v11 = lerp(v110, v111, f.z);
    
    float2 v0 = lerp(v00, v01, f.y);
    float2 v1 = lerp(v10, v11, f.y);
    float2 v = lerp(v0, v1, f.x);
    
    return v;
}

//Simple interpolated noise
float noise1_2(in float2 uv)
{
    float2 f = frac(uv);
    //float2 f = smoothstep(0.0, 1.0, frac(uv));
    
 	float2 uv00 = floor(uv);
    float2 uv01 = uv00 + float2(0,1);
    float2 uv10 = uv00 + float2(1,0);
    float2 uv11 = uv00 + 1.0;
    
    float v00 = hash12(uv00);
    float v01 = hash12(uv01);
    float v10 = hash12(uv10);
    float v11 = hash12(uv11);
    
    float v0 = lerp(v00, v01, f.y);
    float v1 = lerp(v10, v11, f.y);
    float v = lerp(v0, v1, f.x);
    
    return v;
}


//Blur function
float4 blur(in float2 fragCoord, in float2 resolution)
{
    const float BLUR_STRENGTH = 2.0f;
    const float BLUR_RANGE = 2.5f;
    float2 uv = fragCoord / resolution;
    float blurStrength = distance(uv, float2(0.5, 0.0f));
    blurStrength = pow(blurStrength, BLUR_RANGE) * (resolution.x / 100.0) * BLUR_STRENGTH;
    float4 sum = float4(0.0, 0.0, 0.0, 0.0);
    float2 pixelSize = float2(1.0, 0.0) / resolution;
	for (float x = -1.0; x <= 1.0; x += 1.0)
    {
     	for (float y = -1.0; y <= 1.0; y += 1.0)
        {
            sum += GetTextureValue(0, uv + float2(x, y) * pixelSize * blurStrength);
        }
    }

    return sum / 9.0;
}

//Calculates particle movement
float2 cellPointFromRootUV(float2 rootUV, float2 originalUV, out float len, in float PARTICLE_ITERATIONS)
{
    float timeInSeconds = timePassed / 1000.0f;
    float2 displacement = (noise2_3(float3(rootUV * 0.07 + timeInSeconds * 0.3, 0.5 * (timeInSeconds + 0.1) * 1.0 + noise1_2(originalUV * 0.04))) - 0.5);
 	len = dot(displacement, displacement);
    return displacement * 3.0 * (PARTICLE_ITERATIONS) + 0.5 + rootUV;   
}

//Calculates particle size
float particleFromUVAndPoint(in float2 uv, in float2 pt, in float2 rootUV, in float pixelSize)
{
    const float PARTICLE_RADIUS = 0.35f;
    const float PARTICLE_RADIUS2 = 0.75f;
	float dist = distance(uv, pt);
    dist += (hash12(rootUV * 10.0) - 0.5) * particleSizeVariation;
    float particle = 1.0 - smoothstep(PARTICLE_RADIUS - dist * 0.05, PARTICLE_RADIUS2 - dist * 0.05 + pixelSize, dist);
    return particle * particle;
}

//Particle system
float3 surfaceParticles(in float2 uv, in float pixelSize)
{
    float3 particles = float3(0.0, 0.0, 0.0);
 	float2 rootUV = floor(uv);
    
   	float2 tempRootUV;
    float2 pointUV;
    float dist;
    float3 color;
    const float PARTICLE_ITERATIONS = 2.0f;
    float xRatio = D2DGetScenePosition().x / dimensions.x;
    float offset = 0.35f;
    float3 redColor = float3(0.9f, 0.1f, 0.1f);
    float3 centerColor = float3(0.05f, 0.05f, 0.05f);
    float3 blueColor = float3(0.1, 0.1, 0.9f);
    const float3 PARTICLE_COLOR = lerp(redColor, lerp(centerColor, blueColor, smoothstep(0.00, 0.75f, (xRatio - offset) / (1.0f - offset))), xRatio);

    for (float x = -PARTICLE_ITERATIONS; x <= PARTICLE_ITERATIONS; x += 1.0)
    {
        for (float y = -PARTICLE_ITERATIONS; y <= PARTICLE_ITERATIONS; y += 1.0)
        {
            tempRootUV = rootUV + float2(x, y);
            pointUV = cellPointFromRootUV(tempRootUV, uv, dist, PARTICLE_ITERATIONS);
          	color = lerp(float3(0.0f, 0.0f, 0.0f), PARTICLE_COLOR, pow(smoothstep(0.3, 0.0, dist), 4.0));
            particles += particleFromUVAndPoint(uv, pointUV, tempRootUV, pixelSize) * color;
        }
    }
    
    return particles;
}

float4 calculateParticleColor() {
    float2 fragCoord = D2DGetScenePosition().xy;
    float2 uv = (2.0 * fragCoord - dimensions.xy) / dimensions.x;
    
    float vignette = 1.0 - smoothstep(0.5, 1.3, length(uv* float2(1.0, dimensions.x / dimensions.y)));
    
    //for antialiasing
    float pixelSize = 2.5 / dimensions.x;	
    
    uv *= 60.0;
    pixelSize *= 30.0;
    
    float3 particles = surfaceParticles(uv, pixelSize) * particleBrightness;
    
    //postprocess
    particles = smoothstep(-0.1, 0.8, particles * vignette);
    
    return float4(particles, 1.0);
}

D2D_PS_ENTRY(main) : SV_TARGET {
    float4 particleColor = calculateParticleColor();
    //float4 blurColor = blur(D2DGetScenePosition().xy, dimensions);
	return particleColor;
}