#include "ColorizeImageEffect.h"


extern "C" {
	const GUID CLSID_ColorizeImageEffect = { 0xb0e78ada, 0xca8f, 0x4164, { 0x8b, 0x22, 0x93, 0xa7, 0xd0, 0x63, 0x43, 0x5c } };
	const GUID CLSID_ColorizeImageEffectPixelShader = { 0x763ba2ed, 0xece6, 0x42ab, { 0x8c, 0x97, 0xa8, 0x94, 0x6f, 0xeb, 0xbc, 0x97 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT ColorizeImageEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='HighlightColor' type='vector4'>
				<Property name='DisplayName' type='string' value='HighlightColor'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"HighlightColor",  &setTargetColorization,  &getTargetColorization),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_ColorizeImageEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateColorizeImageEffect
	);
	return hr;
}

HRESULT __stdcall ColorizeImageEffect::CreateColorizeImageEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new ColorizeImageEffect();
	return S_OK;
}

ColorizeImageEffect::ColorizeImageEffect() {
	Constants.color = D2D1::Vector4F(1, 1, 1, 1);
}

ColorizeImageEffect::~ColorizeImageEffect() {

}

const GUID ColorizeImageEffect::getEffectGuid() const {
	return CLSID_ColorizeImageEffect;
}
const GUID ColorizeImageEffect::getEffectShaderGuid() const {
	return CLSID_ColorizeImageEffectPixelShader;
}