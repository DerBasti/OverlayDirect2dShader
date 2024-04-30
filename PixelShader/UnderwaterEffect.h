#ifndef __UNDERWATER_EFFECT__
#define __UNDERWATER_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_UnderwaterEffect, 0x68ef620c, 0x7b, 0x421c, 0x87, 0x7d, 0x20, 0x7c, 0xc5, 0xb9, 0x57, 0x2b);
DEFINE_GUID(CLSID_UnderwaterEffectPixelShader, 0xad35eb46, 0xa206, 0x4c5b, 0xbb, 0xad, 0xd0, 0x12, 0x8e, 0xab, 0x84, 0x59);

enum UnderwaterEffectProperties {
	UNDERWATER_EFFECT_PROPERTY_SEED,
	UNDERWATER_EFFECT_PROPERTY_DIMENSIONS,
	UNDERWATER_EFFECT_PROPERTY_ANIMATION_SPEED,
};

class UnderwaterEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
		float animationSpeed;
	} Constants;
	uint32_t seed;
protected:
	UnderwaterEffect();

	virtual const char* getShaderFileName() const {
		return "UnderwaterEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.timePassed = (uint32_t)clock.getDuration() + seed;
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~UnderwaterEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateUnderwaterEffect(_Outptr_ IUnknown** ppEffectImpl);

	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}

	inline uint32_t getSeed() const {
		return seed;
	}
	inline HRESULT setSeed(uint32_t seed) {
		this->seed = seed;
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

#endif //__UNDERWATER_EFFECT__-