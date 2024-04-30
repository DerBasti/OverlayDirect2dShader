#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

cbuffer Constants : register(b0) {
	float4 smokeColor : packoffset(c0);
	float2 canvasOffset : packoffset(c1.x);
	float2 dimensions : packoffset(c1.z);
	uint timePassed : packoffset(c2.x);
	float animationSpeed : packoffset(c2.y);
};

float2 tmpResult;

float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233)) {
	float2 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return random;
}

float3 rand2dTo3d(float2 value) {
	return float3(
		rand2dTo1d(value, float2(12.989, 78.233)),
		rand2dTo1d(value, float2(39.346, 11.135)),
		rand2dTo1d(value, float2(73.156, 52.235))
	);
}

float4 rgbNoise(float2 dim) {
	return float4(rand2dTo3d(dim), 1.0f);
}

float glslMod(float x, float y) {
	return abs(x - (y * floor(x / y)));
}

float4 rgbNoise256(float2 dim) {
	float xMod = dim.x < 0.0 ? 1.0f - abs(frac(dim.x)) : frac(dim.x);
	float yMod = dim.y < 0.0 ? 1.0f - abs(frac(dim.y)) : frac(dim.y);
	uint minX = uint(xMod * dimensions.x);
	uint minY = uint(yMod * dimensions.y);
	return rgbNoise(float2(minX, minY));

}

float noise(in float3 x, out float2 outRg)
{
	float3 p = floor(x);
	float3 f = frac(x);
	f = f * f*(3.0 - 2.0*f);

	float2 uv = (p.xy + float2(37.0, 17.0)*p.z) + f.xy;
	float2 rg = rgbNoise256((uv + 0.5) / 256.0f).yx;//NoiseTexture.SampleLevel(NoiseSampler, (uv + 0.5) / 512.0, 0 ).yx;
	outRg = uv;
	return lerp(rg.x, rg.y, f.z);
}

float4 map(float3 p, out float2 outRg)
{
	float iTime = timePassed / 1000.0f * animationSpeed;
	float den = 0.2f-p.y;

	// invert space	
	p = -7.0*p / dot(p, p);

	// twist space	
	float co = cos(den - 0.25*iTime);
	float si = sin(den - 0.25*iTime);
	p.xz = mul(p.xz, float2x2(co, -si, si, co));

	// smoke	
	float f;
	float3 q = p - float3(0.0, 1.0, 0.0)*iTime;;
	f = 0.50000*noise(q, outRg); q = q * 2.02 - float3(0.0, 1.0, 0.0)*iTime;
	f += 0.25000*noise(q, outRg); q = q * 2.03 - float3(0.0, 1.0, 0.0)*iTime;
	f += 0.12500*noise(q, outRg); q = q * 2.01 - float3(0.0, 1.0, 0.0)*iTime;
	f += 0.06250*noise(q, outRg); q = q * 2.02 - float3(0.0, 1.0, 0.0)*iTime;
	f += 0.03125*noise(q, outRg);

	den = den + 4.0*f;

	float3 col = lerp(float3(1.0, 0.9, 0.8), float3(0.4, 0.15, 0.1), den) + 0.05*sin(p);

	return float4(col, den);
}

float3 raymarch(in float3 fireRotation, in float3 rd, in float2 pixel, out float2 outRg)
{
	float iTime = timePassed / 1000.0f * animationSpeed;
	float4 sum = float4(0,0,0,0);

	// dithering	
	float t = 0.05*frac(10.5421*dot(float2(0.0149451, 0.038921), pixel));

	for (int i = 0; i < 150; i++)
	{
		float3 pos = fireRotation + t * rd;
		float4 col = map(pos, outRg);
		if (col.w > 0.0)
		{
			//float len = length(pos);
			col.w = min(col.w, 1.0);

			col.xyz *= lerp(3.1*smokeColor.rgb, float3(0.48, 0.53, 0.5), clamp((pos.y - 0.2) / 1.9, 0.0, 1.0));
			//col.xyz *= mix( 3.1*vec3(1.0,0.5,0.05), vec3(0.48,0.53,0.5), clamp( 0.35*col.w+0.15*dot(pos,pos), 0.0, 1.0 ) );

			col.a *= 0.6;
			col.rgb *= col.a;

			sum = sum + col * (1.0 - sum.a);
			if (sum.a > 0.99) break;
		}
		t += 0.05;
	}

	return clamp(sum.xyz, 0.0, 1.0);
}


D2D_PS_ENTRY(main) : SV_TARGET{
	
	float iTime = timePassed / 1000.0f * animationSpeed;
	float2 fragCoord = dimensions.xy - D2DGetScenePosition().xy;
	float4 fragColor = float4(0, 0, 0, 0);
	//if (fragCoord.x <= dimensions.x && fragCoord.y <= dimensions.y) {
		float2 p = (2.0*fragCoord - dimensions) / dimensions.y; //(2.0*fragCoord - dimensions) / dimensions.y;

		// camera
		float3 fireRotation = 4.0f * normalize(float3(1.0f, 1.5f, 0.0));
		float3 ta = float3(0.0, 1.0f, 0.0);// +0.05*rgbNoise(iTime*float2(0.013, 0.008)).xyz;
		float cr = 0.5*cos(0.7*iTime);

		// build ray
		float3 ww = normalize(ta - fireRotation);
		float3 uu = normalize(cross(float3(sin(cr), cos(cr), 0.0), ww));
		float3 vv = normalize(cross(ww, uu));
		float3 rd = normalize(p.x*uu + p.y*vv + 2.0*ww);

		// raymarch	
		float2 outRg = float2(0, 0);
		float3 col = raymarch(fireRotation, rd, fragCoord, outRg);

		// color grade
		col = col * 0.5 + 0.5*col*col*(3.0 - 2.0*col);

		// vignetting	
		float2 q = D2DGetScenePosition().xy / dimensions.xy;
		col *= 0.2 + 0.8*pow(16.0*q.x*q.y*(1.0 - q.x)*(1.0 - q.y), 0.1);

		fragColor = float4(col, 1.0f);
	//}
	return fragColor;
}