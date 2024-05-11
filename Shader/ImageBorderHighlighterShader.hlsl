#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float4 borderColor : packoffset(c0);
	int borderStrength : packoffset(c1.x);
};


D2D_PS_ENTRY(main) : SV_TARGET{
	float4 imageColor = D2DGetInput(0);
	if (imageColor.a >= 0.1) {
		bool pixelFound = false;
		int borderMin = -borderStrength;
		[unroll(5)] for (int i = borderMin; i <= borderStrength && !pixelFound; i++) {
			[unroll(5)] for (int j = borderMin; j <= borderStrength && !pixelFound; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				float4 currentColor = D2DSampleInputAtOffset(0, float2(i, j));
				if (currentColor.a <= 0.1) {
					imageColor = borderColor;
					pixelFound = true;
					break;
				}
			}
		}
	}
	return imageColor;
}