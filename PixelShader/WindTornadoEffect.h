#ifndef __WIND_TORNADO_EFFECT__
#define __WIND_TORNADO_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_WindTornadoEffect, 0x4b88be40, 0x27fe, 0x4180, 0xaa, 0xeb, 0xfc, 0x6c, 0xfc, 0x6f, 0xa1, 0x1b);
DEFINE_GUID(CLSID_WindTornadoEffectPixelShader, 0xefcd911b, 0x9656, 0x4ae8, 0x8a, 0x8a, 0xdb, 0xdd, 0x97, 0xae, 0x4, 0x14);

enum WindTornadoEffectProperties {
	WIND_TORNADO_EFFECT_PROPERTY_SMOKE_COLOR,
	WIND_TORNADO_EFFECT_PROPERTY_DIMENSIONS,
	WIND_TORNADO_EFFECT_PROPERTY_ANIMATION_SPEED,
};

class WindTornadoEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_4F smokeColor;
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
		float animationSpeed;
	} Constants;
protected:
	WindTornadoEffect();

	virtual const char* getShaderFileName() const {
		return "WindTornadoEffect";
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
	virtual ~WindTornadoEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateWindTornadoEffect(_Outptr_ IUnknown** ppEffectImpl);

	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}

	inline D2D1_VECTOR_4F getSmokeColor() const {
		return Constants.smokeColor;
	}
	inline HRESULT setSmokeColor(D2D1_VECTOR_4F color) {
		Constants.smokeColor = color;
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
	inline float getAnimationSpeed() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setAnimationSpeed(float animationSpeed) {
		Constants.animationSpeed = animationSpeed;
		return S_OK;
	}
};

#endif //__WIND_TORNADO_EFFECT__