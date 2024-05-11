#include "BorderHighlighterEffect.h"

extern "C" {
	const GUID CLSID_BorderHighlighterEffect = { 0x2c5d59fb, 0xf122, 0x4958, { 0x8c, 0x86, 0xb9, 0x49, 0xd3, 0xd7, 0x29, 0xe8 } };
	const GUID CLSID_BorderHighlighterEffectPixelShader = { 0xff4503ac, 0xd2e7, 0x4325, { 0x99, 0x64, 0x72, 0xe5, 0xdf, 0x69, 0x1f, 0x11 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT BorderHighlighterEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='BorderStrengthPercentage' type='float'>
				<Property name='DisplayName' type='string' value='BorderStrengthPercentage'/>
				<Property name='Default' type='float' value='0.01'/>
			</Property>
			<Property name='HighlightColor' type='vector4'>
				<Property name='DisplayName' type='string' value='HighlightColor'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"BorderStrengthPercentage",  &setBorderStrengthPercentage,  &getBorderStrengthPercentage),
		D2D1_VALUE_TYPE_BINDING(L"HighlightColor",  &setHighlightColor,  &getHighlightColor),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_BorderHighlighterEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateBorderHighlighterEffect
	);
	return hr;
}

HRESULT __stdcall BorderHighlighterEffect::CreateBorderHighlighterEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new BorderHighlighterEffect();
	return S_OK;
}

BorderHighlighterEffect::BorderHighlighterEffect() {
	Constants.width = Constants.height = 0;
	Constants.borderStrengthPercentage = 0.05f;
	Constants.highlighterColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
}

const GUID BorderHighlighterEffect::getEffectGuid() const {
	return CLSID_BorderHighlighterEffect;
}

const GUID BorderHighlighterEffect::getEffectShaderGuid() const {
	return CLSID_BorderHighlighterEffectPixelShader;
}