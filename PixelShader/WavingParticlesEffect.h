#ifndef __WAVING_PARTICLES_EFFECT__
#define __WAVING_PARTICLES_EFFECT__

#pragma once

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_WavingParticlesEffect, 0x2918cd40, 0xdaf7, 0x4dbb, 0xa6, 0x9d, 0xc, 0xf9, 0x84, 0x49, 0xa2, 0x50);
DEFINE_GUID(CLSID_WavingParticlesEffectPixelShader, 0x214e9900, 0x390, 0x437e, 0xab, 0xb1, 0x1c, 0xe6, 0xa0, 0xbc, 0x4c, 0x4);

enum class WavingParticlesDistortion : uint8_t {
	LITTLE = 1,
	REGULAR = 4,
	STRONG = 6,
	VERY_STRONG = 8
};

enum class WavingParticlesSizeVariation : uint8_t {
	UNIFORM = 0,
	LITTLE = 10,
	REGULAR = 20,
	HIGH = 40,
};

enum class WavingParticlesBrightness : uint8_t {
	LITTLE = 15,
	REGULAR = 25,
	BRIGHT = 35,
	VERY_BRIGHT = 50
};

enum WavingParticlesProperty {
	WAVING_PARTICLES_PROPERTY_DIMENSIONS,
	WAVING_PARTICLES_PROPERTY_DISTORTION_STRENGTH,
	WAVING_PARTICLES_PROPERTY_PARTICLE_SIZE_VARIATION,
	WAVING_PARTICLES_PROPERTY_PARTICLE_BRIGHTNESS,
};

class WavingParticlesEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_2F dimensions;
		uint32_t timePassed;
		float distortionStrength;
		float particleSizeVariation;
		float particleBrightness;
	} Constants;
	WavingParticlesEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "WavingParticlesEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.dimensions.x = (float)getWidth();
		Constants.dimensions.y = (float)getHeight();
		Constants.timePassed = getTimePassed();
	}
	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~WavingParticlesEffect();

	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateWavingParticlesEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dimensions) {
		Constants.dimensions = dimensions;
		return S_OK;
	}
	inline WavingParticlesDistortion getDistortionStrength() const {
		return static_cast<WavingParticlesDistortion>(Constants.distortionStrength * 100);
	}
	inline HRESULT setDistortionStrength(WavingParticlesDistortion distortionStrength) {
		Constants.distortionStrength = (float)distortionStrength / 100.0f;
		return S_OK;
	}
	inline WavingParticlesSizeVariation getParticleSizeVariation() const {
		return static_cast<WavingParticlesSizeVariation>(Constants.particleSizeVariation * 100);
	}
	inline HRESULT setParticleSizeVariation(WavingParticlesSizeVariation particleSizeVariation) {
		Constants.particleSizeVariation = (float)particleSizeVariation / 100.0f;
		return S_OK;
	}
	inline WavingParticlesBrightness getParticleBrightness() const {
		return static_cast<WavingParticlesBrightness>(Constants.particleBrightness * 100.0f);
	}
	inline HRESULT setParticleBrightness(WavingParticlesBrightness particleBrightness) {
		Constants.particleBrightness = (float)particleBrightness / 100.0f;
		return S_OK;
	}
};

#endif //__WAVING_PARTICLES_EFFECT__