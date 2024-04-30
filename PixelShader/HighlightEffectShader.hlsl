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
	float modifier = 0.0f;
	float yPosition = (height * (progress*3.25f) - (height*0.75f));
	if (progress <= 0.5f) {
		modifier = pow(progress / 0.5f, 6.0f);
	}
	else if (progress >= 0.5f) {
		modifier = pow((1.0f - progress) / 0.5f, 6.0f);
	}
	currentColor.r += (1.0 - currentColor.r) * modifier; //0.3 + (0.7 * ) = 0.5
	currentColor.g += (1.0 - currentColor.g) * modifier; //
	currentColor.b += (1.0 - currentColor.b) * modifier;
	int pixelDifference = (int)abs(pixelPosition.y - yPosition);
	int maxAllowedDifference = (int)(height * 0.1f);
	if (pixelDifference <= maxAllowedDifference) {
		float pixelDifferenceRatio = pixelDifference / (float)maxAllowedDifference;
		if (pixelDifferenceRatio >= 0.75f) {
			pixelDifferenceRatio = 1.0f;
		}
		currentColor.r += (1.0f - currentColor.r) * (1.0f - pixelDifferenceRatio);
		currentColor.g += (1.0f - currentColor.g) * (1.0f - pixelDifferenceRatio);
		currentColor.b += (1.0f - currentColor.b) * (1.0f - pixelDifferenceRatio);
	}
	return currentColor;
}

D2D_PS_ENTRY(main) : SV_TARGET{
	float4 pixelPosition = D2DGetScenePosition();
	float4 color = D2DGetInput(0);
	if (progress >= 0.0f && progress <= 1.0f) {
		color = getHighlightColor(pixelPosition.xy, color);
	}
	return color;
}