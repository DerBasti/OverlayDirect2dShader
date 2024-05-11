/*
	Technical details:

	The main volume noise is generated from a deformed periodic grid, which can produce
	a large range of noise-like patterns at very cheap evalutation cost. Allowing for multiple
	fetches of volume gradient computation for improved lighting.

	To further accelerate marching, since the volume is smooth, more than half the the density
	information isn't used to rendering or shading but only as an underlying volume	distance to
	determine dynamic step size, by carefully selecting an equation	(polynomial for speed) to
	step as a function of overall density (not necessarily rendered) the visual results can be
	the	same as a naive implementation with ~40% increase in rendering performance.

	Since the dynamic marching step size is even less uniform due to steps not being rendered at all
	the fog is evaluated as the difference of the fog integral at each rendered step.

*/

#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

cbuffer Constants : register(b0) {
	float4 smokeColor : packoffset(c0);
	float2 dimensions : packoffset(c1.x);
	uint timePassed : packoffset(c1.z);
	float animationSpeed : packoffset(c1.w);
};

float2x2 rot(in float a) { float c = cos(a), s = sin(a); return float2x2(c, s, -s, c); }
float mag2(float2 p) { return dot(p, p); }
float linstep(in float mn, in float mx, in float x) { return clamp((x - mn) / (mx - mn), 0., 1.); }

float2 disp(float t) { return float2(sin(t*0.22)*1., cos(t*0.175)*1.)*2.; }

float2 map(float3 p)
{
	const float3x3 m3 = float3x3(0.33338, 0.56034, -0.71817, -0.87887, 0.32651, -0.15323, 0.15162, 0.69596, 0.61339)*1.93;
	float3 p2 = p;
	p2.xy -= disp(p.z).xy;
	p.xy = mul(rot(sin(p.z)*(0.1)), p.xy);
	float cl = mag2(p2.xy);
	float d = 0.;
	p *= .61;
	float z = 1.;
	float trk = 1.;
	float dspAmp = 0.1;
	for (int i = 0; i < 5; i++)
	{
		p += sin(p.zxy*0.75*trk + (timePassed / 1000.0f) * trk*.8)*dspAmp;
		d -= abs(dot(cos(p), sin(p.yzx))*z);
		z *= 0.57;
		trk *= 1.4;
		p = mul(m3, p);
	}
	d = abs(d) - 2.5 + ((0.5*dimensions.xy) / dimensions.y).y;
	return float2(d + cl * .2 + 0.25, cl);
}

float4 render(in float3 ro, in float3 rd, float time)
{
	float4 rez = float4(0,0,0,0);
	const float ldst = 8.;
	float3 lpos = float3(disp(time + ldst)*0.5, time + ldst);
	float t = 1.5;
	float fogT = 0.;
	for (int i = 0; i < 130; i++)
	{
		if (rez.a > 0.99)break;

		float3 pos = ro + t * rd;
		float2 mpv = map(pos);
		float den = clamp(mpv.x - 0.3, 0., 1.)*1.12;
		float dn = clamp((mpv.x + 2.), 0., 3.);

		float4 col = float4(0,0,0,0);
		if (mpv.x > 0.6)
		{

			col = float4(sin(float3(smokeColor.r, smokeColor.g, smokeColor.b)*0.8f + mpv.y*0.1 + sin(pos.z*0.4)*0.5 + 1.8)*0.5 + 0.5, 0.08);
			col *= den * den * den;
			col.rgb *= linstep(4., -2.5, mpv.x)*3.3;
			float dif = clamp((den - map(pos + .8).x) / 9., 0.001, 1.);
			dif += clamp((den - map(pos + .35).x) / 2.5, 0.001, 1.);
			col.xyz *= den * ((smokeColor.rgb/15.0f) + 1.5*(smokeColor.rgb / 50.0f)*dif);
		}

		float fogC = exp(t*0.2 - 2.2);
		col.rgba += float4(((smokeColor.rgb / 10.0f)), 0.1)*clamp(fogC - fogT, 0., 1.);
		fogT = fogC;
		rez = rez + col * (1. - rez.a);
		t += clamp(0.5 - dn * dn*.05, 0.09, 0.3);
	}
	return clamp(rez, 0.0, 1.0);
}

float getsat(float3 c)
{
	float mi = min(min(c.x, c.y), c.z);
	float ma = max(max(c.x, c.y), c.z);
	return (ma - mi) / (ma + 1e-7);
}

//from my "Will it blend" shader (https://www.shadertoy.com/view/lsdGzN)
float3 iLerp(in float3 a, in float3 b, in float x)
{
	float3 ic = lerp(a, b, x) + float3(1e-6, 0., 0.);
	float sd = abs(getsat(ic) - lerp(getsat(a), getsat(b), x));
	float3 dir = normalize(float3(2.*ic.x - ic.y - ic.z, 2.*ic.y - ic.x - ic.z, 2.*ic.z - ic.y - ic.x));
	float lgt = dot(float3(1.0,1.0,1.0), ic);
	float ff = dot(dir, normalize(ic));
	ic += 1.5*dir*sd*ff*lgt;
	return clamp(ic, 0., 1.);
}

D2D_PS_ENTRY(main) : SV_TARGET{
	float iTime = timePassed / 1000.0f * animationSpeed;
	float2 fragCoord = D2DGetScenePosition().xy;
	float4 fragColor = float4(0, 0, 0, 0);

	float2 q = fragCoord.xy / dimensions.xy;
	float2 p = (fragCoord.xy - 0.5*dimensions.xy) / dimensions.y;
	float2 bsMo = (0.5*dimensions.xy) / dimensions.y;

	float time = (timePassed / 1000.0f) * 1.5f;
	float3 ro = float3(0, 0, time);

	ro += float3(sin((timePassed / 1000.0f))*0.1, 0., 0);

	float dspAmp = .85;
	ro.xy += disp(ro.z)*dspAmp;
	float tgtDst = 15.5;

	float3 target = normalize(ro - float3(disp(time + tgtDst)*dspAmp, time + tgtDst));
	ro.x -= bsMo.x*2.;
	float3 rightdir = normalize(cross(target, float3(0, 1, 0)));
	float3 updir = normalize(cross(rightdir, target));
	rightdir = normalize(cross(updir, target));
	float3 rd = normalize((p.x*rightdir + p.y*updir)*1. - target);
	rd.xy = mul(rot(-disp(time).x*0.2 + bsMo.x), rd.xy);
	float4 scn = render(ro, rd, time);

	float3 col = scn.rgb;
	//col = iLerp(col.bgr, col.rgb, clamp(1.-prm1,0.05,1.));

	col = pow(col, float3(.55, 0.65, 0.6))*float3(1., .97, .9);

	col *= pow(16.0*q.x*q.y*(1.0 - q.x)*(1.0 - q.y), 0.12)*0.7 + 0.3; //Vign

	fragColor = float4(col, 1.0);
	return fragColor;
}