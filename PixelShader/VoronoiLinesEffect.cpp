#include "VoronoiLinesEffect.h"

extern "C" {
	const GUID CLSID_VoronoiLinesEffect = { 0x1bd6b1c0, 0xdeb5, 0x409f, { 0xa5, 0xee, 0x9e, 0x83, 0x34, 0xc2, 0x4c, 0x9f } };
	const GUID CLSID_VoronoiLinesEffectPixelShader = { 0x21015196, 0x1402, 0x42c8, { 0x85, 0xa6, 0xf, 0x8e, 0x15, 0x26, 0x29, 0xb4 } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT VoronoiLinesEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
			<Property name='ZoomLevel' type='int32'>
				<Property name='DisplayName' type='string' value='ZoomLevel'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='LineAmountPercentage' type='float'>
				<Property name='DisplayName' type='string' value='LineAmountPercentage'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='Progress' type='float'>
				<Property name='DisplayName' type='string' value='Progress'/>
				<Property name='Default' type='float' value='0.0'/>
			</Property>
			<Property name='AnimationSpeed' type='float'>
				<Property name='DisplayName' type='string' value='AnimationSpeed'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='AnimationStrength' type='float'>
				<Property name='DisplayName' type='string' value='AnimationSpeed'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			<Property name='Seed' type='int32'>
				<Property name='DisplayName' type='string' value='Seed'/>
				<Property name='Default' type='int32' value='0'/>
			</Property>
			<Property name='LineColor' type='vector4'>
				<Property name='DisplayName' type='string' value='Seed'/>
				<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
			</Property>
			<Property name='LineStrength' type='float'>
				<Property name='DisplayName' type='string' value='LineStrength'/>
				<Property name='Default' type='float' value='1.0'/>
			</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"ZoomLevel",  &setZoomLevel,  &getZoomLevel),
		D2D1_VALUE_TYPE_BINDING(L"LineAmountPercentage",  &setLineAmountPercentage,  &getLineAmountPercentage),
		D2D1_VALUE_TYPE_BINDING(L"Progress",  &setProgress,  &getProgress),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
		D2D1_VALUE_TYPE_BINDING(L"AnimationStrength",  &setAnimationStrength,  &getAnimationStrength),
		D2D1_VALUE_TYPE_BINDING(L"Seed",  &setSeed,  &getSeed),
		D2D1_VALUE_TYPE_BINDING(L"LineColor",  &setLineColor,  &getLineColor),
		D2D1_VALUE_TYPE_BINDING(L"LineStrength",  &setLineStrength,  &getLineStrength),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_VoronoiLinesEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateVoronoiLinesEffect
	);
	return hr;
}

HRESULT __stdcall VoronoiLinesEffect::CreateVoronoiLinesEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new VoronoiLinesEffect();
	return S_OK;
}

VoronoiLinesEffect::VoronoiLinesEffect() {
	Constants.animationSpeed = 1.0f;
	Constants.animationStrength = 1.0f;
	Constants.lineAmountPercentage = 1.0f;
	Constants.progress = 0.0f;
	Constants.seed = 0;
	Constants.zoomLevel = VoronoiLinesEffectZoomLevel::VORONOI_LINES_REGULAR;
	Constants.width = Constants.height = 1280.0f;
	Constants.lineColor = D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
	Constants.lineThickness = 1.0f;
}

VoronoiLinesEffect::~VoronoiLinesEffect() {

}

const GUID VoronoiLinesEffect::getEffectGuid() const {
	return CLSID_VoronoiLinesEffect;
}
const GUID VoronoiLinesEffect::getEffectShaderGuid() const {
	return CLSID_VoronoiLinesEffectPixelShader;
}