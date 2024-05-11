#include "BurningGroundEffect.h"
#include "../GUI/GUIColor.h"

extern "C" {
	const GUID CLSID_BurningGroundEffect = { 0x4b556d17, 0xf8ac, 0x4a7c, { 0xa6, 0xe3, 0x21, 0x65, 0xba, 0x44, 0x74, 0x9 } };
	const GUID CLSID_BurningGroundEffectPixelShader = { 0xc89391a1, 0x42ef, 0x4cd4, { 0x8b, 0xa6, 0xa5, 0xc8, 0x7d, 0x8, 0xfc, 0x8d } };
};

BurningGroundEffect::BurningGroundEffect() {
	Constants.fireColor = GUIColor::LIGHT_RED.asD2DVector4f();
	Constants.backgroundColor = GUIColor::BLACK.asD2DVector4f();
	Constants.dimensions = D2D1::Vector2F(1.0f, 1.0f);
	Constants.timePassed = 0;
}
	
BurningGroundEffect::~BurningGroundEffect() {

}
	
HRESULT BurningGroundEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='FireColor' type='vector4'>
					<Property name='DisplayName' type='string' value='FireColor'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
				<Property name='BackgroundColor' type='vector4'>
					<Property name='DisplayName' type='string' value='BackgroundColor'/>
					<Property name='Default' type='vector4' value='(0.0,0.0,0.0,1.0)'/>
				</Property>
				<Property name='Dimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='Dimensions'/>
					<Property name='Default' type='vector2' value='(1.0,1.0)'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"FireColor",  &setFireColor,  &getFireColor),
		D2D1_VALUE_TYPE_BINDING(L"BackgroundColor",  &setBackgroundColor,  &getBackgroundColor),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_BurningGroundEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateBurningGroundEffect
	);
	return hr;
}
	
HRESULT __stdcall BurningGroundEffect::CreateBurningGroundEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new BurningGroundEffect();
	return S_OK;
}


const GUID BurningGroundEffect::getEffectGuid() const {
	return CLSID_BurningGroundEffect;
}

const GUID BurningGroundEffect::getEffectShaderGuid() const {
	return CLSID_BurningGroundEffectPixelShader;
}