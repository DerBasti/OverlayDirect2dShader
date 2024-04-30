#include "WavingParticlesEffect.h"

extern "C" {
	const GUID CLSID_WavingParticlesEffect = { 0x2918cd40, 0xdaf7, 0x4dbb, { 0xa6, 0x9d, 0xc, 0xf9, 0x84, 0x49, 0xa2, 0x50 } };
	const GUID CLSID_WavingParticlesEffectPixelShader = { 0x214e9900, 0x390, 0x437e, { 0xab, 0xb1, 0x1c, 0xe6, 0xa0, 0xbc, 0x4c, 0x4 } };
};

WavingParticlesEffect::WavingParticlesEffect() {
	Constants.timePassed = 0;
	setDimensions(D2D1::Vector2F(1.0f, 1.0f));
	setParticleBrightness(WavingParticlesBrightness::BRIGHT);
	setParticleSizeVariation(WavingParticlesSizeVariation::REGULAR);
	setDistortionStrength(WavingParticlesDistortion::REGULAR);
}
WavingParticlesEffect::~WavingParticlesEffect() {

}

const GUID WavingParticlesEffect::getEffectGuid() const {
	return CLSID_WavingParticlesEffect;
}
const GUID WavingParticlesEffect::getEffectShaderGuid() const {
	return CLSID_WavingParticlesEffectPixelShader;
}

HRESULT WavingParticlesEffect::Register(_In_ ID2D1Factory1* pFactory) {
	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
			<Property name='DisplayName' type='string' value='Wave Effect'/>
			<Property name='Author' type='string' value='Dieser Basti'/>
			<Property name='Category' type='string' value='Rect Pixelshader'/>
			<Property name='Description' type='string' value='Prettification of a simple rectangle'/>
			<Inputs>
			<Input name='Source'/>
			</Inputs>
			<Property name='Dimensions' type='vector2'>
			<Property name='DisplayName' type='string' value='Dimensions'/>
			<Property name='Default' type='vector2' value='(1.0, 1.0)'/>
			</Property>
			<Property name='DistortionStrength' type='int32'>
			<Property name='DisplayName' type='string' value='DistortionStrength'/>
			<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='SizeVariation' type='int32'>
			<Property name='DisplayName' type='string' value='SizeVariation'/>
			<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='ParticleBrightness' type='int32'>
			<Property name='DisplayName' type='string' value='ParticleBrightness'/>
			<Property name='Default' type='int32' value='0'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"DistortionStrength",  &setDistortionStrength,  &getDistortionStrength),
		D2D1_VALUE_TYPE_BINDING(L"SizeVariation",  &setParticleSizeVariation,  &getParticleSizeVariation),
		D2D1_VALUE_TYPE_BINDING(L"ParticleBrightness",  &setParticleBrightness,  &getParticleBrightness),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_WavingParticlesEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateWavingParticlesEffect
	);
	return hr;
}
HRESULT __stdcall WavingParticlesEffect::CreateWavingParticlesEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new WavingParticlesEffect();
	return S_OK;
}