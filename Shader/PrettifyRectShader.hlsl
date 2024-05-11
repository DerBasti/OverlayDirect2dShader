#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float sizeX;
	float sizeY;
	float progress;
	float waveFrequency;
	float waveHeightOffsetPercentage;
	float waveStrength;
	float waveThickness;
	float seed;
}

float4 makeStripEffect(float4 pixelColor) {
	float4 currentPixelPosition = D2DGetScenePosition();
	float modifier = sin(currentPixelPosition.x*(waveFrequency * waveFrequency * 1000.0f) / sizeX);
	float strength = (waveThickness * sizeY / 2.0f);
	float waveCenter = (sizeY * waveHeightOffsetPercentage);
	float expectedWaveCenter = waveCenter + waveCenter * sin(modifier + progress * 100.0f * (currentPixelPosition.x+ currentPixelPosition.y) / sizeX);
	int offset = (int)currentPixelPosition.y - (int)expectedWaveCenter;
	if (abs(offset) <= strength) {
		pixelColor.r *= 1.2f;	
		pixelColor.g *= 1.2f;
		pixelColor.b *= 1.2f;
	}
	return pixelColor;
}

float random(float2 p)
{
	float2 K1 = float2(
		23.14069263277926, // e^pi (Gelfond's constant)
		2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
	);
	return frac(cos(dot(p, K1)) * 12345.6789);
}

float4 makeStripes(float4 pixelColor, float offset) {
	float4 currentPixelPosition = D2DGetScenePosition();
	float actualOffset = sqrt(offset);
	float modifierValue = (currentPixelPosition.x + progress * 30000.0f * actualOffset / offset)*sqrt(waveFrequency * (sizeY * 0.05f) * actualOffset) / sizeY;

	float modifier = offset < 0.5f ? sin(modifierValue) : offset > 1.5f ? tan(modifierValue / actualOffset) : cos(modifierValue / actualOffset);
	float waveCenter = (sizeY * waveHeightOffsetPercentage);
	float center = waveCenter + (waveCenter * modifier * actualOffset / waveStrength);
	bool allowed = abs(currentPixelPosition.y - center) <= 10.0f;
	if (allowed) {
		pixelColor.r += 0.015f;
		pixelColor.g += 0.015f;
		pixelColor.b += 0.015f;
	}
	return pixelColor;
}

float4 makeWave(float4 pixelColor) {
	float4 currentPixelPosition = D2DGetScenePosition();
	float modifier = sin(currentPixelPosition.x*(waveFrequency * 5.0f) / sizeX);
	float strength = (waveThickness * sizeY / 2.0f);
	float waveCenter = (sizeY * waveHeightOffsetPercentage);
	//Show sinus wave progress
	//float expectedWaveCenter = (waveCenter * modifier * waveStrength*0.5f) + sin(currentPixelPosition.x / sizeX * progress * 100.f)*waveCenter + waveCenter;
	
	//Wavey waves
	float expectedWaveCenter = (waveCenter + waveThickness + waveCenter * sin((currentPixelPosition.x + (progress + waveStrength)*1200.0f) * (waveFrequency*waveStrength*100.f) / sizeX) * modifier);
	float offset = (float)currentPixelPosition.y - (float)expectedWaveCenter;
	if (abs(offset) <= strength) {
		pixelColor.r *= 1.1f;
		pixelColor.g *= 1.1f;
		pixelColor.b *= 1.1f;
	}
	return pixelColor;
}

//Needs compile-flags: /D D2D_ENTRY=main 
D2D_PS_ENTRY(main) : SV_TARGET {
	float4 currentPixelPosition = D2DGetScenePosition();
	float4 pixelColor = D2DGetInput(0);
	//4.0, 0.1, 2.4, 0.7,
	float4 resultColor = makeStripes(pixelColor, 4.0f);
	resultColor = makeStripes(resultColor, 0.02f);
	resultColor = makeStripes(resultColor, 2.4f);
	resultColor = makeStripes(resultColor, 0.7f);

	return resultColor;
}