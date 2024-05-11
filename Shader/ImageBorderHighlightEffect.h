#ifndef __DIRECT2D_IMAGE_BORDER_HIGHLIGHTER__
#define __DIRECT2D_IMAGE_BORDER_HIGHLIGHTER__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_ImageBorderHighlightEffect, 0xcd5da618, 0x67a0, 0x426b, 0xb5, 0x29, 0xb8, 0x15, 0x54, 0x60, 0xe3, 0xff);
DEFINE_GUID(CLSID_ImageBorderHighlightEffectPixelShader, 0x3869248b, 0x517e, 0x4078, 0xbe, 0xb4, 0x66, 0xe1, 0xe6, 0xdd, 0x5b, 0x9);

enum ImageBorderHighlighterProperties {
	IMAGE_BORDER_HIGHLIGHTER_BORDER_COLOR,
	IMAGE_BORDER_HIGHLIGHTER_BORDER_STRENGTH,
};

class ImageBorderHighlightEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F borderColor;
		int32_t borderStrength;
	} Constants;
protected:
	ImageBorderHighlightEffect();
	virtual const char* getShaderFileName() const {
		return "ImageBorderHighlighterShader";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const;
	virtual void updateConstantsBuffer();
	virtual const GUID getEffectShaderGuid() const;
	virtual const GUID getEffectGuid() const;
public:
	virtual ~ImageBorderHighlightEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateImageBorderHighlightEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_4F getBorderColor() const {
		return Constants.borderColor;
	}

	inline HRESULT setBorderColor(D2D1_VECTOR_4F newColor) {
		Constants.borderColor = newColor;
		return S_OK;
	}

	inline int32_t getBorderStrength() const {
		return Constants.borderStrength;
	}

	inline HRESULT setBorderStrength(uint32_t strength) {
		Constants.borderStrength = strength;
		return S_OK;
	}
};

#endif //__DIRECT2D_IMAGE_BORDER_HIGHLIGHTER__