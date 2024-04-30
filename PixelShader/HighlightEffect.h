#ifndef __DIRECT2D_HIGHLIGHT_EFFECT__
#define __DIRECT2D_HIGHLIGHT_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_HighlightEffect, 0xd71bf361, 0xd108, 0x4f8a, 0xb4, 0x83, 0xea, 0x55, 0xc0, 0x2a, 0xff, 0x5);
DEFINE_GUID(CLSID_HighlightEffectPixelShader, 0x9ce83fc7, 0x51e2, 0x429c, 0xab, 0xe3, 0x3f, 0xbf, 0x7d, 0xac, 0x27, 0x93);

enum HighlightEffectProperty {
	HIGHLIGHT_EFFECT_PROPERTY_WIDTH,
	HIGHLIGHT_EFFECT_PROPERTY_HEIGHT,
	HIGHLIGHT_EFFECT_PROPERTY_ANGLE,
	HIGHLIGHT_EFFECT_PROPERTY_PROGRESS,
};

class HighlightEffect : public CustomLeagueBaseEffect {
private:
	struct Constants {
		float width;
		float height;
		float angle;
		float progress;
	};

	Constants *constants;

	HighlightEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "HighlightEffect";
	}

	virtual void updateConstantsBuffer() {
		constants->width = (float)getWidth();
		constants->height = (float)getHeight();
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		(*dataLength) = sizeof(*constants);
		return (BYTE*)constants;
	}

	virtual const GUID getEffectShaderGuid() const {
		return CLSID_HighlightEffectPixelShader;
	}
	virtual const GUID getEffectGuid() const {
		return CLSID_HighlightEffect;
	}
public:
	virtual ~HighlightEffect() {
		delete constants;
		constants = nullptr;
	}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateHighlightEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline float getEmpty() const {
		return 0.0f;
	}
	inline HRESULT setEmpty(float value) {
		return S_OK;
	}
	inline float getProgress() const {
		return constants->progress;
	}
	inline HRESULT setProgress(float progress) {
		constants->progress = progress;
		return S_OK;
	}
	inline float getAngle() const {
		return constants->angle;
	}
	inline HRESULT setAngle(float angle) {
		constants->angle = angle;
		return S_OK;
	}
};

#endif //__DIRECT2D_HIGHLIGHT_EFFECT__