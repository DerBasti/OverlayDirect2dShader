#include "CutImageEffect.h"

extern "C" {
	const GUID CLSID_CutImageEffect = { 0x74c62d4b, 0x5630, 0x408e, { 0xbb, 0x71, 0x9b, 0x70, 0xa4, 0xb5, 0x55, 0xd1 } };
	const GUID CLSID_CutImageEffectPixelShader = { 0x54d1f52d, 0x5731, 0x44ba, { 0xbe, 0xcc, 0xca, 0x8b, 0x80, 0x54, 0xcf, 0xf2 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT CutImageEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='Angle' type='float'>
				<Property name='DisplayName' type='string' value='Angle'/>
				<Property name='Default' type = 'float' value='0.0'/>
			</Property>
			<Property name='CutWidth' type='float'>
				<Property name='DisplayName' type='string' value='CutWidth'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='Progress' type='float'>
				<Property name='DisplayName' type='string' value='Progress'/>
				<Property name='Default' type='float' value='0.0'/>
			</Property>
			<Property name='MaxProgress' type='float'>
				<Property name='DisplayName' type='string' value='MaxProgress'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='CutMode' type='int32'>
				<Property name='DisplayName' type='string' value='CutMode'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Angle",  &setCutAngle,  &getCutAngle),
		D2D1_VALUE_TYPE_BINDING(L"CutWidth",  &setCutWidth,  &getCutWidth),
		D2D1_VALUE_TYPE_BINDING(L"Progress",  &setProgress,  &getProgress),
		D2D1_VALUE_TYPE_BINDING(L"MaxProgress",  &setMaxProgress,  &getMaxProgress),
		D2D1_VALUE_TYPE_BINDING(L"CutMode",  &setCutMode,  &getCutMode),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_CutImageEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateCutImageEffect
	);
	return hr;
}

HRESULT __stdcall CutImageEffect::CreateCutImageEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new CutImageEffect();
	return S_OK;
}

CutImageEffect::CutImageEffect() {
	Constants.angle = 45.0f;
	Constants.cutWidth = 25.0f;
	Constants.progress = 0.0f;
	Constants.maxProgress = 1.0f;
	Constants.cutMode = CutImageCutMode::CUT_IMAGE_EFFECT_MODE_BOTH;
}

CutImageEffect::~CutImageEffect() {

}

const GUID CutImageEffect::getEffectGuid() const {
	return CLSID_CutImageEffect;
}
const GUID CutImageEffect::getEffectShaderGuid() const {
	return CLSID_CutImageEffectPixelShader;
}