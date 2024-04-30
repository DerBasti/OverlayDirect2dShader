#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float4 smokeColor : packoffset(c0);
	float4 squaresColor : packoffset(c1);
	float2 dimensions : packoffset(c2);
	float distributionOverHeightPercentage : packoffset(c2.z);
	uint squaresAmount : packoffset(c2.w);
	uint timePassed : packoffset(c3);
};

float random(float2 co) {
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float noise(in float2 p)
{
	const float noiseIntensity = 2.8;
	p *= noiseIntensity;
	float2 i = floor(p);
	float2 f = frac(p);
	float2 u = f * f*(3.0 - 2.0*f);
	return lerp(lerp(random(i + float2(0.0, 0.0)),
		random(i + float2(1.0, 0.0)), u.x),
		lerp(random(i + float2(0.0, 1.0)),
			random(i + float2(1.0, 1.0)), u.x), u.y);
}

float fbm(in float2 uv)
{
	uv *= 5.0;
	float2x2 m = float2x2(1.6, 1.2, -1.2, 1.6);
	float f = 0.5000*noise(uv); uv = mul(uv, m);
	f += 0.2500*noise(uv); uv = mul(uv, m);
	f += 0.1250*noise(uv); uv = mul(uv, m);
	f += 0.0625*noise(uv); uv = mul(uv, m);

	f = 0.5 + 0.5*f;
	return f;
}

float3 bg(float2 uv)
{
	const float noiseDefinition = 0.6;
	const float2 glowPos = float2(-2., 0.);
	float velocity = (timePassed/1000.0f) / 1.6;
	float intensity = sin(uv.x*3. + velocity * 2.)*1.1 + 1.5;
	uv.y -= 2.;
	float2 bp = uv + glowPos;
	uv *= noiseDefinition;

	//ripple
	float rb = fbm(float2(uv.x*.5 - velocity * .03, uv.y))*.1;
	//rb = sqrt(rb); 
	uv += rb;

	//coloring
	float rz = fbm(uv*.9 + float2(velocity*.35, 0.0));
	rz *= dot(bp*intensity, bp) + 1.2;

	float3 col = smokeColor.rgb / (.1 - rz);
	return sqrt(abs(col));
}


float rectangle(float2 uv, float2 pos, float width, float height, float blur) {

	pos = (float2(width, height) + .01) / 2. - abs(uv - pos);
	pos = smoothstep(0., blur, pos);
	return pos.x * pos.y;

}

float2x2 rotate2d(float _angle) {
	return float2x2(cos(_angle), -sin(_angle),
		sin(_angle), cos(_angle));
}

D2D_PS_ENTRY(main) : SV_TARGET{
	float2 pixelPosition = D2DGetScenePosition().xy;
	float2 uv = pixelPosition.xy / dimensions.xy * 2. - 1.;
	uv.x *= dimensions.x / dimensions.y;

	//bg
	float3 color = bg(uv)*(2. - abs(uv.y*2.));

	const float minSize = 0.03;//rectangle min size
	const float maxSize = 0.08 - minSize;//rectangle max size

	//rectangles
	float velX = -((timePassed/1000.0f) / 8.);
	float velY = (timePassed/1000.0f) / 10.;
	for (float i = 0.; i < (float)squaresAmount; i++) {
		float index = i / (float)squaresAmount;
		float rnd = random(float2(index, index));
		float3 pos = float3(0., 0., 0.);
		pos.x = frac(velX*rnd + index)*4. - 2.0;
		pos.y = sin(index*rnd*1000. + velY) * distributionOverHeightPercentage;
		pos.z = maxSize * rnd + minSize;
		float2 uvRot = uv - pos.xy + pos.z / 2.;
		uvRot = mul(uvRot, rotate2d(i + (timePassed/1000.0f) / 2.));
		uvRot += pos.xy + pos.z / 2.;
		float rect = rectangle(uvRot, pos.xy, pos.z, pos.z, (maxSize + minSize - pos.z) / 2.);
		color += squaresColor.rgb * rect * pos.z / maxSize;
	}

	float4 fragColor = float4(color.r, color.g, color.b, 1.0);
	return fragColor;
}