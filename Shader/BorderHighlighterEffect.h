#ifndef __DIRECT2D_BORDER_HIGHLIGHTER_EFFECT__
#define __DIRECT2D_BORDER_HIGHLIGHTER_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_BorderHighlighterEffect, 0x2c5d59fb, 0xf122, 0x4958, 0x8c, 0x86, 0xb9, 0x49, 0xd3, 0xd7, 0x29, 0xe8);
DEFINE_GUID(CLSID_BorderHighlighterEffectPixelShader, 0xff4503ac, 0xd2e7, 0x4325, 0x99, 0x64, 0x72, 0xe5, 0xdf, 0x69, 0x1f, 0x11);

enum BorderHighlighterEffectProperty {
	BORDER_HIGHLIGHT_EFFECT_BORDER_STRENGTH_PERCENTAGE,
	BORDER_HIGHLIGHT_EFFECT_BORDER_COLOR
};

class BorderHighlighterEffect : public CustomLeagueBaseEffect {
private:
	struct {
		float width;
		float height;
		float borderStrengthPercentage;
		D2D1_VECTOR_4F highlighterColor;
	} Constants;
	BorderHighlighterEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "BorderHighlighterEffect";
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
	virtual ~BorderHighlighterEffect() {

	}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateBorderHighlighterEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline float getBorderStrengthPercentage() const {
		return Constants.borderStrengthPercentage;
	}
	inline HRESULT setBorderStrengthPercentage(float percentage) {
		Constants.borderStrengthPercentage = percentage;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getHighlightColor() const {
		return Constants.highlighterColor;
	}
	inline HRESULT setHighlightColor(D2D1_VECTOR_4F timePassedNew) {
		Constants.highlighterColor = timePassedNew;
		return S_OK;
	}
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}
};

#endif //__DIRECT2D_BORDER_HIGHLIGHTER_EFFECT__