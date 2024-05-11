#include "HighlightEffect.h"
#include "../IO/FileReader.h"
#include "../ProjectFilePathHandler.h"

extern "C" {
	const GUID CLSID_HighlightEffect = { 0xd71bf361, 0xd108, 0x4f8a, { 0xb4, 0x83, 0xea, 0x55, 0xc0, 0x2a, 0xff, 0x5 } };
	const GUID CLSID_HighlightEffectPixelShader = { 0x9ce83fc7, 0x51e2, 0x429c, { 0xab, 0xe3, 0x3f, 0xbf, 0x7d, 0xac, 0x27, 0x93 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT HighlightEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='Width' type='float'>
					<Property name='DisplayName' type='string' value='Width'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='Height' type='float'>
					<Property name='DisplayName' type='string' value='Height'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='Angle' type='float'>
					<Property name='DisplayName' type='string' value='Angle'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='Progress' type='float'>
					<Property name='DisplayName' type='string' value='Progress'/>
					<Property name='Default' type='float' value='0.5'/>
				</Property>
			</Effect>
		);


	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Width",  &setEmpty,  &getWidth),
		D2D1_VALUE_TYPE_BINDING(L"Height",  &setEmpty,  &getHeight),
		D2D1_VALUE_TYPE_BINDING(L"Angle",  &setAngle,  &getAngle),
		D2D1_VALUE_TYPE_BINDING(L"Progress",  &setProgress,  &getProgress),
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_HighlightEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateHighlightEffect
	);
}

HRESULT __stdcall HighlightEffect::CreateHighlightEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new HighlightEffect();
	return S_OK;
}


HighlightEffect::HighlightEffect() {
	constants = new Constants();
	constants->width = constants->height = 0.0f;
	constants->angle = 45.0f;
	constants->progress = 0.35f;
}