#ifndef __DIRECT2D_FLYING_SQUARES_BACKGROUND_EFFECT__
#define __DIRECT2D_FLYING_SQUARES_BACKGROUND_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_FlyingSquaresBackgroundEffect, 0x5846094, 0x4989, 0x40e7, 0xb1, 0x29, 0x5, 0xa8, 0x6f, 0x81, 0x9b, 0x19);
DEFINE_GUID(CLSID_FlyingSquaresBackgroundEffectPixelShader, 0x718f8b2e, 0xe40b, 0x46c1, 0xba, 0x59, 0xec, 0x75, 0x41, 0xcc, 0x27, 0x9f);

enum FlyingSquaresBackgroundEffectProperty {
	FLYING_SQUARES_BACKGROUND_PROPERTY_BACKGROUND_SMOKE_COLOR,
	FLYING_SQUARES_BACKGROUND_PROPERTY_SQUARES_COLOR,
	FLYING_SQUARES_BACKGROUND_PROPERTY_CANVAS_DIMENSIONS,
	FLYING_SQUARES_BACKGROUND_PROPERTY_SQUARES_DISTRIBUTION_PERCENTAGE,
	FLYING_SQUARES_BACKGROUND_PROPERTY_SQUARES_AMOUNT,
};

class FlyingSquaresBackgroundEffect : public CustomLeagueBaseEffect {
private:
	PerformanceClock timePassedClock;
	struct {
		D2D1_VECTOR_4F smokeColor;
		D2D1_VECTOR_4F squaresColor;
		D2D1_VECTOR_2F dimensions;
		float distributionOverHeightPercentage;
		uint32_t squaresAmount;
		uint32_t timePassed;
	} Constants;
protected:
	FlyingSquaresBackgroundEffect();
	virtual const char* getShaderFileName() const {
		return "FlyingSquaresBackgroundEffect";
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

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateFlyingSquaresBackgroundEffect(_Outptr_ IUnknown** ppEffectImpl);

	virtual ~FlyingSquaresBackgroundEffect();

	inline D2D1_VECTOR_4F getSmokeColor() const {
		return Constants.smokeColor;
	}
	inline HRESULT setSmokeColor(D2D1_VECTOR_4F color) {
		Constants.smokeColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getSquaresColor() const {
		return Constants.squaresColor;
	}
	inline HRESULT setSquaresColor(D2D1_VECTOR_4F color) {
		Constants.squaresColor = color;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getCanvasDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setCanvasDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		return S_OK;
	}

	inline float getHeightDistributionPercentage() const {
		return Constants.distributionOverHeightPercentage;
	}
	inline HRESULT setHeightDistributionPercentage(float percentage) {
		Constants.distributionOverHeightPercentage = percentage;
		return S_OK;
	}

	inline uint32_t getAmountOfSquares() const {
		return Constants.squaresAmount;
	}
	inline HRESULT setAmountOfSquares(uint32_t amount) {
		Constants.squaresAmount = amount;
		return S_OK;
	}
};

#endif //__DIRECT2D_FLYING_SQUARES_BACKGROUND_EFFECT__