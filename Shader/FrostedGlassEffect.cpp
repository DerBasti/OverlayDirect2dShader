#include "FrostedGlassEffect.h"

extern "C" {
    const GUID CLSID_FrostedGlassEffect = { 0xcab49d51, 0x6427, 0x4dce, { 0xb2, 0xb5, 0xf0, 0x78, 0x59, 0xc2, 0xb3, 0x83 } };
    const GUID CLSID_FrostedGlassEffectPixelShader = { 0xb18e65a0, 0x4e7c, 0x4074, { 0xa0, 0x6f, 0x32, 0x8a, 0xd0, 0x79, 0x5f, 0x6c } };
};


FrostedGlassEffect::FrostedGlassEffect() {
	setDimensions(D2D1::Vector2F(1.0f, 1.0f));
	setEffectStrength(FrostedGlassEffectStrength::REGULAR); 
	setEffectFinesse(FrostedGlassEffectFinesse::FINE);
}

FrostedGlassEffect::~FrostedGlassEffect() {

}

HRESULT __stdcall FrostedGlassEffect::CreateFrostedGlassEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new FrostedGlassEffect();
	return S_OK;
}


const GUID FrostedGlassEffect::getEffectGuid() const {
	return CLSID_FrostedGlassEffect;
}

const GUID FrostedGlassEffect::getEffectShaderGuid() const {
	return CLSID_FrostedGlassEffectPixelShader;
}

HRESULT FrostedGlassEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='Default' type='vector2' value='(1.0,1.0)'/>
			</Property>
			<Property name='EffectStrength' type='int32'>
			<Property name='DisplayName' type='string' value='EffectStrength'/>
			<Property name='Default' type='int32' value='10'/>
			</Property>
			<Property name='EffectFinesse' type='int32'>
			<Property name='DisplayName' type='string' value='EffectFinesse'/>
			<Property name='Default' type='int32' value='15'/>
			</Property>
			</Effect>
		);


	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"EffectStrength",  &setEffectStrength,  &getEffectStrength),
		D2D1_VALUE_TYPE_BINDING(L"EffectFinesse",  &setEffectFinesse,  &getEffectFinesse),
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_FrostedGlassEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateFrostedGlassEffect
	);
}