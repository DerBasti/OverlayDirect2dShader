#include "ImageFadeoutEffect.h"

extern "C" {
	const GUID CLSID_ImageFadeoutEffect = { 0x1065fac7, 0x8c4f, 0x493b, { 0xaa, 0x45, 0x81, 0x84, 0x88, 0x24, 0xa8, 0xc } };
	const GUID CLSID_ImageFadeoutEffectPixelShader = { 0xd09e369e, 0x263b, 0x4ff8, { 0x99, 0x8e, 0x96, 0xb, 0x60, 0x15, 0x6a, 0xe9 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT ImageFadeoutEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='Direction' type='int32'>
					<Property name='DisplayName' type='string' value='Direction'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
				<Property name='CutoffPercentage' type='float'>
					<Property name='DisplayName' type='string' value='CutoffPercentage'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Direction",  &setFadeoutDirection,  &getFadeoutDirection),
		D2D1_VALUE_TYPE_BINDING(L"CutoffPercentage",  &setFadeoutDirection,  &getFadeoutDirection),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_ImageFadeoutEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateImageFadeoutEffect
	);
	return hr;
}

HRESULT __stdcall ImageFadeoutEffect::CreateImageFadeoutEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new ImageFadeoutEffect();
	return S_OK;
}

ImageFadeoutEffect::ImageFadeoutEffect() {
	this->Constants.direction = ImageFadeoutDirection::LEFT_TO_RIGHT;
	this->Constants.cutoffPerctage = 0.2f;
}

const GUID ImageFadeoutEffect::getEffectGuid() const {
	return CLSID_ImageFadeoutEffect;
}
const GUID ImageFadeoutEffect::getEffectShaderGuid() const {
	return CLSID_ImageFadeoutEffectPixelShader;
}