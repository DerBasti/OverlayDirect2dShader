#include "UnderwaterEffect.h"

extern "C" {
	const GUID CLSID_UnderwaterEffect = { 0x68ef620c, 0x7b, 0x421c, { 0x87, 0x7d, 0x20, 0x7c, 0xc5, 0xb9, 0x57, 0x2b } };
	const GUID CLSID_UnderwaterEffectPixelShader = { 0xad35eb46, 0xa206, 0x4c5b, { 0xbb, 0xad, 0xd0, 0x12, 0x8e, 0xab, 0x84, 0x59 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT UnderwaterEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='Seed' type='int32'>
				<Property name='DisplayName' type='string' value='Seed'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='Dimensions' type='vector2'>
				<Property name='DisplayName' type='string' value='Dimensions'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			<Property name='AnimationSpeed' type='float'>
				<Property name='DisplayName' type='string' value='AnimationSpeed'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Seed",  &setSeed,  &getSeed),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_UnderwaterEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateUnderwaterEffect
	);
	return hr;
}
HRESULT __stdcall UnderwaterEffect::CreateUnderwaterEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new UnderwaterEffect();
	return S_OK;
}

UnderwaterEffect::UnderwaterEffect() {
	seed = 0;
	Constants.dimensions = D2D1::Vector2F();
	Constants.timePassed = 0;
	Constants.animationSpeed = 1.0f;
}

UnderwaterEffect::~UnderwaterEffect() {

}

const GUID UnderwaterEffect::getEffectGuid() const {
	return CLSID_UnderwaterEffect;
}
const GUID UnderwaterEffect::getEffectShaderGuid() const {
	return CLSID_UnderwaterEffectPixelShader;
}