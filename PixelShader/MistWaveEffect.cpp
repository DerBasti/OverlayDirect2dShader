#include "MistWaveEffect.h"


extern "C" {
	const GUID CLSID_MistWaveEffect = { 0x5a4af061, 0x4ca9, 0x4a79, { 0xa3, 0x36, 0x31, 0x17, 0x52, 0xde, 0x70, 0xa1 } };
	const GUID CLSID_MistWaveEffectPixelShader = { 0x83a15767, 0x3f80, 0x4ebd, { 0x99, 0x1d, 0xbb, 0x90, 0xff, 0xb6, 0xa4, 0xac } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT MistWaveEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='ZoomLevel' type='uint32'>
					<Property name='DisplayName' type='string' value='ZoomLevel'/>
					<Property name='Default' type='uint32' value='1'/>
				</Property>
				<Property name='TargetColor' type='vector4'>
					<Property name='DisplayName' type='string' value='TargetColor'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"TimePassed",  &setTimePassed,  &getTimePassed),
		D2D1_VALUE_TYPE_BINDING(L"ZoomLevel",  &setZoomLevel,  &getZoomLevel),
		D2D1_VALUE_TYPE_BINDING(L"TargetColor",  &setTargetColor,  &getTargetColor),
	};	
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_MistWaveEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateMistWaveEffect
	);
	return hr;
}

HRESULT __stdcall MistWaveEffect::CreateMistWaveEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new MistWaveEffect();
	return S_OK;
}

MistWaveEffect::MistWaveEffect() {
	Constants.color = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.timePassed = 0.0f;
	Constants.zoomLevel = MistWaveEffectZoomLevel::REGULAR;
}

const GUID MistWaveEffect::getEffectGuid() const {
	return CLSID_MistWaveEffect;
}
const GUID MistWaveEffect::getEffectShaderGuid() const {
	return CLSID_MistWaveEffectPixelShader;
}