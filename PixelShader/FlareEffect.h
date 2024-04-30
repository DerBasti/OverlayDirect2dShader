#ifndef __FLARE_EFFECT_SHADER__
#define __FLARE_EFFECT_SHADER__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"
#include "../GUI/Direct2DFactory.h"

DEFINE_GUID(CLSID_FlareEffect, 0xb84690e5, 0xe61e, 0x4b49, 0x90, 0xb6, 0xcf, 0x2d, 0xfd, 0x1f, 0xa5, 0x3d);
DEFINE_GUID(CLSID_FlareEffectPixelShader, 0x24ca6b76, 0x174c, 0x4d60, 0x81, 0xea, 0x8c, 0x7a, 0xd9, 0xe3, 0x98, 0x7b);

enum FlareEffectDirection {
	TOP_TO_BOTTOM,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP,
	LEFT_TO_RIGHT,
};

enum FlareEffectProperties {
	FLARE_EFFECT_PROPERTY_FLARE_OUTER_COLOR,
	FLARE_EFFECT_PROPERTY_FLARE_INNER_COLOR,
	FLARE_EFFECT_PROPERTY_FLARE_DIMENSIONS,
	FLARE_EFFECT_PROPERTY_FLARE_NOISE_TEXTURE_DIMENSIONS,
	FLARE_EFFECT_PROPERTY_FLARE_HEIGHT,
	FLARE_EFFECT_PROPERTY_FLARE_DIRECTION,
};

class FlareEffect : public CustomLeagueBaseEffect {
private:
	FlareEffect();	
	struct {
		D2D1_VECTOR_4F outerColor;
		D2D1_VECTOR_4F innerColor;
		D2D1_VECTOR_2F dimensions;
		D2D1_VECTOR_2F noiseTextureDimensions;
		uint32_t timePassedInMillis;
		D2D1_VECTOR_3F flareHeight;
		FlareEffectDirection direction;
		float animationSpeed;
	} Constants;
	PerformanceClock clock;

protected:
	virtual const char* getShaderFileName() const {
		return "FlareEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.timePassedInMillis = (uint32_t)clock.getDuration();
	}
	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
	virtual bool needsInternalEffects() const;
	virtual void onInitInternalEffects(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph);

	virtual void onHandleAdditionalInputRects(UINT32 inputRectCount, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects) const {
		pInputRects[1] = D2D1::RectL(0, 0, Constants.noiseTextureDimensions.x, Constants.noiseTextureDimensions.y);
	}
public:
	virtual ~FlareEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateFlareEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_4F getFlareOuterColor() const {
		return Constants.outerColor;
	}
	inline HRESULT setFlareOuterColor(D2D1_VECTOR_4F flareColor) {
		Constants.outerColor = flareColor;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getFlareInnerColor() const {
		return Constants.innerColor;
	}
	inline HRESULT setFlareInnerColor(D2D1_VECTOR_4F flareColor) {
		Constants.innerColor = flareColor;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dims) {
		Constants.dimensions = dims;
		return S_OK;
	}
	inline D2D1_VECTOR_2F getNoiseTextureDimensions() const {
		return Constants.noiseTextureDimensions;
	}
	inline HRESULT setNoiseTextureDimensions(D2D1_VECTOR_2F dims) {
		Constants.noiseTextureDimensions = dims;
		return S_OK;
	}
	inline D2D1_VECTOR_3F getFlareHeight() const {
		return Constants.flareHeight;
	}
	inline HRESULT setFlareHeight(D2D1_VECTOR_3F flareHeight) {
		Constants.flareHeight = flareHeight;
		return S_OK;
	}
	inline FlareEffectDirection getFlareDirection() const {
		return Constants.direction;
	}
	inline HRESULT setFlareDirection(FlareEffectDirection direction) {
		Constants.direction = direction;
		return S_OK;
	}
	virtual IFACEMETHODIMP_(UINT32) GetInputCount() const;
};

#endif //__FLARE_EFFECT_SHADER__