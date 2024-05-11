#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width : packoffset(c0.x);
	float height : packoffset(c0.y);
	float angle : packoffset(c0.z);
	float progress : packoffset(c0.w);
};
 

float4 getHighlightColor(float2 pixelPosition, float4 color) {
	float4 currentColor = color;
	if (progress >= 0.5f) {
		currentColor.r = (1.0 - currentColor.r) * (progress / 0.5f); //0.3 + (0.7 * ) = 0.5
		currentColor.g = (1.0 - currentColor.g) * (progress / 0.5f); //
		currentColor.b = (1.0 - currentColor.b) * (progress / 0.5f);
		currentColor.a = 1.0f;
	}
	currentColor = saturate(currentColor);
	return currentColor;
}

D2D_PS_ENTRY(main) : SV_TARGET {
	float2 pixelPosition = D2DGetScenePosition().xy;
	float4 currentColor = D2DGetInput(0);
	float2 pixelPositionAsPercentage = pixelPosition / float2(width, height);
	return float4(0, 0, 0, 1.0);
}