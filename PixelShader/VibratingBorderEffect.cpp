#include "VibratingBorderEffect.h"

extern "C" {
	const GUID CLSID_VibratingBorderEffect = { 0x14a5fc10, 0x834, 0x4a52, { 0xa1, 0xd5, 0x85, 0xc0, 0xcc, 0xa6, 0x8a, 0xb8 } };
	const GUID CLSID_VibratingBorderEffectPixelShader = { 0xac9fd29, 0x55e9, 0x4b6b, { 0xa1, 0xbd, 0x97, 0x21, 0xe1, 0xc4, 0x40, 0xc } };
};

VibratingBorderEffect::VibratingBorderEffect() {
	Constants.animationSpeed = 1.0f;
	Constants.borderColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.dimensions = D2D1::Vector2F(1.0f, 1.0f);
	Constants.timePassed = 0;
	seed = 0;
}

VibratingBorderEffect::~VibratingBorderEffect() {

}

HRESULT __stdcall VibratingBorderEffect::CreateVibratingBorderEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new VibratingBorderEffect();
	return S_OK;
}

HRESULT VibratingBorderEffect::Register(_In_ ID2D1Factory1* pFactory) {
	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
			<Property name='DisplayName' type='string' value='VibratingBorderEffect'/>
			<Property name='Author' type='string' value='Dieser Basti'/>
			<Property name='Category' type='string' value='VibratingBorderEffect'/>
			<Property name='Description' type='string' value='Prettification of a simple rectangle'/>
			<Inputs>
				<Input name='Source'/>
				<Input name='NoiseMap'/>
			</Inputs>
			<Property name='BorderColor' type='vector4'>
				<Property name='DisplayName' type='string' value='BorderColor'/>
				<Property name='Default' type='vector4' value='(1.0,0.8,0.4,1.0)'/>
			</Property>
			<Property name='Dimensions' type='vector2'>
				<Property name='DisplayName' type='string' value='Dimensions'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			<Property name='AnimationSpeed' type='float'>
				<Property name='DisplayName' type='string' value='AnimationSpeed'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='Seed' type='int32'>
				<Property name='DisplayName' type='string' value='Seed'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"BorderColor",  &setBorderColor,  &getBorderColor),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed", &setAnimationSpeed,  &getAnimationSpeed),
		D2D1_VALUE_TYPE_BINDING(L"Seed",  &setSeed,  &getSeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_VibratingBorderEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateVibratingBorderEffect
	);
	return hr;
}

const GUID VibratingBorderEffect::getEffectGuid() const {
	return CLSID_VibratingBorderEffect;
}

const GUID VibratingBorderEffect::getEffectShaderGuid() const {
	return CLSID_VibratingBorderEffectPixelShader;
}