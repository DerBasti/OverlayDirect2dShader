#ifndef __DIRECT2D_GLOWING_WHIRL_EFFECT__
#define __DIRECT2D_GLOWING_WHIRL_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_GlowingWhirlEffect, 0x351a96b1, 0x4b14, 0x4873, 0xa8, 0x46, 0xae, 0xc, 0x8f, 0x62, 0x92, 0x53);
DEFINE_GUID(CLSID_GlowingWhirlEffectPixelShader, 0x49ae1e22, 0xdd30, 0x49e6, 0x8b, 0x72, 0x1b, 0x1c, 0x72, 0x4c, 0xa9, 0x73);

enum GlowingWhirlEffectProperty {
	GLOWING_WHIRL_EFFECT_PROPERTY_COLOR_LEFT,
	GLOWING_WHIRL_EFFECT_PROPERTY_COLOR_RIGHT,
	GLOWING_WHIRL_EFFECT_PROPERTY_DIMENSIONS,
	GLOWING_WHIRL_EFFECT_PROPERTY_ANIMATION_SPEED,
	GLOWING_WHIRL_EFFECT_PROPERTY_ANIMATION_TYPE,
};

enum GlowingWhirlEffectAnimationSpeed {
	SLOWEST = 16,
	SLOW = 8,
	NORMAL = 4,
	FAST = 2,
	FASTEST = 1
};

enum GlowingWhirlEffectAnimationType {
	NEON_OUTLINE_STYLE = 0,
	SIMPLY_FILLED = 1,
	SIMPLY_FILLED_WITH_NEON_STYLE = 2,
};

class GlowingWhirlEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F leftsideColor;
		D2D1_VECTOR_4F rightsideColor;
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
		float animationSpeed;
		uint32_t animationType;
	} Constants;
	PerformanceClock timePassedClock;
protected:
	GlowingWhirlEffect();

	virtual const char* getShaderFileName() const {
		return "GlowingWhirlEffect";
	}

	virtual void updateConstantsBuffer() {
		if (Constants.dimensions.x == 0 && Constants.dimensions.y == 0) {
			Constants.dimensions.x = (float)getWidth();
			Constants.dimensions.y = (float)getHeight();
		}
		Constants.timePassed = static_cast<uint32_t>(timePassedClock.getDuration());
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		(*dataLength) = sizeof(Constants);
		return (BYTE*)&Constants;
	}

	virtual const GUID getEffectShaderGuid() const;
	virtual const GUID getEffectGuid() const;
public:
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateGlowingWhirlEffect(_Outptr_ IUnknown** ppEffectImpl);

	virtual ~GlowingWhirlEffect();

	inline D2D1_VECTOR_4F getLeftSideColor() const {
		return Constants.leftsideColor;
	}
	inline HRESULT setLeftSideColor(D2D1_VECTOR_4F color) {
		Constants.leftsideColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getRightSideColor() const {
		return Constants.rightsideColor;
	}
	inline HRESULT setRightSideColor(D2D1_VECTOR_4F color) {
		Constants.rightsideColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		return S_OK;
	}
	inline GlowingWhirlEffectAnimationSpeed getAnimationSpeed() const {
		return static_cast<GlowingWhirlEffectAnimationSpeed>(Constants.animationSpeed);
	}
	inline HRESULT setAnimationSpeed(GlowingWhirlEffectAnimationSpeed speed) {
		Constants.animationSpeed = static_cast<float>(speed);
		return S_OK;
	}
	inline GlowingWhirlEffectAnimationType getAnimationType() const {
		return static_cast<GlowingWhirlEffectAnimationType>(Constants.animationType);
	}
	inline HRESULT setAnimationType(GlowingWhirlEffectAnimationType animationType) {
		Constants.animationType = static_cast<uint32_t>(animationType);
		return S_OK;
	}
};



#endif