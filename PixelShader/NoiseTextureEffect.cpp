#include "NoiseTextureEffect.h"


Microsoft::WRL::ComPtr<ID2D1Image> NoiseTextureEffect::CreateNoiseTexture(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderer, RECT dimensions, NoiseTextureConfig textureConfig) {
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> ctx;
	renderer.As(&ctx);

	Microsoft::WRL::ComPtr<ID2D1Effect> turbulenceEffect;

	Microsoft::WRL::ComPtr<ID2D1Effect> highlightsAndShadowsEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect> exposureEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect> revert;
	Microsoft::WRL::ComPtr<ID2D1Effect> grayscale;
	ctx->CreateEffect(CLSID_D2D1Turbulence, &turbulenceEffect);

	turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_NUM_OCTAVES, textureConfig.getDetailLevel());
	turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_SEED, (uint32_t)time(nullptr));
	turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_NOISE, textureConfig.getTextureType() == NoiseTextureType::NOISE_TEXTURE_CLOUDY ? D2D1_TURBULENCE_NOISE_TURBULENCE : D2D1_TURBULENCE_NOISE_FRACTAL_SUM);
	turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_STITCHABLE, TRUE);
	turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_SIZE, D2D1::Vector2F(dimensions.right - dimensions.left, dimensions.bottom - dimensions.top));

	ctx->CreateEffect(CLSID_D2D1HighlightsShadows, &highlightsAndShadowsEffect);

	highlightsAndShadowsEffect->SetValue(D2D1_HIGHLIGHTSANDSHADOWS_PROP_HIGHLIGHTS, 1.0f);
	highlightsAndShadowsEffect->SetValue(D2D1_HIGHLIGHTSANDSHADOWS_PROP_SHADOWS, 0.2f);
	highlightsAndShadowsEffect->SetValue(D2D1_HIGHLIGHTSANDSHADOWS_PROP_CLARITY, 1.0f);
	highlightsAndShadowsEffect->SetValue(D2D1_HIGHLIGHTSANDSHADOWS_PROP_INPUT_GAMMA, D2D1_HIGHLIGHTSANDSHADOWS_INPUT_GAMMA_SRGB);
	highlightsAndShadowsEffect->SetValue(D2D1_HIGHLIGHTSANDSHADOWS_PROP_MASK_BLUR_RADIUS, 0.0f);

	ctx->CreateEffect(CLSID_D2D1Exposure, &exposureEffect);
	ctx->CreateEffect(CLSID_D2D1Invert, &revert);
	ctx->CreateEffect(CLSID_D2D1Grayscale, &grayscale);

	highlightsAndShadowsEffect->SetInputEffect(0, turbulenceEffect.Get());
	exposureEffect->SetInputEffect(0, highlightsAndShadowsEffect.Get());
	revert->SetInputEffect(0, exposureEffect.Get());
	grayscale->SetInputEffect(0, revert.Get());

	Microsoft::WRL::ComPtr<ID2D1Image> image;
	grayscale->GetOutput(image.GetAddressOf());
	return image;
}