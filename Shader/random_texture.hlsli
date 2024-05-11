#ifndef __RANDOM_TEXTURE__
#define __RANDOM_TEXTURE__

#if defined(__D2D_DEFINE_PS_GLOBALS)
	#define GetTextureValue(index, texturePosition) InputTexture##index.Sample(InputSampler##index, texturePosition)
	#define GetTextureValueBias(index, texturePosition, bias) InputTexture##index.Sample(InputSampler##index, texturePosition, bias)
#endif

float rand2dTo1d(float2 value, float2 dotDir) {
	float2 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return random;
}

float hash11(float p)
{
    p = frac(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return frac(p);
}

float hash12(float2 p)
{
	float3 p3  = frac(float3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.x + p3.y) * p3.z);
}

float hash13(float3 p3)
{
	p3  = frac(p3 * .1031);
    p3 += dot(p3, p3.zyx + 31.32);
    return frac((p3.x + p3.y) * p3.z);
}

float2 hash22(float2 p)
{
	float3 p3 = frac(float3(p.xyx) * float3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return frac((p3.xx+p3.yz)*p3.zy);

}


float2 hash23(float3 p3)
{
	p3 = frac(p3 * float3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return frac((p3.xx+p3.yz)*p3.zy);
}

float2 GetNoiseTextureValueAt(float2 value) {
	return float2(
		rand2dTo1d(floor(value), float2(12.989, 78.233)),
		rand2dTo1d(floor(value), float2(39.346, 11.135))
	);
}

float fmodGlsl(float x, float y) {
	return x - y * floor(x/y);
}

float2 GetScaledNoiseTextureValueAt(float2 value, float noiseCubeSize) {
	float2 scaledValue = float2(fmodGlsl(value.x, noiseCubeSize), fmodGlsl(value.y, noiseCubeSize));
	return float2(
		rand2dTo1d(scaledValue, float2(12.989, 78.233)),
		rand2dTo1d(scaledValue, float2(39.346, 11.135))
	);
}


float2 fmodGlsl(float2 vec, float y) {
	return float2(fmodGlsl(vec.x, y), fmodGlsl(vec.y, y));
}

float2 ran(float2 uv) {
	uv *= float2(dot(uv, float2(127.1, 311.7)), dot(uv, float2(227.1, 521.7)));
	return 1.0 - frac(tan(cos(uv)*123.6)*3533.3)*frac(tan(cos(uv)*123.6)*3533.3);
}

#endif //__RANDOM_TEXTURE__