//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// This code is licensed under the MIT License (MIT). 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//*********************************************************

// Constant buffer b0 is used to store the transformation matrics from scene space
// to clip space. Depending on the number of inputs to the vertex shader, there
// may be more or fewer "sceneToInput" matrices.
#define D2D_INPUT_COUNT 0
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>


cbuffer ClipSpaceTransforms : register(b0)
{
	float2x1 sceneToOutputX;
	float2x1 sceneToOutputY;
	float2x1 sceneToInput0X;
	float2x1 sceneToInput0Y;
};

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer constants : register(b1)
{
	float4x4 rotationMatrix;
	float2 size;
	float waveOffset;
	float waveStrength;
};

struct VSIn
{
	float2 meshPosition : MESH_POSITION;
};

// It is recommended that this structure be used as vertex shader output to guarantee that
// DirectImage pixel shaders can process the result. The one variable is the number of
// "texelSpaceInput" fields, which depends on the number of inputs. This also means it 
// is recommended that this structure be the input for all custom pixel shaders.
struct VSOut
{
	float4 clipSpaceOutput   : SV_POSITION;      // Special semantic for rasterization
	float4 sceneSpaceOutput  : SCENE_POSITION;   // { SceneX, SceneY }

	// {InputTextureX, InputTextureY, deltaInputTextureXY / deltaSceneXY }
	float4 texelSpaceInput0 : TEXCOORD0;
};;

VSOut GeometryVS(VSIn input) {
	// Create the output variable.
	VSOut output;

	// First, screen space position needs to be computed. A sin function is used to generate the wavy appearance of the image.
	output.sceneSpaceOutput.x = size.x * input.meshPosition.x;
	output.sceneSpaceOutput.y = size.y * input.meshPosition.y;
	output.sceneSpaceOutput.z = sin(input.meshPosition.x * 15 + waveOffset)*waveStrength * size.x;
	output.sceneSpaceOutput.w = 1;

	// Perform a small rotation on image.
	output.sceneSpaceOutput = mul(output.sceneSpaceOutput, rotationMatrix);

	output.sceneSpaceOutput.y = (output.sceneSpaceOutput.y * (1.0 - waveStrength * 0.05));

	// Now we compute the DirectImage-required clip-space position.
	output.clipSpaceOutput.x = (output.sceneSpaceOutput.x * sceneToOutputX[0] + sceneToOutputX[1]);
	output.clipSpaceOutput.y = (output.sceneSpaceOutput.y * sceneToOutputY[0] + sceneToOutputY[1]);
	output.clipSpaceOutput.z = 1 - input.meshPosition.y;
	output.clipSpaceOutput.w = 1;

	// Warping or other modifications of the texture coordinate should be performed on
	// the resulting output.texelSpaceInput0 computed here, rather than on the values
	// passed into the shader. In this shader, no modification of the texture coordinate
	// is needed. 
	output.texelSpaceInput0.x = size.x * input.meshPosition.x * sceneToInput0X[0] + sceneToInput0X[1];
	output.texelSpaceInput0.y = size.y * input.meshPosition.y * sceneToInput0Y[0] + sceneToInput0Y[1];
	output.texelSpaceInput0.z = sceneToInput0X[0];
	output.texelSpaceInput0.w = sceneToInput0Y[0];

	return output;
}