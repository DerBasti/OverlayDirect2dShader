#ifndef __VORONOI_GRADIENT_EFFECT__
#define __VORONOI_GRADIENT_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_VoronoiGradientEffect, 0xcd80225c, 0x604c, 0x48d7, 0x8c, 0x65, 0x14, 0x9, 0xbd, 0xcf, 0x6c, 0xf0);
DEFINE_GUID(CLSID_VoronoiGradientEffectPixelShader, 0x2ce56e5e, 0x47e7, 0x4d89, 0x91, 0xe7, 0x90, 0x6d, 0xae, 0xf7, 0xdc, 0xdb);

enum VoronoiGradientEffectProperties {
	VORONOI_GRADIENT_EFFECT_PROPERTY_DIMENSIONS,
	VORONOI_GRADIENT_EFFECT_PROPERTY_COLOR_LEFT,
	VORONOI_GRADIENT_EFFECT_PROPERTY_COLOR_MIDDLE_LEFT,
	VORONOI_GRADIENT_EFFECT_PROPERTY_COLOR_MIDDLE_RIGHT,
	VORONOI_GRADIENT_EFFECT_PROPERTY_COLOR_RIGHT,
	VORONOI_GRADIENT_EFFECT_PROPERTY_COLOR_POSITIONS,
	VORONOI_GRADIENT_EFFECT_PROPERTY_TILE_SIZE,
	VORONOI_GRADIENT_EFFECT_PROPERTY_ANGLE,
	VORONOI_GRADIENT_EFFECT_PROPERTY_ANIMATION_SPEED,
};

class VoronoiGradientEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_4F leftColor;
		D2D1_VECTOR_4F middleLeftColor;
		D2D1_VECTOR_4F middleRightColor;
		D2D1_VECTOR_4F rightColor;
		D2D1_VECTOR_4F colorPositions;
		uint32_t timePassedInMillis;
		float tileSize;
		D2D1_VECTOR_2F dimensions;
		float angle;
		float animationSpeed;
	} Constants;
protected:
	VoronoiGradientEffect();
	virtual const char* getShaderFileName() const {
		return "VoronoiGradientEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.timePassedInMillis = (uint32_t)clock.getDuration();
		if (Constants.dimensions.x == 0 && Constants.dimensions.y == 0) {
			Constants.dimensions = D2D1::Vector2F((float)getWidth(), (float)getHeight());
		}
	}
	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~VoronoiGradientEffect();

	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateVoronoiGradientEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getColorLeft() const {
		return Constants.leftColor;
	}
	inline HRESULT setColorLeft(D2D1_VECTOR_4F color) {
		Constants.leftColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getColorMiddleLeft() const {
		return Constants.middleLeftColor;
	}
	inline HRESULT setColorMiddleLeft(D2D1_VECTOR_4F color) {
		Constants.middleLeftColor = color;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getColorMiddleRight() const {
		return Constants.middleRightColor;
	}
	inline HRESULT setColorMiddleRight(D2D1_VECTOR_4F color) {
		Constants.middleRightColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getColorRight() const {
		return Constants.rightColor;
	}
	inline HRESULT setColorRight(D2D1_VECTOR_4F color) {
		Constants.rightColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getColorPositions() const {
		return Constants.colorPositions;
	}
	inline HRESULT setColorPositions(D2D1_VECTOR_4F colorPositions) {
		Constants.colorPositions = colorPositions;
		return S_OK;
	}
	inline float getTileSize() const {
		return Constants.tileSize;
	}
	inline HRESULT setTileSize(float tileSize) {
		Constants.tileSize = tileSize;
		return S_OK;
	}
	inline float getAngle() const {
		return Constants.angle;
	}
	inline HRESULT setAngle(float angle) {
		Constants.angle = angle;
		return S_OK;
	}
	inline float getAnimationSpeed() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setAnimationSpeed(float animationSpeed) {
		Constants.animationSpeed = animationSpeed;
		return S_OK;
	}
};

#endif //__VORONOI_GRADIENT_EFFECT__