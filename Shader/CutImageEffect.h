#ifndef __CUT_IMAGE_EFFECT__
#define __CUT_IMAGE_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_CutImageEffect, 0x74c62d4b, 0x5630, 0x408e, 0xbb, 0x71, 0x9b, 0x70, 0xa4, 0xb5, 0x55, 0xd1);
DEFINE_GUID(CLSID_CutImageEffectPixelShader, 0x54d1f52d, 0x5731, 0x44ba, 0xbe, 0xcc, 0xca, 0x8b, 0x80, 0x54, 0xcf, 0xf2);

enum CutImageCutMode {
	CUT_IMAGE_EFFECT_MODE_BOTH,
	CUT_IMAGE_EFFECT_MODE_TOP_ONLY,
	CUT_IMAGE_EFFECT_MODE_BOTTOM_ONLY,
};

enum CutImageEffectProperty {
	CUT_IMAGE_EFFECT_PROPERTY_ANGLE,
	CUT_IMAGE_EFFECT_PROPERTY_CUT_WIDTH,
	CUT_IMAGE_EFFECT_PROPERTY_PROGRESS,
	CUT_IMAGE_EFFECT_PROPERTY_MAX_PROGRESS,
	CUT_IMAGE_EFFECT_PROPERTY_CUT_MODE,
};

class CutImageEffect : public CustomLeagueBaseEffect {
private:
	struct {
		float width;
		float height;
		float angle;
		float cutWidth;
		float progress;
		float maxProgress;
		CutImageCutMode cutMode;
	} Constants;
	CutImageEffect();

	virtual const char* getShaderFileName() const {
		return "CutImageEffect";
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
	virtual ~CutImageEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateCutImageEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline float getCutAngle() const {
		return Constants.angle;
	}
	inline HRESULT setCutAngle(float angle) {
		Constants.angle = (std::min)(angle, 90.0f);
		return S_OK;
	}
	inline float getCutWidth() const {
		return Constants.cutWidth;
	}
	inline HRESULT setCutWidth(float width) {
		Constants.cutWidth = width;
		return S_OK;
	}
	inline float getProgress() const {
		return Constants.progress;
	}
	inline HRESULT setProgress(float progress) {
		Constants.progress = (std::max)(0.0f, (std::min)(progress, getMaxProgress()*1.1f));
		return S_OK;
	}
	inline float getMaxProgress() const {
		return Constants.maxProgress;
	}
	inline HRESULT setMaxProgress(float maxProgress) {
		Constants.maxProgress = maxProgress;
		return S_OK;
	}
	inline CutImageCutMode getCutMode() const {
		return Constants.cutMode;
	}
	inline HRESULT setCutMode(CutImageCutMode mode) {
		Constants.cutMode = mode;
		return S_OK;
	}
};

#endif
