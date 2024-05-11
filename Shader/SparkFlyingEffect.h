#ifndef __SPARK_FLYING_EFFECT__
#define __SPARK_FLYING_EFFECT__

#include "CustomLeagueEffect.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_SparkFlyingEffect, 0x1e1db835, 0x41a0, 0x4cbd, 0x8f, 0xeb, 0x20, 0xc3, 0xe2, 0x68, 0x9, 0x78);
DEFINE_GUID(CLSID_SparkFlyingEffectPixelShader, 0x94356913, 0xb209, 0x4d08, 0xab, 0x61, 0xa8, 0xac, 0xe7, 0x39, 0x8, 0xde);

enum SparkFlyingEffectBloomIntensity {
	SPARK_FLYING_EFFECT_BLOOM_INTENSITY_LOW,
	SPARK_FLYING_EFFECT_BLOOM_INTENSITY_MEDIUM,
	SPARK_FLYING_EFFECT_BLOOM_INTENSITY_HIGH,
};

enum SparkFlyingEffectParticleAmount {
	FEW = 2,
	SEVERAL = 5,
	MANY = 10
};

enum SparkFlyingEffectProperty {
	SPARK_FLYING_EFFECT_PROPERTY_SPARK_COLOR,
	SPARK_FLYING_EFFECT_PROPERTY_SMOKE_COLOR,
	SPARK_FLYING_EFFECT_PROPERTY_BLOOM_INTENSITY,
	SPARK_FLYING_EFFECT_PROPERTY_ANIMATION_SPEED,
	SPARK_FLYING_EFFECT_PROPERTY_PARTICLE_SIZE_IN_CANVASPERCENTAGE,
	SPARK_FLYING_EFFECT_PROPERTY_PARTICLE_DIRECTION_VECTOR,
	SPARK_FLYING_EFFECT_PROPERTY_PARTICLE_AMOUNT,
	SPARK_FLYING_EFFECT_PROPERTY_DIMENSIONS,
};

class SparkFlyingEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F sparkColor;
		D2D1_VECTOR_4F smokeColor;
		float width;
		float height;
		SparkFlyingEffectBloomIntensity bloomIntensity;
		uint32_t timePassed;
		float animationSpeed;
		float sizeOfParticleInPercentOfCanvas;
		D2D1_VECTOR_2F particleDirectionVector;
		SparkFlyingEffectParticleAmount particleAmount;
	} Constants;
	SparkFlyingEffect();
	PerformanceClock clock;

	virtual const char* getShaderFileName() const {
		return "SparkFlyingEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.width = (float)getWidth();
		Constants.height = (float)getHeight();
		Constants.timePassed = (uint32_t)clock.getDuration();
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~SparkFlyingEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateSparkFlyingEffect(_Outptr_ IUnknown** ppEffectImpl);
	
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 0;
	}
	
	inline D2D1_VECTOR_4F getSparkColor() const {
		return Constants.sparkColor;
	}
	inline HRESULT setSparkColor(D2D1_VECTOR_4F sparkColor) {
		Constants.sparkColor = sparkColor;
		return S_OK;
	}

	inline D2D1_VECTOR_4F getSmokeColor() const {
		return Constants.smokeColor;
	}
	inline HRESULT setSmokeColor(D2D1_VECTOR_4F color) {
		Constants.smokeColor = color;
		return S_OK;
	}

	inline SparkFlyingEffectBloomIntensity getBloomIntensityType() const {
		return Constants.bloomIntensity;
	}
	inline HRESULT setBloomIntensityType(SparkFlyingEffectBloomIntensity type) {
		Constants.bloomIntensity = type;
		return S_OK;
	}

	inline float getAnimationSpeed() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setAnimationSpeed(float animationSpeed) {
		Constants.animationSpeed = animationSpeed;
		return S_OK;
	}
	inline float getParticleSize() const {
		return Constants.sizeOfParticleInPercentOfCanvas;
	}
	inline HRESULT setParticleSize(float size) {
		Constants.sizeOfParticleInPercentOfCanvas = size;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getParticleDirectionVector() const {
		return Constants.particleDirectionVector;
	}
	inline HRESULT setParticleDirectionVector(D2D1_VECTOR_2F dir) {
		Constants.particleDirectionVector = dir;
		return S_OK;
	}
	inline SparkFlyingEffectParticleAmount getParticleAmount() const {
		return Constants.particleAmount;
	}
	inline HRESULT setParticleAmount(SparkFlyingEffectParticleAmount amount) {
		Constants.particleAmount = amount;
		return S_OK;
	}

	inline float getDimensions() const {
		return Constants.animationSpeed;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dims) {
		if (getWidth() == 0 && getHeight() == 0 && GetInputCount()==0) {
			setWidth((uint32_t)dims.x);
			setHeight((uint32_t)dims.y);
		}
		return S_OK;
	}
};

#endif
