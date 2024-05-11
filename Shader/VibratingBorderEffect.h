#ifndef __VIBRATING_BORDER_EFFECT__
#define __VIBRATING_BORDER_EFFECT__

#pragma once

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_VibratingBorderEffect, 0x14a5fc10, 0x834, 0x4a52, 0xa1, 0xd5, 0x85, 0xc0, 0xcc, 0xa6, 0x8a, 0xb8);
DEFINE_GUID(CLSID_VibratingBorderEffectPixelShader, 0xac9fd29, 0x55e9, 0x4b6b, 0xa1, 0xbd, 0x97, 0x21, 0xe1, 0xc4, 0x40, 0xc);

enum VibratingBorderEffectProperties {
	VIBRATING_BORDER_EFFECT_PROPERTY_COLOR,
	VIBRATING_BORDER_EFFECT_PROPERTY_DIMENSIONS,
	VIBRATING_BORDER_EFFECT_PROPERTY_ANIMATION_SPEED,
	VIBRATING_BORDER_EFFECT_PROPERTY_SEED,
};

class VibratingBorderEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_4F borderColor;
		D2D1_VECTOR_2F dimensions;
		float animationSpeed;
		uint32_t timePassed;
	} Constants;
	uint32_t seed;
protected:
	VibratingBorderEffect();

	virtual const char* getShaderFileName() const {
		return "VibratingBorderEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
 		Constants.timePassed = (uint32_t)clock.getDuration() + seed;
	}
	virtual RECT getOutputRect(const RECT& otherRect) const {
		return RECT{ otherRect.left, otherRect.top, otherRect.left + (LONG)Constants.dimensions.x, otherRect.top + (LONG)Constants.dimensions.y };
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~VibratingBorderEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateVibratingBorderEffect(_Outptr_ IUnknown** ppEffectImpl);

	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 2;
	}

	inline D2D1_VECTOR_4F getBorderColor() const {
		return Constants.borderColor;
	}

	inline HRESULT setBorderColor(D2D1_VECTOR_4F color) {
		Constants.borderColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}

	inline HRESULT setDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		return S_OK;
	}

	inline float getAnimationSpeed() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setAnimationSpeed(float animationSpeed) {
		Constants.animationSpeed = animationSpeed;
		return S_OK;
	}

	inline uint32_t getSeed() const {
		return seed;
	}
	inline HRESULT setSeed(uint32_t seed) {
		this->seed = seed;
		return S_OK;
	}
};

#endif //__UNDERWATER_EFFECT__-