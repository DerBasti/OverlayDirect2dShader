#ifndef __SMOKE_TORNADO_EFFECT__
#define __SMOKE_TORNADO_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_SmokeTornadoEffect, 0x83fdfb22, 0x8c9e, 0x41ba, 0xbf, 0x44, 0x38, 0x7a, 0xd7, 0xb6, 0x5d, 0xa6);
DEFINE_GUID(CLSID_SmokeTornadoEffectPixelShader, 0x76e85765, 0xadf1, 0x4418, 0x96, 0x74, 0x2c, 0xf7, 0xe8, 0x4f, 0x5e, 0x52);

enum SmokeTornadoEffectProperties {
	SMOKE_TORNADO_EFFECT_PROPERTY_SMOKE_COLOR,
	SMOKE_TORNADO_EFFECT_PROPERTY_CANVAS_OFFSET,
	SMOKE_TORNADO_EFFECT_PROPERTY_DIMENSIONS,
	SMOKE_TORNADO_EFFECT_PROPERTY_ANIMATION_SPEED,
};

class SmokeTornadoEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock clock;
	struct {
		D2D1_VECTOR_4F smokeColor;
		D2D1_VECTOR_2F canvasOffset;
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
		float animationSpeed;
	} Constants;
protected:
	SmokeTornadoEffect();

	virtual const char* getShaderFileName() const {
		return "SmokeTornadoEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.timePassed = (uint32_t)clock.getDuration();
	}
	inline virtual uint32_t getX() const {
		return (uint32_t)Constants.canvasOffset.x;
	}
	inline virtual uint32_t getY() const {
		return (uint32_t)Constants.canvasOffset.y;
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~SmokeTornadoEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateSmokeTornadoEffect(_Outptr_ IUnknown** ppEffectImpl);

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
	inline D2D1_VECTOR_2F getCanvasOffset() const {
		return Constants.canvasOffset;
	}
	inline HRESULT setCanvasOffset(D2D1_VECTOR_2F dimensions) {
		Constants.canvasOffset = dimensions;
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

#endif //__SMOKE_TORNADO_EFFECT__