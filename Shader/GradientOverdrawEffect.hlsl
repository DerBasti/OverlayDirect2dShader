#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float4 startColor : packoffset(c0);
	float4 endColor : packoffset(c1);
	float2 colorPositions : packoffset(c2);
	float2 dimensions : packoffset(c2.z);
};

D2D_PS_ENTRY(main) : SV_TARGET{
	float2 pixelPosition = D2DGetScenePosition().xy;
	float currentPercentage = pixelPosition.x / dimensions.x;
	float4 targetColor = D2DGetInput(0);
	if (currentPercentage <= colorPositions.x) {
		float alpha = targetColor.a - endColor.a;
		float3 diffColor = (targetColor.rgb - endColor.rgb) * alpha;
		targetColor.rgb = targetColor.rgb - diffColor;
	}
	if (currentPercentage >= colorPositions.x && currentPercentage <= colorPositions.y) {
		float gradientPercentage = (currentPercentage - colorPositions.x) / (colorPositions.y - colorPositions.x);
		//0,0,0,0 | 1,1,1,1

		//(1,1,1,1) - (0,0,0,1) * 0.5f + (0,0,0,1) = (1,1,1,0) * 0.5 + (0,0,0,1) = (0.5, 0.5, 0.5, 1.0f)
		float4 gradientDiff = ((endColor - startColor) * gradientPercentage) + startColor;
		targetColor.rgb = targetColor.rgb * (1.0f - gradientDiff.a) + gradientDiff.rgb * gradientDiff.a;
	}
	else if (currentPercentage > colorPositions.y) {
		targetColor.rgb = endColor.rgb;
	}
	targetColor.a = 1.0f;
	return targetColor;
}