#include "MistWaveTwoEdgesEffect.h"

extern "C" {
	const GUID CLSID_MistWaveTwoEdgesEffect = { 0x4b64b5c, 0x39f, 0x4ae7, { 0x81, 0x44, 0x1f, 0x8d, 0xd, 0x1e, 0xfe, 0xf9 } };
	const GUID CLSID_MistWaveTwoEdgesEffectPixelShader = { 0xd3a2b32e, 0x7eb7, 0x4f66, { 0x9b, 0x83, 0x8, 0x1b, 0x0, 0x7c, 0x81, 0xf3 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT MistWaveTwoEdgesEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='TimePassed' type='float'>
				<Property name='DisplayName' type='string' value='TimePassed'/>
				<Property name='Default' type='float' value='0.0'/>
			</Property>
			<Property name='ZoomLevel' type='int32'>
				<Property name='DisplayName' type='string' value='ZoomLevel'/>
				<Property name='Default' type='int32' value='1'/>
			</Property>
			<Property name='BackgroundColor' type='vector4'>
				<Property name='DisplayName' type='string' value='BackgroundColor'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			<Property name='TargetColorLeft' type='vector4'>
				<Property name='DisplayName' type='string' value='TargetColorLeft'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			<Property name='TargetColorRight' type='vector4'>
				<Property name='DisplayName' type='string' value='TargetColorRight'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"TimePassed",  &setTimePassed,  &getTimePassed),
		D2D1_VALUE_TYPE_BINDING(L"ZoomLevel",  &setZoomLevel,  &getZoomLevel),
		D2D1_VALUE_TYPE_BINDING(L"BackgroundColor",  &setBackgroundColor,  &getBackgroundColor),
		D2D1_VALUE_TYPE_BINDING(L"TargetColorLeft",  &setTargetColorLeftSide,  &getTargetColorLeftSide),
		D2D1_VALUE_TYPE_BINDING(L"TargetColorRight",  &setTargetColorRightSide,  &getTargetColorRightSide),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_MistWaveTwoEdgesEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateMistWaveTwoEdgesEffect
	);
	return hr;
}

HRESULT __stdcall MistWaveTwoEdgesEffect::CreateMistWaveTwoEdgesEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new MistWaveTwoEdgesEffect();
	return S_OK;
}

MistWaveTwoEdgesEffect::MistWaveTwoEdgesEffect() {
	Constants.leftColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.rightColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.timePassed = 0.0f;
	Constants.zoomLevel = MistWaveTwoEdgesEffectZoomLevel::MIST_WAVE_TWO_EDGES_REGULAR;
	Constants.backgroundColor = D2D1::Vector4F(0, 0, 0, 1.0f);
}

const GUID MistWaveTwoEdgesEffect::getEffectGuid() const {
	return CLSID_MistWaveTwoEdgesEffect;
}
const GUID MistWaveTwoEdgesEffect::getEffectShaderGuid() const {
	return CLSID_MistWaveTwoEdgesEffectPixelShader;
}