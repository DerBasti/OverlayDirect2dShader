#ifndef __DIRECT2D_MIST_WAVE_TWO_EDGES_EFFECT__
#define __DIRECT2D_MIST_WAVE_TWO_EDGES_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_MistWaveEffect, 0x5a4af061, 0x4ca9, 0x4a79, 0xa3, 0x36, 0x31, 0x17, 0x52, 0xde, 0x70, 0xa1);
DEFINE_GUID(CLSID_MistWaveEffectPixelShader, 0x83a15767, 0x3f80, 0x4ebd, 0x99, 0x1d, 0xbb, 0x90, 0xff, 0xb6, 0xa4, 0xac);

enum MistWaveEffectProperty {
	MIST_WAVE_EFFECT_PROPERTY_TIME_PASSED,
	MIST_WAVE_EFFECT_ZOOM_LEVEL,
	MIST_WAVE_EFFECT_COLOR
};


enum MistWaveEffectZoomLevel {
	NEAREST,
	NEARER,
	REGULAR,
	MORE_FAR,
	FAREST,
	INVALID
};

class MistWaveEffect : public CustomLeagueBaseEffect {
private:
	MistWaveEffect();
	struct {
		float width; 
		float height;
		float timePassed;
		MistWaveEffectZoomLevel zoomLevel;
		D2D1_VECTOR_4F color;
	} Constants;
protected:
	virtual const char* getShaderFileName() const {
		return "MistWaveEffect";
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
	virtual ~MistWaveEffect() {}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateMistWaveEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline float getTimePassed() const {
		return Constants.timePassed;
	}
	inline HRESULT setTimePassed(float timePassedNew) {
		Constants.timePassed = timePassedNew;
		return S_OK;
	}	
	inline uint32_t getZoomLevel() const {
		return static_cast<uint32_t>(Constants.zoomLevel);
	}
	inline HRESULT setZoomLevel(uint32_t zoomLevel) {
		Constants.zoomLevel = static_cast<MistWaveEffectZoomLevel>(zoomLevel);
		return S_OK;
	}
	inline D2D1_VECTOR_4F getTargetColor() const {
		return Constants.color;
	}
	inline HRESULT setTargetColor(D2D1_VECTOR_4F targetColor) {
		Constants.color = targetColor;
		return S_OK;
	}
};

#endif //__DIRECT2D_MIST_WAVE_TWO_EDGES_EFFECT__