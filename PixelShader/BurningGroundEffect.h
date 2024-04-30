#ifndef __BURNING_GROUND_EFFECT__
#define __BURNING_GROUND_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_BurningGroundEffect, 0x4b556d17, 0xf8ac, 0x4a7c, 0xa6, 0xe3, 0x21, 0x65, 0xba, 0x44, 0x74, 0x9);
DEFINE_GUID(CLSID_BurningGroundEffectPixelShader, 0xc89391a1, 0x42ef, 0x4cd4, 0x8b, 0xa6, 0xa5, 0xc8, 0x7d, 0x8, 0xfc, 0x8d);

enum BurningGroundEffectProperties {
	BURNING_GROUND_EFFECT_PROPERTY_FIRE_COLOR,
	BURNING_GROUND_EFFECT_PROPERTY_BACKGROUND_COLOR,
	BURNING_GROUND_EFFECT_PROPERTY_CANVAS_DIMENSIONS,
};

class BurningGroundEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_4F fireColor;
		D2D1_VECTOR_4F backgroundColor;
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
	} Constants;
protected:
	BurningGroundEffect();

	virtual const char* getShaderFileName() const {
		return "BurningGroundEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.timePassed = (uint32_t)clock.getDuration();
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~BurningGroundEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateBurningGroundEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_4F getFireColor() const {
		return Constants.fireColor;
	}
	inline HRESULT setFireColor(D2D1_VECTOR_4F color) {
		Constants.fireColor = color;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getBackgroundColor() const {
		return Constants.backgroundColor;
	}
	inline HRESULT setBackgroundColor(D2D1_VECTOR_4F color) {
		Constants.backgroundColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}

	inline HRESULT setDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		if (GetInputCount() <= 0) {
			setWidth((uint32_t)dimensions.x);
			setHeight((uint32_t)dimensions.y);
		}
		return S_OK;
	}
	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 0;
	}
};

#endif //__BURNING_GROUND_EFFECT__