#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float4 fireColor : packoffset(c0);
	float4 backgroundColor : packoffset(c1);
	float2 dimensions : packoffset(c2.x);
	uint timePassed : packoffset(c2.z);
};

float rand(float2 n) {
	return frac(sin(cos(dot(n, float2(12.9898, 12.1414)))) * 83758.5453);
}

float noise(float2 n) {
	const float2 d = float2(0.0, 1.0);
	float2 b = floor(n), f = smoothstep(float2(0.0, 0.0f), float2(1.0, 1.0f), frac(n));
	return lerp(lerp(rand(b), rand(b + d.yx), f.x), lerp(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float fbm(float2 n) {
	float total = 0.0, amplitude = 1.0;
	for (int i = 0; i < 5; i++) {
		total += noise(n) * amplitude;
		n += n * 1.7;
		amplitude *= 0.47;
	}
	return total;
}

D2D_PS_ENTRY(main) : SV_TARGET{

	float4 fragColor = D2DGetInput(0);
	float2 fragCoord = D2DGetScenePosition().xy;
	fragCoord.y = dimensions.y - fragCoord.y;

	const float3 c1 = float3(0.5, 0.0, 0.1);
	const float3 c2 = float3(0.9, 0.1, 0.0);
	const float3 c3 = float3(0.2, 0.1, 0.7);
	const float3 c4 = float3(1.0, 0.9, 0.1);
	const float3 c5 = float3(0.1, 0.1f, 0.1f);
	const float3 c6 = float3(0.9, 0.9f, 0.9f);

	float iTime = (timePassed / 1000.0f);

	float2 speed = float2(0.1, 0.9);
	float shift = 1.327 + sin(iTime*2.0) / 2.4;
	float alpha = 1.0;

	float dist = 3.5 - sin(iTime*0.4) / 1.89;

	float2 uv = fragCoord.xy / dimensions.xy;
	float2 p = fragCoord.xy * dist / dimensions.xx;
	p += sin(p.yx*4.0 + float2(.2, -.3)*iTime)*0.04;
	p += sin(p.yx*8.0 + float2(.6, +.1)*iTime)*0.01;

	p.x -= iTime / 1.1;
	float q = fbm(p - iTime * 0.3 + 1.0*sin(iTime + 0.5) / 2.0);
	float qb = fbm(p - iTime * 0.4 + 0.1*cos(iTime) / 2.0);
	float q2 = fbm(p - iTime * 0.44 - 5.0*cos(iTime) / 2.0) - 6.0;
	float q3 = fbm(p - iTime * 0.9 - 10.0*cos(iTime) / 15.0) - 4.0;
	float q4 = fbm(p - iTime * 1.4 - 20.0*sin(iTime) / 14.0) + 2.0;
	q = (q + qb - .4 * q2 - 2.0*q3 + .6*q4) / 3.8;
	float2 r = float2(fbm(p + q / 2.0 + iTime * speed.x - p.x - p.y), fbm(p + q - iTime * speed.y));
	float3 c = lerp(c1, c2, fbm(p + r)) + lerp(c3, c4, r.x) - lerp(c5, c6, r.y);
	float3 color = float3(1.0 / (pow(c + 1.61, float3(4.0, 4.0f, 4.0f))) * cos(shift * fragCoord.y / dimensions.y));

	color = fireColor.rgb / (pow((r.y + r.y)* max(.0, p.y) + 0.1, 4.0));;
	color += backgroundColor.rgb;
	color = color / (1.0 + max(float3(0., 0., 0.), color));
	fragColor = float4(color.x, color.y, color.z, alpha);
	return fragColor;
}