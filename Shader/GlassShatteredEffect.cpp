#include "GlassShatteredEffect.h"

extern "C" {
	const GUID CLSID_GlassShatteredEffect = { 0x222518c2, 0xa214, 0x4158, { 0x93, 0x93, 0xfc, 0x92, 0xc5, 0xdd, 0x17, 0x3c } };
	const GUID CLSID_GlassShatteredEffectPixelShader = { 0xce0a78e, 0x9756, 0x4020, { 0x82, 0xe3, 0x74, 0x69, 0x14, 0x50, 0xc3, 0x9e } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT GlassShatteredEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='Dimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='Dimensions'/>
					<Property name='Default' type='vector2' value='(0.0,0.0)'/>
				</Property>
				<Property name='Center' type='vector2'>
					<Property name='DisplayName' type='string' value='Center'/>
					<Property name='Default' type='vector2' value='(0.0,0.0)'/>
				</Property>
				<Property name='GlassJitter' type='int32'>
					<Property name='DisplayName' type='string' value='GlassJitter'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
				<Property name='GlassDistortion' type='int32'>
					<Property name='DisplayName' type='string' value='GlassDistortion'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
				<Property name='FilterType' type='int32'>
					<Property name='DisplayName' type='string' value='FilterType'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
				<Property name='Seed' type='int32'>
					<Property name='DisplayName' type='string' value='Seed'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"Center",  &setCenter,  &getCenter),
		D2D1_VALUE_TYPE_BINDING(L"GlassJitter",  &setGlassJitter,  &getGlassJitter),
		D2D1_VALUE_TYPE_BINDING(L"GlassDistortion",  &setGlassDistortion,  &getGlassDistortion),
		D2D1_VALUE_TYPE_BINDING(L"FilterType",  &setGlassFilterType,  &getGlassFilterType),
		D2D1_VALUE_TYPE_BINDING(L"Seed",  &setSeed, &getSeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_GlassShatteredEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateGlassShatteredEffect
	);
	return hr;
}

HRESULT __stdcall GlassShatteredEffect::CreateGlassShatteredEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new GlassShatteredEffect();
	return S_OK;
}

GlassShatteredEffect::GlassShatteredEffect() {
	Constants.dimensions.x = 1;
	Constants.dimensions.y = 1;
	Constants.center.x = 0;
	Constants.center.y = 0;
	setSeed((uint32_t)time(nullptr));
	setGlassJitter(GlassShatteredDarkSpots::NONE); //static_cast<float>(GlassShatteredDarkSpots::SEVERAL) / 10.0f;
	setGlassDistortion(GlassShatteredDistortion::NORMAL); //static_cast<float>(GlassShatteredDistortion::LESS_THAN_NORMAL);
	setGlassFilterType(GlassShatteredFilterType::SIMPLE);
}

GlassShatteredEffect::~GlassShatteredEffect() {

}

const GUID GlassShatteredEffect::getEffectGuid() const {
	return CLSID_GlassShatteredEffect;
}
const GUID GlassShatteredEffect::getEffectShaderGuid() const {
	return CLSID_GlassShatteredEffectPixelShader;
}