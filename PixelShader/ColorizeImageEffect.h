#ifndef __DIRECT2D_COLORIZE_IMAGE_EFFECT__
#define __DIRECT2D_COLORIZE_IMAGE_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_ColorizeImageEffect, 0xb0e78ada, 0xca8f, 0x4164, 0x8b, 0x22, 0x93, 0xa7, 0xd0, 0x63, 0x43, 0x5c);
DEFINE_GUID(CLSID_ColorizeImageEffectPixelShader, 0x763ba2ed, 0xece6, 0x42ab, 0x8c, 0x97, 0xa8, 0x94, 0x6f, 0xeb, 0xbc, 0x97);

enum ColorizeImageEffectProperties {
	COLORIZE_IMAGE_EFFECT_PROPERTIES_COLOR
};

class ColorizeImageEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F color;
	} Constants;
	ColorizeImageEffect();

	virtual const char* getShaderFileName() const {
		return "ColorizeImageEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {

	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~ColorizeImageEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateColorizeImageEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_4F getTargetColorization() const {
		return Constants.color;
	}
	inline HRESULT setTargetColorization(D2D1_VECTOR_4F color) {
		Constants.color = color;
		return S_OK;
	}
};

#endif //__DIRECT2D_COLORIZE_IMAGE_EFFECT__