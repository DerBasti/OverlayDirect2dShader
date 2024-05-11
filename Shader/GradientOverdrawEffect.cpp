#include "GradientOverdrawEffect.h"


extern "C" {
	const GUID CLSID_GradientOverdrawEffect = { 0x12af5d20, 0x3943, 0x4aa3, { 0x8e, 0x8e, 0xef, 0xc7, 0x87, 0xfe, 0x1b, 0x4b } };
	const GUID CLSID_GradientOverdrawEffectPixelShader = { 0x7f6bbc7d, 0x5d07, 0x431f, { 0xb2, 0x83, 0x4f, 0x70, 0xeb, 0xe9, 0xd7, 0xa0 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT GradientOverdrawEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='GradientStart' type='vector4'>
					<Property name='DisplayName' type='string' value='GradientStart'/>
					<Property name='Default' type='vector4' value='(0,0,0,0)'/>
				</Property>
				<Property name='GradientEnd' type='vector4'>
					<Property name='DisplayName' type='string' value='GradientEnd'/>
					<Property name='Default' type='vector4' value='(0,0,0,0)'/>
				</Property>
				<Property name='GradientPositions' type='vector2'>
					<Property name='DisplayName' type='string' value='GradientPositions'/>
					<Property name='Default' type='vector2' value='(0,0)'/>
				</Property>
			</Effect>
		);

	
	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"GradientStart",  &setStartColor,  &getStartColor),
		D2D1_VALUE_TYPE_BINDING(L"GradientEnd",  &setEndColor,  &getEndColor),
		D2D1_VALUE_TYPE_BINDING(L"GradientPositions",  &setPositionPercentages,  &getPositionPercentages),
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_GradientOverdrawEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateGradientOverdrawEffect
	);
}

HRESULT __stdcall GradientOverdrawEffect::CreateGradientOverdrawEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new GradientOverdrawEffect();
	return S_OK;
}


GradientOverdrawEffect::GradientOverdrawEffect() {
	Constants.startColor = D2D1::Vector4F(0.3f, 0.1f, 0.1f, 0.0f);
	Constants.endColor = D2D1::Vector4F(0.03f, 0.1f, 0.1f, 1.0f);
	Constants.positionPercentages = D2D1::Vector2F(0.2f, 0.75f);
}

GradientOverdrawEffect::~GradientOverdrawEffect() {

}
const GUID GradientOverdrawEffect::getEffectGuid() const {
	return CLSID_GradientOverdrawEffect;
}
const GUID GradientOverdrawEffect::getEffectShaderGuid() const {
	return CLSID_GradientOverdrawEffectPixelShader;
}