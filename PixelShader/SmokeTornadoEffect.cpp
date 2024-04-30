#include "SmokeTornadoEffect.h"

extern "C" {
	const GUID CLSID_SmokeTornadoEffect = { 0x83fdfb22, 0x8c9e, 0x41ba, { 0xbf, 0x44, 0x38, 0x7a, 0xd7, 0xb6, 0x5d, 0xa6 } };
	const GUID CLSID_SmokeTornadoEffectPixelShader = { 0x76e85765, 0xadf1, 0x4418, { 0x96, 0x74, 0x2c, 0xf7, 0xe8, 0x4f, 0x5e, 0x52 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT SmokeTornadoEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='CanvasOffset' type='vector2'>
				<Property name='DisplayName' type='string' value='CanvasOffset'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
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
		D2D1_VALUE_TYPE_BINDING(L"CanvasOffset",  &setCanvasOffset,  &getCanvasOffset),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_SmokeTornadoEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateSmokeTornadoEffect
	);
	return hr;
}
HRESULT __stdcall SmokeTornadoEffect::CreateSmokeTornadoEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new SmokeTornadoEffect();
	return S_OK;
}

SmokeTornadoEffect::SmokeTornadoEffect() {
	Constants.smokeColor = D2D1::Vector4F(1.0f, 0.5f, 0.4f, 1.0f);
	Constants.canvasOffset = D2D1::Vector2F();
	Constants.dimensions = D2D1::Vector2F();
	Constants.timePassed = 0;
	Constants.animationSpeed = 1.0f;
}

SmokeTornadoEffect::~SmokeTornadoEffect() {

}

const GUID SmokeTornadoEffect::getEffectGuid() const {
	return CLSID_SmokeTornadoEffect;
}
const GUID SmokeTornadoEffect::getEffectShaderGuid() const {
	return CLSID_SmokeTornadoEffectPixelShader;
}