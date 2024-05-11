#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>


cbuffer Constants : register(b0) {
	float4 colorLeft : packoffset(c0);
	float4 colorCenterLeft : packoffset(c1);
	float4 colorCenterRight : packoffset(c2);
	float4 colorRight : packoffset(c3);
	float4 colorPositions : packoffset(c4);
	uint timePassed : packoffset(c5.x);
	float tileSize : packoffset(c5.y);
	float2 dimensions : packoffset(c5.z);
	float angle : packoffset(c6.x);
	float animationSpeed : packoffset(c6.y);
}

float rand2dTo1d(float2 value, float2 dotDir) {
	float2 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return random;
}

float2 rand2dTo2d(float2 value) {
	return float2(
		rand2dTo1d(value, float2(12.989, 78.233)),
		rand2dTo1d(value, float2(39.346, 11.135))
	);
}

float2 ran(float2 uv) {
	uv *= float2(dot(uv, float2(127.1, 311.7)), dot(uv, float2(227.1, 521.7)));
	return 1.0 - frac(tan(cos(uv)*123.6)*3533.3)*frac(tan(cos(uv)*123.6)*3533.3);
}


float2 pt(float2 id) {
	return sin((timePassed * animationSpeed * 2.0f /1000.0f)*(rand2dTo2d(id + .5) - 0.5) + rand2dTo2d(id - 20.1)*8.0)*0.5;
}


D2D_PS_ENTRY(main) : SV_TARGET{
	float2 fragCoord = D2DGetScenePosition().xy;
	float2 uv = (fragCoord - (0.5f*dimensions.xy)) / dimensions.x; //D2DGetScenePosition().xy / dimensions.xy;
	float2 off = (timePassed * animationSpeed / 1000.f) / float2(50., 30.);
	uv += off;
	uv *= tileSize;

	float2 gv = frac(uv) - .5;
	float2 id = floor(uv);

	float mindist = 1e9;
	float2 vorv;
	for (float i = -1.; i <= 1.; i++) {
		for (float j = -1.; j <= 1.; j++) {
			float2 offv = float2(i, j);
			float dist = length(gv + pt(id + offv) - offv);
			if (dist < mindist) {
				mindist = dist;
				vorv = (id + pt(id + offv) + offv) / tileSize - off;
			}
		}
	}

	float modifier = clamp(vorv.x*2.f, -1., 1.)*0.5 + 0.5;

	float3 col = float3(0, 0, 0);
	if (modifier < colorPositions.y) {
		float colorModifier = smoothstep(colorPositions.x, colorPositions.y, modifier);
		float3 colorDiff = colorCenterLeft.rgb - colorLeft.rgb;
		col = colorLeft.rgb + colorDiff * colorModifier;
	}
	else if (modifier < colorPositions.z) {
		float colorModifier = smoothstep(colorPositions.y, colorPositions.z, modifier);
		float3 colorDiff = colorCenterRight.rgb - colorCenterLeft.rgb;
		col = colorCenterLeft.rgb + colorDiff * colorModifier;
	}
	else {
		float colorModifier = smoothstep(colorPositions.z, colorPositions.w, modifier);
		float3 colorDiff = colorRight.rgb - colorCenterRight.rgb;
		col = colorCenterRight.rgb + colorDiff * colorModifier;
	}
	float4 fragColor = float4(col, 1.0f); //float4(colorPositions.x, colorPositions.y, colorPositions.y, 1.0);
	return fragColor;
}