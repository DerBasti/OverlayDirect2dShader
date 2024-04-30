#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width : packoffset(c0.x);
	float height : packoffset(c0.y);
	float2 axisPointFirst : packoffset(c0.z);
	float2 axisPointSecond : packoffset(c1.x);
	float2 leftSideColorFirstPart : packoffset(c1.z);
	float2 leftSideColorSecondPart : packoffset(c2.x);
	float2 rightSideColorFirstPart : packoffset(c2.z);
	float2 rightSideColorSecondPart : packoffset(c3.x);
	uint timePassed : packoffset(c3.z);
};

float random(float p) {
	return frac(sin(p)*10000.);
}

float noise(float2 p) {
	return random(p.x + p.y*10000.);
}

float2 sw(float2 p) { return float2(floor(p.x), floor(p.y)); }
float2 se(float2 p) { return float2(ceil(p.x), floor(p.y)); }
float2 nw(float2 p) { return float2(floor(p.x), ceil(p.y)); }
float2 ne(float2 p) { return float2(ceil(p.x), ceil(p.y)); }

float smoothNoise(float2 p) {
	float2 inter = smoothstep(0., 1., frac(p));
	float s = lerp(noise(sw(p)), noise(se(p)), inter.x);
	float n = lerp(noise(nw(p)), noise(ne(p)), inter.x);
	return lerp(s, n, inter.y);
}

float movingNoise(float2 p) {
	float total = -0.8f;
	total += smoothNoise(p - timePassed);
	total += smoothNoise(p*2. + timePassed) / 2.;
	total += smoothNoise(p*4. - timePassed) / 4.;
	total += smoothNoise(p*8. + timePassed) / 8.;
	total += smoothNoise(p*16. - timePassed) / 16.;
	total /= 1. + 1. / 2. + 1. / 4. + 1. / 8. + 1. / 16.;
	return total;
}

float nestedNoise(float2 p) {
	float x = movingNoise(p);
	float y = movingNoise(p + 100.);
	return lerp(0.5, 0.9, movingNoise(p + float2(x, y)));
}

D2D_PS_ENTRY(main) : SV_TARGET {
	float4 currentColor = D2DGetInput(0);
	float2 currentPosition = D2DGetScenePosition().xy;
	float distanceX = (axisPointSecond.x - axisPointFirst.x) * width;
	float distanceY = (axisPointSecond.y - axisPointFirst.y) * height;

	const float DEFAULT_ALLOWED_DISTANCE = 0.01f*width;
	float xRatio = currentPosition.y / height;

	int distance = (int)currentPosition.x - ((int)((xRatio*distanceX) + (axisPointFirst.x*width)));
	float4 targetColor = currentColor;

	float2 scalarPosition = float2(currentPosition.x / width, currentPosition.y / height);
	float maxDistanceAllowed = DEFAULT_ALLOWED_DISTANCE + lerp(0.001f, 0.029f, nestedNoise(scalarPosition))*width;
	if (distance >= 0 && distance <= maxDistanceAllowed) {
		targetColor = float4(rightSideColorFirstPart.x, rightSideColorFirstPart.y, rightSideColorSecondPart.x, rightSideColorSecondPart.y);
		targetColor = targetColor + (targetColor * nestedNoise(scalarPosition)/2);
	} 
	else if(distance <= 0 && distance >= -maxDistanceAllowed) {
		targetColor = float4(leftSideColorFirstPart.x, leftSideColorFirstPart.y, leftSideColorSecondPart.x, leftSideColorSecondPart.y);
		targetColor = targetColor + (targetColor * nestedNoise(scalarPosition)/2);
	}
	float4 diffColor = targetColor - currentColor;
	currentColor = diffColor * (1.0f - (abs(distance) / maxDistanceAllowed)) + currentColor;
	return currentColor;
}