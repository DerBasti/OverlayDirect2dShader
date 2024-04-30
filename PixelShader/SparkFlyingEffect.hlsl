#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

cbuffer Constants : register(b0) {
	float4 sparkColor : packoffset(c0);
	float4 smokeColor : packoffset(c1);
	float2 dimensions : packoffset(c2.x);
	uint bloomType : packoffset(c2.z);
	uint timePassed : packoffset(c2.w);
	float animationSpeed : packoffset(c3.x);
	float particleSize : packoffset(c3.y);
	float2 particleMovementDirection : packoffset(c3.z);
	uint particleAmount : packoffset(c4.x);
}

#define PI 3.1415927
#define TWO_PI 6.283185

#define ANIMATION_SPEED 1.5
#define SMOKE_MOVEMENT_DIRECTION float2(0.7, -0.5)

#define PARTICLE_SCALE (float2(0.5, 1.6))
#define PARTICLE_SCALE_VAR (float2(0.25, 0.2))

#define PARTICLE_BLOOM_SCALE (float2(0.5, 0.8))
#define PARTICLE_BLOOM_SCALE_VAR (float2(0.3, 0.1))

#define SIZE_MOD 1.5
#define ALPHA_MOD 0.9

float hash1_2(float2 x)
{
	return frac(sin(dot(x, float2(52.127, 61.2871))) * 521.582);
}

float2 hash2_2(float2 x)
{
	return frac(sin(mul(x, float2x2(20.52, 24.1994, 70.291, 80.171))) * 492.194);
}

//Simple interpolated noise
float2 noise2_2(float2 uv)
{
	//float2 f = fract(uv);
	float2 f = smoothstep(0.0, 1.0, frac(uv));

	float2 uv00 = floor(uv);
	float2 uv01 = uv00 + float2(0, 1);
	float2 uv10 = uv00 + float2(1, 0);
	float2 uv11 = uv00 + 1.0;
	float2 v00 = hash2_2(uv00);
	float2 v01 = hash2_2(uv01);
	float2 v10 = hash2_2(uv10);
	float2 v11 = hash2_2(uv11);

	float2 v0 = lerp(v00, v01, f.y);
	float2 v1 = lerp(v10, v11, f.y);
	float2 v = lerp(v0, v1, f.x);

	return v;
}

//Simple interpolated noise
float noise1_2(float2 uv)
{
	float2 f = frac(uv);

	float2 uv00 = floor(uv);
	float2 uv01 = uv00 + float2(0, 1);
	float2 uv10 = uv00 + float2(1, 0);
	float2 uv11 = uv00 + 1.0;

	float v00 = hash1_2(uv00);
	float v01 = hash1_2(uv01);
	float v10 = hash1_2(uv10);
	float v11 = hash1_2(uv11);

	float v0 = lerp(v00, v01, f.y);
	float v1 = lerp(v10, v11, f.y);
	float v = lerp(v0, v1, f.x);

	return v;
}

float layeredNoise1_2(float2 uv, float sizeMod, float alphaMod, int layers, float animation)
{
	float noise = 0.0;
	float alpha = 1.0;
	float size = 1.0;
	float2 offset;
	for (int i = 0; i < layers; i++)
	{
		offset += hash2_2(float2(alpha, size)) * 10.0;

		//Adding noise with movement
		noise += noise1_2(uv * size + (timePassed/1000.0f) * animation * 8.0 * particleMovementDirection * animationSpeed + offset) * alpha;
		alpha *= alphaMod;
		size *= sizeMod;
	}

	noise *= (1.0 - alphaMod) / (1.0 - pow(alphaMod, float(layers)));
	return noise;
}

//Rotates point around 0,0
float2 rotate(float2 vpoint, float deg)
{
	float s = sin(deg);
	float c = cos(deg);
	return mul(float2x2(s, c, -c, s), vpoint);
}

//Cell center from point on the grid
float2 voronoiPointFromRoot(float2 root, float deg)
{
	float2 vpoint = hash2_2(root) - 0.5;
	float s = sin(deg);
	float c = cos(deg);
	vpoint = mul(float2x2(s, c, -c, s), vpoint) * 0.66;
	vpoint += root + 0.5;
	return vpoint;
}

//Voronoi cell point rotation degrees
float degFromRootUV(in float2 uv)
{
	return (timePassed/1000.0f) * ANIMATION_SPEED * (hash1_2(uv) - 0.5) * 2.0;
}

float2 randomAround2_2(in float2 vpoint, in float2 range, in float2 uv)
{
	return vpoint + (hash2_2(uv) - 0.5) * range;
}

