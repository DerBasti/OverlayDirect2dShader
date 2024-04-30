#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width;
	float height;
	float timePassed;
	int zoomLevel;
	float4 targetColor;
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
	return movingNoise(p + float2(x, y));
}

D2D_PS_ENTRY(main) : SV_TARGET {
	float x = D2DGetScenePosition().x / width;
	float y = D2DGetScenePosition().y / height;

	float zoomFactor = 1.0f;
	switch (zoomLevel) {
		case 1: //NEARER
			zoomFactor = 2.0f;
		break;
		case 2:
			zoomFactor = 4.0f;
		break;
		case 3:
			zoomFactor = 8.0f;
			break;
		case 4:
			zoomFactor = 16.0f;
		break;
	}
	float2 p = float2(x,y) * zoomFactor;
	float brightness = nestedNoise(p);
	float4 imageColor = D2DGetInput(0);
	float4 fragmentColor = float4(brightness * targetColor.r + imageColor.r, brightness * targetColor.g + imageColor.g, 
		brightness * targetColor.b + imageColor.b, brightness * targetColor.a + imageColor.a);
	return fragmentColor;
}