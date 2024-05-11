#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float4 highlightColor : packoffset(c0);
	float width : packoffset(c1.x);
	float height : packoffset(c1.y);
	uint maxProgress : packoffset(c1.z);
	uint progress : packoffset(c1.w);
};


D2D_PS_ENTRY(main) : SV_TARGET{
	float4 currentColor = D2DGetInput(0).rgba;
	float colorLength = length(currentColor.rgb);

	float progressInPercent = (progress % maxProgress) * 100.0f / (float)maxProgress;
	uint wantedX = width * progressInPercent / 100.0f;
	float2 currentPos = D2DGetScenePosition().xy;
	uint offsetDueToHeight = (uint)(width * 0.2f);
	float heightPercentage = (currentPos.y / height) + abs(sin(progress*0.01f)*0.05f);

	int maxDiff = (wantedX - (currentPos.x + (offsetDueToHeight * heightPercentage)));

	float4 resultColor = highlightColor;
	if (abs(maxDiff) > ((width/150)+1) || colorLength <= 0.06f) {
		resultColor = float4(0, 0, 0, 0);
	}
	return resultColor;
}