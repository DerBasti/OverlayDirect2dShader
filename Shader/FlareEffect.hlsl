#define D2D_INPUT_COUNT 2
#define D2D_INPUT0_SIMPLE
#define D2D_INPUT1_SIMPLE
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D<float4> ThisCanvas : register(t0);
Texture2D<float4> NoiseTexture : register(t1);

SamplerState ThisSampler : register(s0);
SamplerState NoiseSampler : register(s1);

cbuffer Constants : register(b0) {
	float4 flareOuterColor : packoffset(c0);
	float4 flareInnerColor : packoffset(c1);
	float2 dimensions : packoffset(c2);
	float2 noiseTextureDimensions : packoffset(c2.z);
	uint timePassed : packoffset(c3);
	float3 flareHeight : packoffset(c3.y);
	uint direction : packoffset(c4);
};

float2 getTextureCoordinate(float absX, float absY) {
	//float2 modifier = float2(absX / resolutionX, absY / resolutionY);
	//float2 value = float2(absX - round((uint)modifier.x * resolutionX), absY - round((uint)modifier.y * resolutionY));
	float2 modifier = float2(frac(absX / noiseTextureDimensions.x), frac(absY / noiseTextureDimensions.y));
	float2 wrappedValue = float2(noiseTextureDimensions.x * modifier.x, noiseTextureDimensions.y * modifier.y);

	wrappedValue.x = clamp(wrappedValue.x, 0, noiseTextureDimensions.x - 1);
	wrappedValue.y = clamp(wrappedValue.y, 0, noiseTextureDimensions.y - 1);
	return wrappedValue;
}

D2D_PS_ENTRY(main) : SV_TARGET{

	float2 absPos = D2DGetScenePosition().xy;
	float timeInSeconds = timePassed / 20.0f;

	float xTime = 0.0f;
	float yTime = 0.0f;
	float gradientValue = absPos.y / dimensions.y;
	switch (direction) {
		case 0: //TOP_TO_BOTTOM
			gradientValue = 1.0f - gradientValue;
			yTime = -timeInSeconds;
		break;
		case 1: //
			gradientValue = (absPos.x / dimensions.x);
			xTime = timeInSeconds;
		break;
		case 2:
			yTime = timeInSeconds;
		break;
		case 3:
			gradientValue = 1.0f - (absPos.x / dimensions.x);
			xTime = -timeInSeconds;
		break;
	}


	float2 modifiedAbsValues = getTextureCoordinate(absPos.x + xTime, absPos.y + yTime);
	float4 color = D2DSampleInputAtPosition(1, modifiedAbsValues);

	float noiseValue = color.x;

	float step1 = step(noiseValue, gradientValue - flareHeight.x);
	float step2 = step(noiseValue, gradientValue - flareHeight.y);
	float step3 = step(noiseValue, gradientValue - flareHeight.z);
	float step4 = step(noiseValue, gradientValue - 0.9f);

	float4 threeFourthInner = (flareInnerColor * 3 + flareOuterColor) / 4;
	float4 centerColor = (flareInnerColor + flareOuterColor) / 2;
	float4 almostOuterColor = (flareInnerColor + flareOuterColor * 3) / 4;

	float4 resultColor = float4(lerp(flareInnerColor.rgb, flareOuterColor.rgb, step1 - step2), step1); //Darkest
	resultColor.rgb = lerp(resultColor.rgb, almostOuterColor.rgb, step2 - step3); //Darker
	resultColor.rgb = lerp(resultColor.rgb, centerColor.rgb, step3 - step4); //Darky
	if (step1 <= 0.05f) {
		resultColor = float4(0, 0, 0, 0);
	}
	return resultColor;
}