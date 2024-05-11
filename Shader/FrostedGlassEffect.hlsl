#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>
#include "random_texture.hlsli"

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float2 dimensions : packoffset(c0);
	float effectStrength : packoffset(c0.z);
    float effectFinesse : packoffset(c0.w);
};

float rand(float2 uv) {
    float a = dot(uv, float2(92., 10.));
    float b = dot(uv, float2(41., 62.));
    
    float x = sin(a) * effectFinesse / 2.0f + cos(b) * effectFinesse;
    return frac(x);
}

D2D_PS_ENTRY(main) : SV_TARGET{
	float2 pixelPosition = D2DGetScenePosition().xy;
	float2 uv = pixelPosition.xy / dimensions.xy;
	float2 rnd = float2(rand(uv), rand(uv));
    uv += rnd * effectStrength;
    return GetTextureValue(0, uv);
}