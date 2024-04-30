#ifndef __DIRECT2D_GRADIENT_OVERDRAW_EFFECT__
#define __DIRECT2D_GRADIENT_OVERDRAW_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_GradientOverdrawEffect, 0x12af5d20, 0x3943, 0x4aa3, 0x8e, 0x8e, 0xef, 0xc7, 0x87, 0xfe, 0x1b, 0x4b);
DEFINE_GUID(CLSID_GradientOverdrawEffectPixelShader, 0x7f6bbc7d, 0x5d07, 0x431f, 0xb2, 0x83, 0x4f, 0x70, 0xeb, 0xe9, 0xd7, 0xa0);

enum GradientOverdrawEffectProperty {
	GRADIENT_OVERDRAW_EFFECT_PROPERTY_COLOR_START,
	GRADIENT_OVERDRAW_EFFECT_PROPERTY_COLOR_END,
	GRADIENT_OVERDRAW_EFFECT_PROPERTY_COLOR_POSITION_PERCENTAGES,
};

class GradientOverdrawEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F startColor;
		D2D1_VECTOR_4F endColor;
		D2D1_VECTOR_2F positionPercentages;
		D2D1_VECTOR_2F dimensions;
	} Constants;
protected:
	GradientOverdrawEffect();
	virtual const char* getShaderFileName() const {
		return "GradientOverdrawEffect";
	}

	virtual void updateConstantsBuffer() {
		if (Constants.dimensions.x == 0 && Constants.dimensions.y == 0) {
			Constants.dimensions.x = (float)getWidth();
			Constants.dimensions.y = (float)getHeight();
		}
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		(*dataLength) = sizeof(Constants);
		return (BYTE*)&Constants;
	}

	virtual const GUID getEffectShaderGuid() const;
	virtual const GUID getEffectGuid() const;
public:
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateGradientOverdrawEffect(_Outptr_ IUnknown** ppEffectImpl);

	virtual ~GradientOverdrawEffect();

	inline D2D1_VECTOR_4F getStartColor() const {
		return Constants.startColor;
	}
	inline HRESULT setStartColor(D2D1_VECTOR_4F color) {
		Constants.startColor = color;
		return S_OK;
	}
	
	inline D2D1_VECTOR_4F getEndColor() const {
		return Constants.endColor;
	}
	inline HRESULT setEndColor(D2D1_VECTOR_4F color) {
		Constants.endColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getPositionPercentages() const {
		return Constants.positionPercentages;
	}
	inline HRESULT setPositionPercentages(D2D1_VECTOR_2F percentages) {
		Constants.positionPercentages = percentages;
		return S_OK;
	}
};



#endif