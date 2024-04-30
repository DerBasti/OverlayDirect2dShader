#include "WindTornadoEffect.h"

extern "C" {
	const GUID CLSID_WindTornadoEffect = { 0x4b88be40, 0x27fe, 0x4180, { 0xaa, 0xeb, 0xfc, 0x6c, 0xfc, 0x6f, 0xa1, 0x1b } };
	const GUID CLSID_WindTornadoEffectPixelShader = { 0xefcd911b, 0x9656, 0x4ae8, { 0x8a, 0x8a, 0xdb, 0xdd, 0x97, 0xae, 0x4, 0x14 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT WindTornadoEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='SmokeColor' type='vector4'>
			<Property name='DisplayName' type='string' value='SmokeColor'/>
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
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"SmokeColor",  &setSmokeColor,  &getSmokeColor),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_WindTornadoEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateWindTornadoEffect
	);
	return hr;
}
HRESULT __stdcall WindTornadoEffect::CreateWindTornadoEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new WindTornadoEffect();
	return S_OK;
}

WindTornadoEffect::WindTornadoEffect() {
	Constants.smokeColor = D2D1::Vector4F(1.0f, 0.2f, 0.1f, 1.0f);
	Constants.dimensions = D2D1::Vector2F();
	Constants.timePassed = 0;
	Constants.animationSpeed = 1.0f;
}

WindTornadoEffect::~WindTornadoEffect() {

}

const GUID WindTornadoEffect::getEffectGuid() const {
	return CLSID_WindTornadoEffect;
}
const GUID WindTornadoEffect::getEffectShaderGuid() const {
	return CLSID_WindTornadoEffectPixelShader;
}