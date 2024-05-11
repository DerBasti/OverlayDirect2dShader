#ifndef __NOISE_TEXTURE_EFFECT__
#define __NOISE_TEXTURE_EFFECT__

#include "../GUI/Direct2DFactory.h"

enum NoiseTextureType {
	NOISE_TEXTURE_ROUNDED,
	NOISE_TEXTURE_CLOUDY,
};

enum NoiseTextureDetails {
	NOISE_TEXTURE_DETAILS_LOWEST = 2,
	NOISE_TEXTURE_DETAILS_LOW = 4,
	NOISE_TEXTURE_DETAILS_MEDIUM = 6,
	NOISE_TEXTURE_DETAILS_HIGH = 12,
	NOISE_TEXTURE_DETAILS_HIGHEST = 32,
};

class NoiseTextureConfig {
private:
	NoiseTextureType type;
	NoiseTextureDetails detailLevel;
public:
	NoiseTextureConfig() : NoiseTextureConfig(NoiseTextureType::NOISE_TEXTURE_ROUNDED, NoiseTextureDetails::NOISE_TEXTURE_DETAILS_MEDIUM) {}
	NoiseTextureConfig(NoiseTextureType type, NoiseTextureDetails detailLevel) {
		this->type = type;
		this->detailLevel = detailLevel;
	}
	virtual ~NoiseTextureConfig() {

	}
	inline NoiseTextureType getTextureType() const {
		return type;
	}
	inline NoiseTextureDetails getDetailLevel() const {
		return detailLevel;
	}
};

class NoiseTextureEffect {
private:
	NoiseTextureEffect() {

	}
public:
	static Microsoft::WRL::ComPtr<ID2D1Image> CreateNoiseTexture(Microsoft::WRL::ComPtr<ID2D1RenderTarget> target, RECT dimensions, NoiseTextureConfig type = NoiseTextureConfig());
};

#endif //__NOISE_TEXTURE_EFFECT__