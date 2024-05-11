#include "SparkFlyingEffect.h"

#include "CutImageEffect.h"

extern "C" {
	const GUID CLSID_SparkFlyingEffect = { 0x1e1db835, 0x41a0, 0x4cbd, { 0x8f, 0xeb, 0x20, 0xc3, 0xe2, 0x68, 0x9, 0x78 } };
	const GUID CLSID_SparkFlyingEffectPixelShader = { 0x94356913, 0xb209, 0x4d08, { 0xab, 0x61, 0xa8, 0xac, 0xe7, 0x39, 0x8, 0xde } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT SparkFlyingEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='SparkColor' type='vector4'>
				<Property name='DisplayName' type='string' value='SparkColor'/>
				<Property name='Default' type='vector4' value='(1.0,0.0,0.0,1.0)'/>
			</Property>
			<Property name='SmokeColor' type='vector4'>
				<Property name='DisplayName' type='string' value='SmokeColor'/>
				<Property name='Default' type='vector4' value='(1.0,0.0,0.0,1.0)'/>
			</Property>
			<Property name='BloomIntensityType' type='int32'>
				<Property name='DisplayName' type='string' value='BloomIntensityType'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='AnimationSpeed' type='float'>
				<Property name='DisplayName' type='string' value='AnimationSpeed'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='ParticleSize' type='float'>
				<Property name='DisplayName' type='string' value='ParticleSize'/>
				<Property name='Default' type='float' value='0.01'/>
			</Property>
			<Property name='ParticleDirectionVector' type='vector2'>
				<Property name='DisplayName' type='string' value='ParticleDirectionVector'/>
				<Property name='Default' type='vector2' value='(0.7,-1.0)'/>
			</Property>
			<Property name='ParticleAmount' type='int32'>
				<Property name='DisplayName' type='string' value='ParticleAmount'/>
				<Property name='Default' type='int32' value='5'/>
			</Property>
			<Property name='Dimensions' type='vector2'>
				<Property name='DisplayName' type='string' value='Dimensions'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"SparkColor",  &setSparkColor,  &getSparkColor),
		D2D1_VALUE_TYPE_BINDING(L"SmokeColor",  &setSmokeColor,  &getSmokeColor),
		D2D1_VALUE_TYPE_BINDING(L"BloomIntensityType",  &setBloomIntensityType,  &getBloomIntensityType),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
		D2D1_VALUE_TYPE_BINDING(L"ParticleSize",  &setParticleSize,  &getParticleSize),
		D2D1_VALUE_TYPE_BINDING(L"ParticleDirectionVector",  &setParticleDirectionVector,  &getParticleDirectionVector),
		D2D1_VALUE_TYPE_BINDING(L"ParticleAmount",  &setParticleAmount,  &getParticleAmount),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_SparkFlyingEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateSparkFlyingEffect
	);
	return hr;
}

HRESULT __stdcall SparkFlyingEffect::CreateSparkFlyingEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new SparkFlyingEffect();
	return S_OK;
}

SparkFlyingEffect::SparkFlyingEffect() {
	setSmokeColor(D2D1::Vector4F(1.0f, 0.43f, 0.1f, 1.0f));
	setSparkColor(D2D1::Vector4F(1.0f, 0.4f, 0.05f, 1.0f));
	setBloomIntensityType(SparkFlyingEffectBloomIntensity::SPARK_FLYING_EFFECT_BLOOM_INTENSITY_MEDIUM);
	setAnimationSpeed(1.0f);
	setParticleSize(0.01f);
	setParticleDirectionVector(D2D1::Vector2F(0.7f, -1.0f));
	setParticleAmount(SparkFlyingEffectParticleAmount::FEW);
	Constants.timePassed = 0;
}

SparkFlyingEffect::~SparkFlyingEffect() {

}

const GUID SparkFlyingEffect::getEffectGuid() const {
	return CLSID_SparkFlyingEffect;
}
const GUID SparkFlyingEffect::getEffectShaderGuid() const {
	return CLSID_SparkFlyingEffectPixelShader;
}