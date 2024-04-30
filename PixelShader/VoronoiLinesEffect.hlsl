#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX
#define D2D_REQUIRES_SCENE_POSITION
#include <d2d1effecthelpers.hlsli>

Texture2D InputTexture : register(t0);

SamplerState Sampler : register(s0);

cbuffer Constants : register(b0) {
	float width;
	float height;
	int zoomLevel;
	float amountPercentage;
	float progress;
	float animationSpeed;
	float animationStrength;
	int _seed;
	float4 targetColor;
	float lineThickness;
};

float random(float p) {
	return frac(sin(p)*10000.);
}


float random2(float2 p) {
	return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

float2 getDirectionToCurrentPoint(int2 localCellIndex, int2 worldCellIndex, float2 relativePos)
{
	float2 cellLocalCenter = float2(0.5, 0.5);

	// Random position in the cell
	float _Seed = float(_seed) + random(float(_seed));
	int2 coord = worldCellIndex + localCellIndex;
	float2 seed = float(_Seed) * float2(coord);
	float fractionalCoord = random2(seed);

	//animation
	float _AnimationStrength = animationStrength;
	float  _AnimationAmount = pow(amountPercentage, 8);
	float _AnimationSpeed = animationSpeed;
	float2 animatedCoord = saturate(cellLocalCenter + (float(0.5f) * sin(fractionalCoord * progress * _AnimationSpeed)) * _AnimationStrength);
	float2 localCoord = lerp(fractionalCoord, animatedCoord, saturate(_AnimationAmount));

	//voronoi amount
	float _VoronoiAmount = amountPercentage;
	float2 voronoiAmount = (1. - _VoronoiAmount) * cellLocalCenter + _VoronoiAmount * localCoord;

	//direction from current position to randomly generated point
	float2 dirToCurrentPoint = (float2(localCellIndex)+voronoiAmount) - relativePos;
	return dirToCurrentPoint;
}

float calculateDistanceToCurrentPoint() {
	float scale = 1.0f;
	float imageDimensionsAsLength = length(float2(width, height));
	switch (zoomLevel) {
		case 0: //FAREST
			scale = 35.0f;
		break;
		case 1: //FAR
			scale = 20.0f;
		break;
		case 2: //REGULAR
			scale = 10.0f;
		break;
		case 3: //NEAR
			scale = 5.0f;
		break;
		case 4: //NEAREST
			scale = 2.25f;
		break;
	}
	float2 st = (D2DGetScenePosition().xy / float2(width, height)) * scale;
	int2 worldCellIndex = floor(st);
	float2 relativePos = frac(st);

	int2 closestLocalCellIndex;
	int2 closestWorldCellIndex;
	float2 dirToclosestPoint;

	float m_dist = -1;
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			// neighbor place in the grid
			int2 localCellIndex = int2(x, y);
			float2 dirToCurrentPoint = getDirectionToCurrentPoint(localCellIndex, worldCellIndex, relativePos);
			float sqrDist = dot(dirToCurrentPoint, dirToCurrentPoint);

			if (sqrDist < m_dist || m_dist == -1.)
			{
				m_dist = sqrDist;
				dirToclosestPoint = dirToCurrentPoint;
				closestLocalCellIndex = localCellIndex;
				closestWorldCellIndex = worldCellIndex + localCellIndex;
			}
		}
	}

	m_dist = -1;
	for (int j = -2; j <= 2; j++)
	{
		for (int i = -2; i <= 2; i++)
		{
			int2 localCellIndex = closestLocalCellIndex + int2(i, j);
			float2 dirCurrentPoint = getDirectionToCurrentPoint(localCellIndex, worldCellIndex, relativePos);
			float d = sqrt(dot(dirToclosestPoint + dirCurrentPoint, normalize(dirCurrentPoint - dirToclosestPoint)));

			if (d < m_dist || m_dist == -1.)
			{
				m_dist = d;
			}
		}
	}
	return m_dist;
}


D2D_PS_ENTRY(main) : SV_TARGET{
	float4 color = D2DGetInput(0);
	float noise = calculateDistanceToCurrentPoint();
	float4 calcTargetColor = float4(targetColor.r, targetColor.g, targetColor.b, targetColor.a / 100.0f);
	float threshold = pow(lineThickness / (lineThickness + 1.0f), 1.25f); //3 / 4 / 2 = 0.375
	if (noise <= 0.3) {
		float4 colorDiff = calcTargetColor - color;
		float modifier = (threshold - noise) / 0.9f;
		color = float4(color.r + colorDiff.r * modifier, color.g + colorDiff.g * modifier, color.b + colorDiff.b * modifier, targetColor.a / 100.0f);
	}
	return color;
}