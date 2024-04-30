#include "EdgeDetectionColorizerEffect.h"

extern "C" {
	const GUID CLSID_EdgeDetectionColorizerEffect = { 0x2240e67c, 0x110b, 0x4c10, { 0x87, 0xa, 0x3c, 0x51, 0x56, 0x35, 0x94, 0xe7 } };
	const GUID CLSID_EdgeDetectionColorizerEffectPixelShader = { 0x80b21189, 0x1fab, 0x4c91, { 0x89, 0x9b, 0xd3, 0x32, 0xe5, 0xde, 0x6a, 0x32 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif


EdgeDetectionColorizerEffect::EdgeDetectionColorizerEffect() {
	Constants.color = D2D1::Vector4F(1.0, 1.0, 1.0, 0.05f);
	Constants.progress = 0;
	Constants.maxProgress = 1000;
}

EdgeDetectionColorizerEffect::~EdgeDetectionColorizerEffect() {

}

HRESULT EdgeDetectionColorizerEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='Color' type='vector4'>
					<Property name='DisplayName' type='string' value='Color'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
				<Property name='MaxProgress' type='int32'>
					<Property name='DisplayName' type='string' value='MaxProgress'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
				<Property name='Progress' type='int32'>
					<Property name='DisplayName' type='string' value='Progress'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
			</Effect>
		);


	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Color",  &setColorization,  &getColorization),
		D2D1_VALUE_TYPE_BINDING(L"MaxProgress",  &setMaxProgress,  &getMaxProgress),
		D2D1_VALUE_TYPE_BINDING(L"Progress",  &setCurrentProgress,  &getCurrentProgress),
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_EdgeDetectionColorizerEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateEdgeDetectionColorizerEffect
	);
}

HRESULT __stdcall EdgeDetectionColorizerEffect::CreateEdgeDetectionColorizerEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new EdgeDetectionColorizerEffect();
	return S_OK;
}


const GUID EdgeDetectionColorizerEffect::getEffectGuid() const {
	return CLSID_EdgeDetectionColorizerEffect;
}

const GUID EdgeDetectionColorizerEffect::getEffectShaderGuid() const {
	return CLSID_EdgeDetectionColorizerEffectPixelShader;
}