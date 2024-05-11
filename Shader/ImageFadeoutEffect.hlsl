#define D2D_INPUT_COUNT 2
#define D2D_INPUT0_COMPLEX
#define D2D_INPUT1_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width;
	float height;
	int direction;
	float cutoffPercentage;
};

D2D_PS_ENTRY(main) : SV_TARGET{
	float x = D2DGetScenePosition().x / width;
	float y = D2DGetScenePosition().y / height;
	float distance = 0.0f;
	switch (direction) {
		case 1: //TOP-BOTTOM
			distance = y;
		break;
		case 2://RIGHT->LEFT
			distance = 1.0f - x;
		break;
		case 3://BOTTOM-TOP
			distance = 1.0 - y;
		break;
		default: //LEFT->RIGHT
			distance = x;
		break;
	}
	float4 resultColor = D2DGetInput(0);
	float calcDistance = 0.8f;
	if (distance >= calcDistance) {
		resultColor = float4(resultColor.r, resultColor.g, resultColor.b, (1.0 - distance) / cutoffPercentage);
	}
	return resultColor;
}