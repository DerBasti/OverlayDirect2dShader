#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float highlightColorR : packoffset(c0.x);
	float highlightColorG : packoffset(c0.y);
	float highlightColorB : packoffset(c0.z);
	float highlightColorA : packoffset(c0.w);
};

D2D_PS_ENTRY(main) : SV_TARGET{
	float2 position = D2DGetScenePosition().xy;
	float4 currentColor = D2DGetInput(0).rgba;
	if (currentColor.a > 0.0f) {
		float4 diffColors = 1.0 - float4(highlightColorR - currentColor.r, highlightColorG - currentColor.g, highlightColorB - currentColor.b, 0.0);
		float4 resultColor = float4(currentColor.r + (diffColors.r * highlightColorR), currentColor.g + (diffColors.g * highlightColorG), currentColor.b + (diffColors.b * highlightColorB), 1.0);
		currentColor = resultColor;
	}
	return currentColor;
}