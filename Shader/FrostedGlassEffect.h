#ifndef __FROSTED_GLASS_EFFECT__
#define __FROSTED_GLASS_EFFECT__

#pragma once

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_FrostedGlassEffect, 0xcab49d51, 0x6427, 0x4dce, 0xb2, 0xb5, 0xf0, 0x78, 0x59, 0xc2, 0xb3, 0x83);
DEFINE_GUID(CLSID_FrostedGlassEffectPixelShader, 0xb18e65a0, 0x4e7c, 0x4074, 0xa0, 0x6f, 0x32, 0x8a, 0xd0, 0x79, 0x5f, 0x6c);

enum class FrostedGlassEffectStrength : uint16_t {
	VERY_LITTLE = 5,
	SLIGHTLY = 7,
	REGULAR = 10,
	STRONG = 20,
	STRONGEST = 30,
};

enum class FrostedGlassEffectFinesse : uint16_t {
	VERY_ROUGH = 1,
	SLIGHTLY_ROUGH = 5,
	FINE = 10,
	FINEST = 25,
};

enum class FrostedGlassEffectProperties {
	FROSTED_GLASS_PROPERTY_DIMENSIONS,
	FROSTED_GLASS_PROPERTY_EFFECT_STRENGTH,
	FROSTED_GLASS_PROPERTY_EFFECT_FINESSE
};

class FrostedGlassEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_2F dimensions;
		float effectStrength;
		float effectFinesse;
	} Constants;
protected:
	FrostedGlassEffect();

	virtual const char* getShaderFileName() const {
		return "FrostedGlassEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.dimensions.x = (float)getWidth();
		Constants.dimensions.y = (float)getHeight();
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~FrostedGlassEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateFrostedGlassEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F color) {
		Constants.dimensions = color;
		return S_OK;
	}
	inline FrostedGlassEffectStrength getEffectStrength() const {
		return static_cast<FrostedGlassEffectStrength>(Constants.effectStrength * 1000.0f);
	}
	inline HRESULT setEffectStrength(FrostedGlassEffectStrength strength) {
		Constants.effectStrength = float(strength) / 1000.0f;
		return S_OK;
	}
	inline FrostedGlassEffectFinesse getEffectFinesse() const {
		return static_cast<FrostedGlassEffectFinesse>(Constants.effectFinesse);
	}
	inline HRESULT setEffectFinesse(FrostedGlassEffectFinesse strength) {
		Constants.effectFinesse = float(strength);
		return S_OK;
	}
};

#endif //__FROSTED_GLASS_EFFECT__