float3 fireParticles(in float2 uv, in float2 originalUV, in float angle)
{
	float3 particles = float3(0.0, 0.0, 0.0);
	float2 rootUV = floor(uv);
	float deg = degFromRootUV(rootUV);
	float2 pointUV = voronoiPointFromRoot(rootUV, deg);
	float dist = 2.0;
	float distBloom = 0.0;

	//UV manipulation for the faster particle movement
	float2 tempUV = uv + (noise2_2(uv * 2.0) - 0.5) * 0.1;
	tempUV += -(noise2_2(uv * 3.0 + (timePassed/1000.0f)) - 0.5) * 0.07;

	//Sparks sdf
	dist = length(rotate(tempUV - pointUV, angle) * randomAround2_2(PARTICLE_SCALE, PARTICLE_SCALE_VAR, rootUV));

	//Bloom sdf
	distBloom = length(rotate(tempUV - pointUV, angle) * randomAround2_2(PARTICLE_BLOOM_SCALE, PARTICLE_BLOOM_SCALE_VAR, rootUV));

	//Add sparks
	particles += (1.0 - smoothstep(particleSize * 0.6, particleSize * 3.0, dist)) * (sparkColor.rgb * 1.5f);

	//Add bloom
	particles += pow((1.0 - smoothstep(0.0, particleSize * 6.0, distBloom)) * 1.0, 8.0) * (sparkColor.rgb * 0.8f);

	//Upper disappear curve randomization
	float border = (hash1_2(rootUV) - 0.5) * 2.0;
	float disappear = 1.0 - smoothstep(border, border + 0.5, originalUV.y);

	//Lower appear curve randomization
	border = (hash1_2(rootUV + 0.214) - 1.8) * 0.7;
	float appear = smoothstep(border, border + 0.4, originalUV.y);

	return particles * disappear * appear;
}


//Layering particles to imitate 3D view
float3 layeredParticles(in float2 uv, in float sizeMod, in float alphaMod, in int layers, in float smoke, in float angle)
{
	float3 particles = float3(0.0, 0.0, 0.0);
	float size = 1.0;
	float alpha = 1.0;
	float2 offset = float2(0.0, 0.0);
	float2 noiseOffset;
	float2 bokehUV;

	for (int i = 0; i < layers; i++)
	{
		//Particle noise movement
		noiseOffset = (noise2_2(uv * size * 2.0 + 0.5) - 0.5) * 0.15;

		//UV with applied movement
		bokehUV = (uv * size + (timePassed/1000.0f) * particleMovementDirection * animationSpeed) + offset + noiseOffset;

		//Adding particles								if there is more smoke, remove smaller particles
		particles += fireParticles(bokehUV, uv, angle) * alpha * (1.0 - smoothstep(0.0, 1.0, smoke) * (float(i) / float(layers)));

		//Moving uv origin to avoid generating the same particles
		offset += hash2_2(float2(alpha, alpha)) * 10.0;

		alpha *= alphaMod;
		size *= sizeMod;
	}

	return particles;
}


D2D_PS_ENTRY(main) : SV_TARGET {
	float2 fragCoord = dimensions.xy - D2DGetScenePosition().xy;

	float2 uv = (2.0 * fragCoord - dimensions.xy) / dimensions.x;
	float vignette = 1.0 - smoothstep(0.4, 1.4, length(uv + float2(0.0, 0.2)));

	uv *= 1.8;
	float alpha = 1.0f;

	float2 allowedSmokeDimensions = float2(uv.x, uv.y);

	float smokeIntensity = layeredNoise1_2(allowedSmokeDimensions * 10.0 + (timePassed/1000.0f) * 4.0 * SMOKE_MOVEMENT_DIRECTION * animationSpeed, 1.7f, 0.7, 12, 0.2);
	smokeIntensity *= pow(1.0 - smoothstep(-1.0, 1.6, allowedSmokeDimensions.y), 2.0);
	float3 smoke = smokeIntensity * smokeColor.rgb * 0.8 * vignette;

	//Cutting holes in smoke
	smoke *= pow(layeredNoise1_2(allowedSmokeDimensions * 4.0 + (timePassed / 1000.0f) * 0.5 * SMOKE_MOVEMENT_DIRECTION * animationSpeed, 1.8, 0.5, 3, 0.2),
		2.0) * 2.0;

	smoke = float3(0, 0, 0);

	float3 particles = layeredParticles(uv, SIZE_MOD, ALPHA_MOD, particleAmount, smokeIntensity, atan2(-particleMovementDirection.y, particleMovementDirection.x) - PI/2);

	float4 col = float4(particles + smoke + smokeColor.rgb * 0.01, alpha) * vignette;
	col.rgb = smoothstep(-0.08, 1.0, col.rgb);

	float lumaMin = 0.04; float lumaMinSmooth = 0.02;
	float4 original_color = D2DGetInput(0);
	float luma = dot(col.rgb, float3(0.299, 0.587, 0.114));
	float luma_min = smoothstep(lumaMin, lumaMin + lumaMinSmooth, luma);
	col.a = clamp(luma_min, 0.0, 1.0);

	col.rgb = lerp(original_color.rgb, col.rgb, alpha); //apply alpha slider
	col = lerp(original_color, col, col.a); //remove black background color

	return col;
}