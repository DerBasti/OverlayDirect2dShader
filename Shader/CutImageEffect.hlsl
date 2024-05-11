#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width;
	float height;
	float angle;
	float cutWidth;
	float progress;
	float maxProgress;
	int cutMode;
};

float random(float p) {
	return frac(sin(p)*10000.);
}

float calcDistanceToRay(float2 absPos) {
	float allowedY = (1.0f - (absPos.x/width)) * height;
	float dist = (allowedY - absPos.y) / cutWidth * (cutMode == 0 ? 2 : 1);
	return dist;
}

float getDistanceBasedOnCutmode(float dist) {
	switch (cutMode) {
		case 0: //Both
			dist = abs(dist);
		break;
		case 1: //TOP
			dist += 0.33f;
		break;
		case 2: //BOTTOM
			dist = -dist + 0.33f;
		break;
	}
	return dist;
}

float4 getColorAtPosition(float2 absPos) {
	float4 resultColor = D2DGetInput(0);
	if (absPos.x >= 0 && absPos.y >= 0 && absPos.x < width && absPos.y < height) {
		float dist = getDistanceBasedOnCutmode(calcDistanceToRay(absPos));
		resultColor = D2DSampleInputAtPosition(0, absPos);
		if (dist >= 0.7f && dist <= 1.0f) {
			float4 diffColor = float4(dist, (dist - 0.7f), (dist - 0.7f), (dist - 0.7f));
			resultColor = lerp(resultColor, diffColor, (1.0f - dist) / 0.3f);
		}
	}
	return resultColor;
}

D2D_PS_ENTRY(main) : SV_TARGET {
	float2 absPos = D2DGetScenePosition().xy;
	float2 relativePos = float2(absPos.x / width, absPos.y / height);

	const float SPEED_UP_CUT_MODIFIER = 1.0f;
	float progressPercentage = (progress / (maxProgress/SPEED_UP_CUT_MODIFIER));
	float cutHighlightRadius = cutWidth * 1.5f;

	float distX = (relativePos.x - progressPercentage) * width; //1.0 - 0.0
	float distY = ((1.0f - relativePos.y) - progressPercentage) * height;
	float actualDist = length(float2(distX, distY));

	float4 resultColor = D2DGetInput(0);
	float dist = getDistanceBasedOnCutmode(calcDistanceToRay(absPos));
	float progressModifierAftercut = ((SPEED_UP_CUT_MODIFIER-1) / SPEED_UP_CUT_MODIFIER);
	bool isDistReached = false;

	if (dist >= 0.0 && dist <= 1.0f && relativePos.x <= ((progress-progressModifierAftercut)*SPEED_UP_CUT_MODIFIER)) {
		float len = length(relativePos);
		float4 lineColor = float4(len * 0.16f, len * 0.158f, len * 0.162f, 1);
		if (dist >= 0.7f) {
			float distFromEdge = 1.0 - dist;  // positive when inside the circle
			float derivX = relativePos.x;
			float derivY = relativePos.y;
			float gradientLength = length(float2(derivX, derivY));
			float thresholdWidth = 0.1f * gradientLength;  // the 2.0 is a constant you can tune
			float antialiasedCircle = saturate((distFromEdge / thresholdWidth) + 0.5);
			resultColor = lerp(resultColor, getColorAtPosition(absPos), antialiasedCircle);
		}
		else {
			resultColor = lineColor;
		}
	}
	if (actualDist <= cutHighlightRadius) {
		float colorModifier = random(progress) * 0.1f;
		float modifier = (1.0f - (actualDist / cutHighlightRadius));
		float4 diffColor = float4(1 - colorModifier, 1 - colorModifier, 1 - colorModifier, (1.0f - (actualDist / cutHighlightRadius)));

		resultColor = lerp(resultColor, diffColor, modifier);
	}
	return resultColor;
}