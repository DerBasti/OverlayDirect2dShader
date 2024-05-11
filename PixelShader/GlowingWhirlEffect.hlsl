#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>
#include "random_texture.hlsli"

cbuffer Constants : register(b0) {
	float4 leftsideColor : packoffset(c0);
	float4 rightsideColor : packoffset(c1);
	float2 dimensions : packoffset(c2);
	uint timePassed : packoffset(c2.z);
	float animationSpeed : packoffset(c2.w);
    uint animationType : packoffset(c3.x);
};

float max3(float3 color) {
    return max(max(color.r, color.g), color.b);
}

D2D_PS_ENTRY(main) : SV_TARGET{
    float2 fragCoord = D2DGetScenePosition().xy;
    float2 uv =  (2.0 * fragCoord - dimensions.xy) / min(dimensions.x, dimensions.y);
    float animatedTime = float(timePassed) / 1000.0f / animationSpeed;

    for(float i = 1.0; i < 10.0; i++){
        uv.x += 0.25 / i * sin(i * 3.5* uv.y + animatedTime);
        uv.y += 0.35 / i * cos(i * 2.0 * uv.x + animatedTime);
    }
    float4 color = lerp(leftsideColor, rightsideColor, uv.x);
    float4 fragColor = float4(float3(0.15f, 0.15f, 0.15f)/abs(sin(animatedTime-uv.y-uv.x)),1.0) * color;
    switch(animationType) {
        case 1:
            if(length(fragColor) <= 1.0f) {
                fragColor = color + color * pow(length(fragColor), 4.0f);
            }
            if(length(fragColor) >= 1.5f) {
                fragColor = color;
            } 
        break;
        case 2:
            if(length(fragColor) >= 0.99f) {
                fragColor += sqrt(color);
            }
        break;
        default: //NEON OUTLINE
            if(length(normalize(fragColor)) >= 0.75f) {
                fragColor *= pow(color, 1.25);
            }
            break;
    }
    if(animationType != 1 && length(fragColor) >= 1.9f) {
        fragColor = color * length(fragColor) / 2.0f;
    }
    fragColor += GetTextureValue(0, fragCoord);
    return fragColor;
}