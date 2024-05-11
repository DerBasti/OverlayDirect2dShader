#ifndef __DIRECT2D_MIST_WAVE_EFFECT__
#define __DIRECT2D_MIST_WAVE_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_MistWaveTwoEdgesEffect, 0x4b64b5c, 0x39f, 0x4ae7, 0x81, 0x44, 0x1f, 0x8d, 0xd, 0x1e, 0xfe, 0xf9);
DEFINE_GUID(CLSID_MistWaveTwoEdgesEffectPixelShader, 0xd3a2b32e, 0x7eb7, 0x4f66, 0x9b, 0x83, 0x8, 0x1b, 0x0, 0x7c, 0x81, 0xf3);

enum MistWaveTwoEdgesEffectProperty {
	MIST_WAVE_TWO_EDGES_EFFECT_PROPERTY_TIME_PASSED,
	MIST_WAVE_TWO_EDGES_EFFECT_ZOOM_LEVEL,
	MIST_WAVE_TWO_EDGES_EFFECT_COLOR_BACKGROUND,
	MIST_WAVE_TWO_EDGES_EFFECT_COLOR_LEFTSIDE,
	MIST_WAVE_TWO_EDGES_EFFECT_COLOR_RIGHTSIDE,
};

enum MistWaveTwoEdgesEffectZoomLevel {
	MIST_WAVE_TWO_EDGES_NEAREST,
	MIST_WAVE_TWO_EDGES_NEARER,
	MIST_WAVE_TWO_EDGES_REGULAR,
	MIST_WAVE_TWO_EDGES_MORE_FAR,
	MIST_WAVE_TWO_EDGES_FAREST,
	MIST_WAVE_TWO_EDGES_INVALID
};


class MistWaveTwoEdgesEffect : public CustomLeagueBaseEffect {
private:
	MistWaveTwoEdgesEffect();
	struct {
		float width;
		float height;
		float timePassed;
		MistWaveTwoEdgesEffectZoomLevel zoomLevel;
		D2D1_VECTOR_4F backgroundColor;
		D2D1_VECTOR_4F leftColor;
		D2D1_VECTOR_4F rightColor;
	} Constants;
protected:
	virtual const char* getShaderFileName() const {
		return "MistWaveTwoEdgesEffect";
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
	virtual ~MistWaveTwoEdgesEffect() {}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateMistWaveTwoEdgesEffect(_Outptr_ IUnknown** ppEffectImpl);

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
		Constants.zoomLevel = static_cast<MistWaveTwoEdgesEffectZoomLevel>(zoomLevel);
		return S_OK;
	}
	inline D2D1_VECTOR_4F getBackgroundColor() const {
		return Constants.backgroundColor;
	}
	inline HRESULT setBackgroundColor(D2D1_VECTOR_4F targetColor) {
		Constants.backgroundColor = targetColor;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getTargetColorLeftSide() const {
		return Constants.leftColor;
	}
	inline HRESULT setTargetColorLeftSide(D2D1_VECTOR_4F targetColor) {
		Constants.leftColor = targetColor;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getTargetColorRightSide() const {
		return Constants.rightColor;
	}
	inline HRESULT setTargetColorRightSide(D2D1_VECTOR_4F targetColor) {
		Constants.rightColor = targetColor;
		return S_OK;
	}
};

#endif //__DIRECT2D_MIST_WAVE_EFFECT__