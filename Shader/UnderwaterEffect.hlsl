#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float2 dimensions : packoffset(c0.x);
	uint timePassed : packoffset(c0.z);
	float animationSpeed : packoffset(c0.w);
};

// Created by greenbird10
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0

float hash(float2 p) {
	return 0.5*(
		sin(dot(p, float2(271.319, 413.975)) + 1217.13*p.x*p.y)
		) + 0.5;
}

float noise(float2 p) {
	float2 w = frac(p);
	w = w * w * (3.0 - 2.0*w);
	p = floor(p);
	return lerp(
		lerp(hash(p + float2(0, 0)), hash(p + float2(1, 0)), w.x),
		lerp(hash(p + float2(0, 1)), hash(p + float2(1, 1)), w.x), w.y);
}

// wave octave inspiration
// Alexander Alekseev - Seascape
// https://www.shadertoy.com/view/Ms2SD1
float map_octave(float2 uv) {
	uv = (uv + noise(uv)) / 2.5;
	uv = float2(uv.x*0.6 - uv.y*0.8, uv.x*0.8 + uv.y*0.6);
	float2 uvsin = 1.0 - abs(sin(uv));
	float2 uvcos = abs(cos(uv));
	uv = lerp(uvsin, uvcos, uvsin);
	float val = 1.0 - pow(uv.x * uv.y, 0.65);
	return val;
}

float map(float3 p) {
	float2 uv = p.xz + (timePassed / 1000.0f) / 2.;
	float amp = 0.6, freq = 2.0, val = 0.0;
	for (int i = 0; i < 3; ++i) {
		val += map_octave(uv) * amp;
		amp *= 0.3;
		uv *= freq;
		// uv = float2(uv.x*0.6-uv.y*0.8, uv.x*0.8+uv.y*0.6);
	}
	uv = p.xz - 1000. - (timePassed / 1000.0f) / 2.;
	amp = 0.6, freq = 2.0;
	for (int j = 0; j < 3; ++j) {
		val += map_octave(uv) * amp;
		amp *= 0.3;
		uv *= freq;
		// uv = float2(uv.x*0.6-uv.y*0.8, uv.x*0.8+uv.y*0.6);
	}
	return val + 3.0 - p.y;
}

float3 getNormal(float3 p) {
	float eps = 1. / dimensions.x;
	float3 px = p + float3(eps, 0, 0);
	float3 pz = p + float3(0, 0, eps);
	return normalize(float3(map(px), eps, map(pz)));
}

// raymarch inspiration
// Alexander Alekseev - Seascape
// https://www.shadertoy.com/view/Ms2SD1
float raymarch(float3 ro, float3 rd, out float3 outP, out float outT) {
	float l = 0., r = 26.;
	int i = 0, steps = 16;
	float dist = 1000000.;
	for (i = 0; i < steps; ++i) {
		float mid = (r + l) / 2.;
		float mapmid = map(ro + rd * mid);
		dist = min(dist, abs(mapmid));
		if (mapmid > 0.) {
			l = mid;
		}
		else {
			r = mid;
		}
		if (r - l < 1. / dimensions.x) break;
	}
	outP = ro + rd * l;
	outT = l;
	return dist;
}

float fbm(float2 n) {
	float total = 0.0, amplitude = 1.0;
	for (int i = 0; i < 5; i++) {
		total += noise(n) * amplitude;
		n += n;
		amplitude *= 0.4;
	}
	return total;
}

float lightShafts(float2 st) {
	float angle = -0.2;
	float2 _st = st;
	float t = (timePassed / 1000.0f) / 16.;
	st = float2(st.x * cos(angle) - st.y * sin(angle),
		st.x * sin(angle) + st.y * cos(angle));
	float val = fbm(float2(st.x*2. + 200. + t, st.y / 4.));
	val += fbm(float2(st.x*2. + 200. - t, st.y / 4.));
	val = val / 3.;
	float mask = pow(clamp(1.0 - abs(_st.y - 0.15), 0., 1.)*0.49 + 0.5, 2.0);
	mask *= clamp(1.0 - abs(_st.x + 0.2), 0., 1.) * 0.49 + 0.5;
	return pow(val*mask, 2.0);
}

float2 bubble(float2 uv, float scale) {
	if (uv.y > 0.2) return float2(0.0f,0.0f);
	float t = (timePassed / 1000.0f) / 4.;
	float2 st = uv * scale;
	float2 _st = floor(st);
	float2 bias = float2(0., 4. * sin(_st.x*128. + t));
	float mask = smoothstep(0.1, 0.2, -cos(_st.x*128. + t));
	st += bias;
	float2 _st_ = floor(st);
	st = frac(st);
	float size = noise(_st_)*0.07 + 0.01;
	float2 pos = float2(noise(float2(t, _st_.y*64.1)) * 0.8 + 0.1, 0.5);
	if (length(st.xy - pos) < size) {
		return (st + pos) * float2(.1, .2) * mask;
	}
	return float2(0.0f, 0.0f);
}

D2D_PS_ENTRY(main) : SV_TARGET{
	float4 fragColor = D2DGetInput(0);
	float3 ro = float3(0., 0., 2.);
	float3 lightPos = float3(8, 3, -3);
	float3 lightDir = normalize(lightPos - ro);

	// adjust uv
	float2 fragCoord = D2DGetScenePosition().xy;
	float2 uv = fragCoord;
	uv = (fragCoord - (0.5f*dimensions.xy)) / dimensions.y;
	//uv = (-dimensions.xy + 2.0*uv) / dimensions.y;
	uv.y *= -0.5;
	uv.x *= 0.45;
	uv += bubble(uv, 12.) + bubble(uv, 24.); // add bubbles

	float3 rd = normalize(float3(uv, -1.));
	float3 hitPos;
	float hitT;
	float3 seaColor = float3(11, 82, 142) / 255.;
	float3 color;

	// waves
	float dist = raymarch(ro, rd, hitPos, hitT);
	float diffuse = dot(getNormal(hitPos), rd) * 0.5 + 0.5;
	color = lerp(seaColor, float3(15, 120, 152) / 255., diffuse);
	color += pow(diffuse, 12.0);
	// refraction
	float3 ref = normalize(refract(hitPos - lightPos, getNormal(hitPos), 0.05));
	float refraction = clamp(dot(ref, rd), 0., 1.0);
	color += float3(245, 250, 220) / 255. * 0.6 * pow(refraction, 1.5);

	float3 col = float3(0.0f, 0.0f, 0.0f);
	col = lerp(color, seaColor, pow(clamp(0., 1., dist), 0.2)); // glow edge
	col += float3(225, 230, 200) / 255. * lightShafts(uv); // light shafts

	// tone map
	col = (col*col + sin(col)) / float3(1.8, 1.8, 1.9);

	// vignette
	// inigo quilez - Stop Motion Fox 
	// https://www.shadertoy.com/view/3dXGWB
	float2 q = fragCoord / dimensions.xy;
	col *= 0.7 + 0.3*pow(16.0*q.x*q.y*(1.0 - q.x)*(1.0 - q.y), 0.2);

	fragColor = float4(col, 1.0);
	return fragColor;
}
