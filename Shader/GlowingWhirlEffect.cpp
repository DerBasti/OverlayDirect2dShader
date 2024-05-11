#include "GlowingWhirlEffect.h"
#include "../GUI/GUIColor.h"

extern "C" {
    const GUID CLSID_GlowingWhirlEffect = { 0x351a96b1, 0x4b14, 0x4873, { 0xa8, 0x46, 0xae, 0xc, 0x8f, 0x62, 0x92, 0x53 } };
    const GUID CLSID_GlowingWhirlEffectPixelShader = { 0x49ae1e22, 0xdd30, 0x49e6, { 0x8b, 0x72, 0x1b, 0x1c, 0x72, 0x4c, 0xa9, 0x73 } };
};

HRESULT GlowingWhirlEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
				<Property name='LeftsideColor' type='vector4'>
					<Property name='DisplayName' type='string' value='LeftsideColor'/>
					<Property name='Default' type='vector4' value='(0.0,0.0,0.0,1.0)'/>
				</Property>
				<Property name='RightsideColor' type='vector4'>
					<Property name='DisplayName' type='string' value='RightsideColor'/>
					<Property name='Default' type='vector4' value='(0.0,0.0,0.0,1.0)'/>
				</Property>
				<Property name='Dimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='Dimensions'/>
					<Property name='Default' type='vector2' value='(1.0, 1.0)'/>
				</Property>
				<Property name='AnimationSpeed' type='float'>
					<Property name='DisplayName' type='string' value='AnimationSpeed'/>
					<Property name='Default' type='float' value='4.0'/>
				</Property>
				<Property name='AnimationType' type='int32'>
					<Property name='DisplayName' type='string' value='AnimationType'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"LeftsideColor",  &setLeftSideColor,  &getLeftSideColor),
		D2D1_VALUE_TYPE_BINDING(L"RightsideColor",  &setRightSideColor,  &getRightSideColor),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
		D2D1_VALUE_TYPE_BINDING(L"AnimationType",  &setAnimationType,  &getAnimationType),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_GlowingWhirlEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateGlowingWhirlEffect
	);
	return hr;
}

HRESULT __stdcall GlowingWhirlEffect::CreateGlowingWhirlEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new GlowingWhirlEffect();
	return S_OK;
}

GlowingWhirlEffect::GlowingWhirlEffect() {
	setLeftSideColor(GUIColor::WHITE.asD2DVector4f());
	setRightSideColor(GUIColor::WHITE.asD2DVector4f());
	setDimensions(D2D1::Vector2F(1.0f, 1.0f));
	setAnimationSpeed(GlowingWhirlEffectAnimationSpeed::NORMAL);
	setAnimationType(GlowingWhirlEffectAnimationType::SIMPLY_FILLED);
	Constants.timePassed = 0;
}

GlowingWhirlEffect::~GlowingWhirlEffect() {

}

const GUID GlowingWhirlEffect::getEffectGuid() const {
	return CLSID_GlowingWhirlEffect;
}
const GUID GlowingWhirlEffect::getEffectShaderGuid() const {
	return CLSID_GlowingWhirlEffectPixelShader;
}