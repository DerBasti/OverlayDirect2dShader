#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>
#include "random_texture.hlsli"

Texture3D InputTexture : register(t0);
SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float2 dimensions : packoffset(c0);
	float2 offsetFromCenter : packoffset(c0.z);
    float cellJitter : packoffset(c1.x);
    float distortion : packoffset(c1.y);
    uint filterType : packoffset(c1.z);
    uint seed : packoffset(c1.w);
};


// Credit: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(float n){return frac(sin(n) * 43758.5453123);}

float2 hash( float2 p )
{
    p = fmodGlsl(p, distortion);
    return GetScaledNoiseTextureValueAt((p + 0.5) / ((cellJitter + distortion) * 16.0f), 16.0f).xy;
}

float3 voronoi( in float2 x, out float2 cpId )
{
    float2 n = floor(x);
    float2 f = frac(x);

    //----------------------------------
    // first pass: regular voronoi
    //----------------------------------
	float2 mg, mr;

    float md = 8.0;
    int i, j;
    for( j=-1; j<=1; j++ )
    for( i=-1; i<=1; i++ )
    {
        float2 g = float2(float(i),float(j));
		float2 o = cellJitter * hash( n + g );
        float2 r = g + o - f;
        float d = dot(r,r);

        if( d<md )
        {
            md = d;
            mr = r;
            mg = g;
        }
    }

    //----------------------------------
    // second pass: distance to borders
    //----------------------------------
    md = 8.0;
    
    for( j=-2; j<=2; j++ )
    for( i=-2; i<=2; i++ )
    {
        float2 g = mg + float2(float(i),float(j));
		float2 o = cellJitter * hash( n + g );
        float2 r = g + o - f;

		
        if( dot(mr-r,mr-r)>0.000001 )
		{
            float d = dot( 0.5*(mr+r), normalize(r-mr) );
            md = min( md, d );
		}
    }
	
	cpId = n+mg;

    return float3( md, mr );
}

float remap(float x) { return x * 0.5 + 0.5; }

float2x2 rot2(float theta) {
    return float2x2(cos(theta), sin(theta),
                -sin(theta), cos(theta));
}

float atan01(float2 p) {
    return -atan2(p.y, p.x)/6.28318530718 + 0.5;
}

float noise(float p){
    float fl = floor(p);
    float fc = frac(p);
    return lerp(rand(fl), rand(fl + 1.0), fc);
}

float lowfreqnoise(float x) {
    x = fmodGlsl(x,1.0);
    float res = 10.;
    float ia = floor(x * res);
    float ib = floor(x * res) + 1.;

    float a = noise(fmodGlsl(ia, res));
    float b = noise(fmodGlsl(ib, res));
    
    float t = frac(x * res);
    return lerp(a, b, t) * 2.0 - 1.;
}

float3 image(float2 fragCoord) {
    float2 uvCenter = dimensions.xy / dimensions.x / 2. + float2(0.f, 0.f);
    float2 uv = fragCoord.xy / dimensions.x - uvCenter;
    float r = length(uv);
    
    float2 cyl = float2(max(0.5,pow(r, 0.1)),
                    atan01(uv));
    
    // Add some uneveness to lines
    cyl.x += 0.015 * abs(lowfreqnoise(cyl.y));
    float2 freq = float2(12., distortion);
    
    float2 cpid = float2(0.0f, 0.0f);
    float3 c = voronoi( cyl*freq, cpid );
    cpid = fmodGlsl(cpid, distortion);
    float centerDist = length( c.yz );
    float borderDist = c.x;
    // Make edges more even in screenspace width
    float e0 = lerp(.1, .0, pow(r, .1));
    float edge = smoothstep(e0, e0+.0001, borderDist);
    //edge = 1.;

    // rotate camera/cracked lens
    float theta = 0.0f; //iTime * 3.14 / 20.;
    float3 W = float3(uv, -0.5);
    W.xz = mul(rot2(theta), W.xz);
    float3 V = normalize(float3(0.0, 0.0, 0.0) - W);
    float3 normOffset = float3(noise(cpid.x*7.), noise(cpid.y*13.), noise(27.*(cpid.x-cpid.y))) * 2. + 4.0;
    float3 N = normalize(float3(0., 0., 1.) + 0.1*normOffset);
    float3 texturePosition = reflect(-V, N);
    float3 env = InputTexture.Sample(Sampler, reflect(-V, N)).rgb; //D2DSampleInputAtPosition(0, reflect(-V, N)).rgb;
    float3 F = float3(0.0f, 0.0f, 0.0f);
    switch(filterType) {
        case 1:
            F = lerp(env, env + hash(uv*128.).x, 1.0f - edge);
            F = lerp(F, float3(0.5, 0.9, 1.0)*0.7, 1.0-edge) * 1.7f;    
        break;
        default:
   	        F = lerp(float3(1., 1., 1.), float3(0.5, 0.9, 1.0)*0.4, 1.0-edge);
        break;
    }
    float3 lit = env * F;
    return lit;
}

D2D_PS_ENTRY(main) : SV_TARGET{
    // FSAA, box filter
    const int L = 3;
    const float Lf = float(L);
    float3 sum = float3(0., 0., 0.);
	float2 fragCoord = D2DGetScenePosition().xy;
    for(int i = 0; i < L; i++) {
        for(int j = 0; j < L; j++) {
            float2 ofs = float2(float(i), float(j)) / Lf;
            sum += image(fragCoord + ofs);
        }
    }
    float3 result = sum/(Lf*Lf);
	float4 fragColor = float4(result.r, result.g, result.b, 1.0);
    return fragColor;
}