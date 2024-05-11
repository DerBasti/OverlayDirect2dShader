#ifndef __VORONOI_LINES_EFFECT__
#define __VORONOI_LINES_EFFECT__

#pragma once

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_VoronoiLinesEffect, 0x1bd6b1c0, 0xdeb5, 0x409f, 0xa5, 0xee, 0x9e, 0x83, 0x34, 0xc2, 0x4c, 0x9f);
DEFINE_GUID(CLSID_VoronoiLinesEffectPixelShader, 0x21015196, 0x1402, 0x42c8, 0x85, 0xa6, 0xf, 0x8e, 0x15, 0x26, 0x29, 0xb4);

enum VoronoiLinesEffectProperty {
	VORONOI_LINES_PROPERTY_ZOOMLEVEL,
	VORONOI_LINES_PROPERTY_LINE_AMOUNT_PERCENTAGE,
	VORONOI_LINES_PROPERTY_PROGRESS,
	VORONOI_LINES_PROPERTY_ANIMATION_SPEED,
	VORONOI_LINES_PROPERTY_ANIMATION_STRENGTH,
	VORONOI_LINES_PROPERTY_SEED,
	VORONOI_LINES_PROPERTY_LINE_COLOR,
	VORONOI_LINES_PROPERTY_LINE_STRENGTH,
};


enum VoronoiLinesEffectZoomLevel {
	VORONOI_LINES_FAREST,
	VORONOI_LINES_FAR,
	VORONOI_LINES_REGULAR,
	VORONOI_LINES_NEAR,
	VORONOI_LINES_NEAREST,
	VORONOI_LINES_INVALID
};

class VoronoiLinesEffect : public CustomLeagueBaseEffect {
private:
	struct {
		float width;
		float height;
		VoronoiLinesEffectZoomLevel zoomLevel;
		float lineAmountPercentage;
		float progress;
		float animationSpeed;
		float animationStrength;
		int seed;
		D2D1_VECTOR_4F lineColor;
		float lineThickness;
	} Constants;
	VoronoiLinesEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "VoronoiLinesEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.width = (float)getWidth();
		Constants.height = (float)getHeight();
	}
	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~VoronoiLinesEffect(); 
	
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateVoronoiLinesEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline VoronoiLinesEffectZoomLevel getZoomLevel() const {
		return Constants.zoomLevel;
	}
	inline HRESULT setZoomLevel(VoronoiLinesEffectZoomLevel level) {
		Constants.zoomLevel = level;
		return S_OK;
	}
	inline float getLineAmountPercentage() const {
		return Constants.lineAmountPercentage;
	}
	inline HRESULT setLineAmountPercentage(float percentage) {
		Constants.lineAmountPercentage = (std::min)((std::max)(percentage, 0.0f), 1.0f);
		return S_OK;
	}
	inline float getProgress() const {
		return Constants.progress;
	}
	inline HRESULT setProgress(float progress) {
		Constants.progress = progress;
		return S_OK;
	}
	inline float getAnimationSpeed() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setAnimationSpeed(float speed) {
		Constants.animationSpeed = (std::min)((std::max)(speed, 0.0f), 3.0f);
		return S_OK;
	}
	inline float getAnimationStrength() const {
		return Constants.animationStrength;
	}
	inline HRESULT setAnimationStrength(float speed) {
		Constants.animationStrength = (std::min)((std::max)(speed, 0.0f), 3.0f);
		return S_OK;
	}
	inline int32_t getSeed() const {
		return Constants.seed;
	}
	inline HRESULT setSeed(int32_t seed) {
		Constants.seed = seed;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getLineColor() const {
		return Constants.lineColor;
	}
	inline HRESULT setLineColor(D2D1_VECTOR_4F color) {
		Constants.lineColor = color;
		return S_OK;
	}
	inline float getLineStrength() const {
		return Constants.lineThickness;
	}
	inline HRESULT setLineStrength(float percentage) {
		Constants.lineThickness = (std::max)(percentage, 0.01f);
		return S_OK;
	}

};

#endif //__VORONOI_LINES_EFFECT__