#ifndef __GLASS_SHATTERED_EFFECT__
#define __GLASS_SHATTERED_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_GlassShatteredEffect, 0x222518c2, 0xa214, 0x4158, 0x93, 0x93, 0xfc, 0x92, 0xc5, 0xdd, 0x17, 0x3c);
DEFINE_GUID(CLSID_GlassShatteredEffectPixelShader, 0xce0a78e, 0x9756, 0x4020, 0x82, 0xe3, 0x74, 0x69, 0x14, 0x50, 0xc3, 0x9e);

enum class GlassShatteredDarkSpots : uint8_t {
	NONE = 0,
	FEW = 10,
	SEVERAL = 15,
	LOT = 22
};

enum class GlassShatteredDistortion : uint8_t {
	LITTLE = 5,
	LESS_THAN_NORMAL = 7,
	NORMAL = 10,
	HIGH = 15,
	HIGHEST = 25
};

enum class GlassShatteredFilterType : uint8_t {
	SIMPLE = 0,
	COLOR_STRENGTHEND = 1
};

enum GlassShatteredEffectProperty : uint8_t {
	GLASS_SHATTERED_EFFECT_PROPERTY_DIMENSIONS,
	GLASS_SHATTERED_EFFECT_PROPERTY_OFFSET_FROM_CENTER,
	GLASS_SHATTERED_EFFECT_PROPERTY_DARK_SPOTS_AMOUNT,
	GLASS_SHATTERED_EFFECT_PROPERTY_DISTORTION,
	GLASS_SHATTERED_EFFECT_FILTER_TYPE
};

class GlassShatteredEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_2F dimensions;
		D2D1_VECTOR_2F center;
		float cellJitter;
		float glassDistortion;
		uint32_t filterType;
		uint32_t seed;
	} Constants;
	GlassShatteredEffect();

	virtual const char* getShaderFileName() const {
		return "GlassShatteredEffect";
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
	virtual ~GlassShatteredEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateGlassShatteredEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_2F getDimensions() const {
		return Constants.dimensions;
	}
	inline HRESULT setDimensions(D2D1_VECTOR_2F dims) {
		Constants.dimensions = dims;
		setWidth(dims.x);
		setHeight(dims.y);
		return S_OK;
	}
	inline D2D1_VECTOR_2F getCenter() const {
		return Constants.dimensions;
	}
	inline HRESULT setCenter(D2D1_VECTOR_2F center) {
		Constants.center = center;
		return S_OK;
	}
	inline GlassShatteredDarkSpots getGlassJitter() const {
		return static_cast<GlassShatteredDarkSpots>((Constants.cellJitter-1.0f) * 10);
	}
	inline HRESULT setGlassJitter(GlassShatteredDarkSpots cellJitter) {
		Constants.cellJitter = (static_cast<int32_t>(cellJitter) / 10.0f) + 1.0f;
		return S_OK;
	}
	inline GlassShatteredDistortion getGlassDistortion() const {
		return static_cast<GlassShatteredDistortion>(Constants.glassDistortion);
	}
	inline HRESULT setGlassDistortion(GlassShatteredDistortion glassDistortion) {
		Constants.glassDistortion = static_cast<float>(glassDistortion);
		return S_OK;
	}
	inline GlassShatteredFilterType getGlassFilterType() const {
		return static_cast<GlassShatteredFilterType>(Constants.filterType);
	}
	inline HRESULT setGlassFilterType(GlassShatteredFilterType filterType) {
		Constants.filterType = static_cast<uint32_t>(filterType);
		return S_OK;
	}
	inline uint32_t getSeed() const {
		return Constants.seed;
	}
	inline HRESULT setSeed(uint32_t seed) {
		Constants.seed = seed;
		return S_OK;
	}
};

#endif //__GLASS_SHATTERED_EFFECT__