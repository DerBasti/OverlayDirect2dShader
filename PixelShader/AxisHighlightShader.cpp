#include "AxisHighlightShader.h"


extern "C" {
	const GUID CLSID_AxisHighlighterEffect = { 0xe160834a, 0x349a, 0x461f, { 0xb9, 0x4e, 0x2e, 0x61, 0xca, 0xf0, 0xa, 0xe0 } };
	const GUID CLSID_AxisHighlighterEffectPixelShader = { 0x387344c8, 0xe069, 0x4f48, { 0xab, 0x7f, 0x77, 0x4e, 0x6d, 0x15, 0x4e, 0x81 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT AxisHighlighterEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='WidthAndHeight' type='vector2'>
				<Property name='DisplayName' type='string' value='WidthAndHeight'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			<Property name='axisPointFirst' type='vector2'>
				<Property name='DisplayName' type='string' value='axisPointFirst'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			<Property name='axisPointSecond' type='vector2'>
				<Property name='DisplayName' type='string' value='axisPointSecond'/>
				<Property name='Default' type='vector2' value='(0.0,0.0)'/>
			</Property>
			<Property name='leftsideAxisColor' type='vector4'>
				<Property name='DisplayName' type='string' value='BorderStrengthPercentage'/>
				<Property name='Default' type='vector4' value='(0.0,0.0,1.0,1.0)'/>
			</Property>
			<Property name='rightsideAxisColor' type='vector4'>
				<Property name='DisplayName' type='string' value='rightsideAxisColor'/>
				<Property name='Default' type='vector4' value='(1.0,0.0,0.0,1.0)'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"WidthAndHeight",  &setWidthAndHeight,  &getWidthAndHeight),
		D2D1_VALUE_TYPE_BINDING(L"axisPointFirst",  &setAxisFirstPoint,  &getAxisFirstPoint),
		D2D1_VALUE_TYPE_BINDING(L"axisPointSecond",  &setAxisSecondPoint,  &getAxisSecondPoint),
		D2D1_VALUE_TYPE_BINDING(L"leftsideAxisColor",  &setAxisLeftsideColor,  &getAxisLeftsideColor),
		D2D1_VALUE_TYPE_BINDING(L"rightsideAxisColor",  &setAxisRightsideColor,  &getAxisRightsideColor),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_AxisHighlighterEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateAxisHighlighterEffect
	);
	return hr;
}

HRESULT __stdcall AxisHighlighterEffect::CreateAxisHighlighterEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new AxisHighlighterEffect();
	return S_OK;
}

AxisHighlighterEffect::AxisHighlighterEffect() {
	Constants.width = Constants.height = 0;
	Constants.axisPoint1 = D2D1::Vector2F();
	Constants.axisPoint2 = D2D1::Vector2F(1.0, 1.0);
	Constants.leftsideColor = D2D1::Vector4F(0, 0, 1, 1);
	Constants.rightsideColor = D2D1::Vector4F(1, 0, 0, 1);
	startTimepoint = std::chrono::system_clock::now().time_since_epoch();
}

void AxisHighlighterEffect::updateConstantsBuffer() {
	Constants.width = (float)getWidth();
	Constants.height = (float)getHeight();
	Constants.timePassed = (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - startTimepoint).count() / 60;
	if (Constants.timePassed >= 1500) {
		startTimepoint = std::chrono::system_clock::now().time_since_epoch();
		Constants.timePassed = 0;
	}
}

const GUID AxisHighlighterEffect::getEffectGuid() const {
	return CLSID_AxisHighlighterEffect;
}

const GUID AxisHighlighterEffect::getEffectShaderGuid() const {
	return CLSID_AxisHighlighterEffectPixelShader;
}