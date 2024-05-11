#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION

#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

D2D_PS_ENTRY(main) : SV_TARGET {
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}