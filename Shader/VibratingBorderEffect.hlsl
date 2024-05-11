#define D2D_INPUT_COUNT 2
#define D2D_INPUT0_COMPLEX
#define D2D_INPUT1_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);
SamplerState Sampler : register(s0);
Texture2D NoiseTexture : register(t1);
SamplerState NoiseSampler : register(s1);

cbuffer Constants : register(b0)
{
    float4 borderColor : packoffset(c0);
    float2 dimensions : packoffset(c1);
    float animationSpeed : packoffset(c1.z);
    uint timePassed : packoffset(c1.w);
};

D2D_PS_ENTRY(main) : SV_TARGET{
    float4 fragColor = D2DGetInput(0); //float4(0.0, 0.0f, 0.0f, 0.0f);
    float2 fragCoord = D2DGetScenePosition().xy;
    float borderValue = 0.001f;
    float otherBorderValue = 1.0f - borderValue;
    float2 p1 = float2(borderValue, borderValue);
    float2 p2 = float2(otherBorderValue, otherBorderValue);
    float2 p3 = float2(borderValue, otherBorderValue);
    float2 p4 = float2(otherBorderValue, borderValue);

    float2 uv = fragCoord.xy / dimensions.xy;
    float2 expectedUv = uv + (timePassed / 10000.0f);
    expectedUv = frac(expectedUv);
    float4 c2 = float4(D2DSampleInput(1, expectedUv).rgb, 1.0f);
    
    float d1 = step(p1.x,uv.x)*step(uv.x,p4.x)*abs(uv.y-p1.y)+
        step(uv.x,p1.x)*distance(uv,p1)+step(p4.x,uv.x)*distance(uv,p4);
    d1 = min(step(p3.x,uv.x)*step(uv.x,p2.x)*abs(uv.y-p2.y)+
        step(uv.x,p3.x)*distance(uv,p3)+step(p2.x,uv.x)*distance(uv,p2),d1);
    d1 = min(step(p1.y,uv.y)*step(uv.y,p3.y)*abs(uv.x-p1.x)+
        step(uv.y,p1.y)*distance(uv,p1)+step(p3.y,uv.y)*distance(uv,p3),d1);
    d1 = min(step(p4.y,uv.y)*step(uv.y,p2.y)*abs(uv.x-p2.x)+
        step(uv.y,p4.y)*distance(uv,p4)+step(p2.y,uv.y)*distance(uv,p2),d1);
        
    float f1 = .01 / abs(d1 + c2.r/100.);

	fragColor += float4(f1 * borderColor.rgb, f1);

    return fragColor;
}