#include "ImageBorderHighlightEffect.h"

extern "C" {
	const GUID CLSID_ImageBorderHighlightEffect = { 0xcd5da618, 0x67a0, 0x426b, { 0xb5, 0x29, 0xb8, 0x15, 0x54, 0x60, 0xe3, 0xff } };
	const GUID CLSID_ImageBorderHighlightEffectPixelShader = { 0x3869248b, 0x517e, 0x4078, { 0xbe, 0xb4, 0x66, 0xe1, 0xe6, 0xdd, 0x5b, 0x9 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

ImageBorderHighlightEffect::ImageBorderHighlightEffect() {
	Constants.borderColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.borderStrength = 1;
}

ImageBorderHighlightEffect::~ImageBorderHighlightEffect() {

}

BYTE* ImageBorderHighlightEffect::getConstantData(uint32_t* dataLength) const {
	*dataLength = sizeof(Constants);
	return (BYTE*)&Constants;
}

void ImageBorderHighlightEffect::updateConstantsBuffer() {

}

const GUID ImageBorderHighlightEffect::getEffectShaderGuid() const {
	return CLSID_ImageBorderHighlightEffectPixelShader;
}

const GUID ImageBorderHighlightEffect::getEffectGuid() const {
	return CLSID_ImageBorderHighlightEffect;
}

HRESULT ImageBorderHighlightEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
					<Property name='DisplayName' type='string' value = 'HighlightColor'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
				<Property name='BorderStrength' type='int32'>
					<Property name='DisplayName' type='string' value='BorderStrength'/>
					<Property name='Default' type='int32' value='1'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"HighlightColor",  &setBorderColor,  &getBorderColor),
		D2D1_VALUE_TYPE_BINDING(L"BorderStrength",  &setBorderStrength,  &getBorderStrength),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_ImageBorderHighlightEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateImageBorderHighlightEffect
	);
	return hr;
}
HRESULT __stdcall ImageBorderHighlightEffect::CreateImageBorderHighlightEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new ImageBorderHighlightEffect();
	return S_OK;
}