#include "FlyingSquaresBackgroundEffect.h"

extern "C" {
	const GUID CLSID_FlyingSquaresBackgroundEffect = { 0x5846094, 0x4989, 0x40e7, { 0xb1, 0x29, 0x5, 0xa8, 0x6f, 0x81, 0x9b, 0x19 } };
	const GUID CLSID_FlyingSquaresBackgroundEffectPixelShader = { 0x718f8b2e, 0xe40b, 0x46c1, { 0xba, 0x59, 0xec, 0x75, 0x41, 0xcc, 0x27, 0x9f } };
};

HRESULT FlyingSquaresBackgroundEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='SmokeColor' type='vector4'>
					<Property name='DisplayName' type='string' value='SmokeColor'/>
					<Property name='Default' type='vector4' value='(0,0,0,0)'/>
				</Property>
				<Property name='SquaresColor' type='vector4'>
					<Property name='DisplayName' type='string' value='SquaresColor'/>
					<Property name='Default' type='vector4' value='(0,0,0,0)'/>
				</Property>
				<Property name='CanvasDimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='CanvasDimensions'/>
					<Property name='Default' type='vector2' value='(0,0)'/>
				</Property>
				<Property name='SquaresDistributionPercentage' type='float'>
					<Property name='DisplayName' type='string' value='SquaresDistributionPercentage'/>
					<Property name='Default' type='float' value='0.6'/>
				</Property>	
				<Property name='SquaresAmount' type='int32'>
					<Property name='DisplayName' type='string' value='SquaresAmount'/>
					<Property name='Default' type='int32' value='50'/>
				</Property>
			</Effect>
		);


	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"SmokeColor",  &setSmokeColor,  &getSmokeColor),
		D2D1_VALUE_TYPE_BINDING(L"SquaresColor",  &setSquaresColor,  &getSquaresColor),
		D2D1_VALUE_TYPE_BINDING(L"CanvasDimensions",  &setCanvasDimensions,  &getCanvasDimensions),
		D2D1_VALUE_TYPE_BINDING(L"SquaresDistributionPercentage",  &setHeightDistributionPercentage,  &getHeightDistributionPercentage),
		D2D1_VALUE_TYPE_BINDING(L"SquaresAmount",  &setAmountOfSquares,  &getAmountOfSquares),
	};

	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_FlyingSquaresBackgroundEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateFlyingSquaresBackgroundEffect
	);
}

HRESULT __stdcall FlyingSquaresBackgroundEffect::CreateFlyingSquaresBackgroundEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new FlyingSquaresBackgroundEffect();
	return S_OK;
}


FlyingSquaresBackgroundEffect::FlyingSquaresBackgroundEffect() {
	Constants.dimensions = D2D1::Vector2F(1.0f, 1.0f);
	Constants.distributionOverHeightPercentage = 0.6f;
	Constants.smokeColor = D2D1::Vector4F(0.01f, 0.16f, 0.42f, 1.0f);
	Constants.squaresAmount = 50;
	Constants.squaresColor = D2D1::Vector4F(0.01f, 0.25f, 0.575f, 1.0f);
	Constants.timePassed = 0;
}

FlyingSquaresBackgroundEffect::~FlyingSquaresBackgroundEffect() {

}

const GUID FlyingSquaresBackgroundEffect::getEffectShaderGuid() const {
	return CLSID_FlyingSquaresBackgroundEffectPixelShader;
}

const GUID FlyingSquaresBackgroundEffect::getEffectGuid() const {
	return CLSID_FlyingSquaresBackgroundEffect;
}
