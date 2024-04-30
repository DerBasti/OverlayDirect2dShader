#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width : packoffset(c0.x);
	float height : packoffset(c0.y);
	float borderStrengthPercentage : packoffset(c0.z);
	float highlightColorR : packoffset(c0.w);
	float highlightColorG : packoffset(c1.x);
	float highlightColorB : packoffset(c1.y);
	float highlightColorA : packoffset(c1.z);
};

D2D_PS_ENTRY(main) : SV_TARGET{
	float x = D2DGetScenePosition().x / width; 
	float y = D2DGetScenePosition().y / height;
	float4 highlightColor = float4(highlightColorR, highlightColorG, highlightColorB, highlightColorA);
	float4 currentColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 resultColor = currentColor;
	float border = borderStrengthPercentage;
	float reverseBorder = 1.0f - border;
	float4 colorDiff = highlightColor - resultColor;
	if (x <= border && y >= reverseBorder) {
		float maxResult = max(1.0f - x, y);
		float ratio = (1.0f - maxResult) / border;
		resultColor = colorDiff * (1.0f - ratio);
	}
	else if (x >= reverseBorder && y <= border) {
		float maxResult = max(x, 1.0 - y);
		float ratio = (1.0f - maxResult) / border;
		resultColor = colorDiff * (1.0f - ratio);
	}
	else if (x <= border || y <= border) {
		float minResult = min(x, y);
		float ratio = minResult / border;
		resultColor = colorDiff * (1.0f - ratio);
	}
	else if (x >= reverseBorder || y >= reverseBorder) {
		float maxResult = max(x, y);
		float ratio = (1.0f - maxResult) / border;
		resultColor = colorDiff * (1.0f - ratio);
	}
	return resultColor;
}