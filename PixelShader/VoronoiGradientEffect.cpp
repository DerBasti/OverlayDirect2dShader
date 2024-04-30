#include "VoronoiGradientEffect.h"

extern "C" {
	const GUID CLSID_VoronoiGradientEffect = { 0xcd80225c, 0x604c, 0x48d7, { 0x8c, 0x65, 0x14, 0x9, 0xbd, 0xcf, 0x6c, 0xf0 } };
	const GUID CLSID_VoronoiGradientEffectPixelShader = { 0x2ce56e5e, 0x47e7, 0x4d89, { 0x91, 0xe7, 0x90, 0x6d, 0xae, 0xf7, 0xdc, 0xdb } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

HRESULT VoronoiGradientEffect::Register(_In_ ID2D1Factory1* pFactory) {
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
					<Property name='Default' type='vector2' value='(0.0, 0.0)'/>
				</Property>
				<Property name='ColorLeft' type='vector4'>
					<Property name='DisplayName' type='string' value='ColorLeft'/>
					<Property name='Default' type='vector4' value='(0.0, 0.0, 0.0, 1.0)'/>
				</Property>
				<Property name='ColorCenterLeft' type='vector4'>
					<Property name='DisplayName' type='string' value='ColorCenterLeft'/>
					<Property name='Default' type='vector4' value='(0.0, 0.0, 0.0, 1.0)'/>
				</Property>
				<Property name='ColorCenterRight' type='vector4'>
					<Property name='DisplayName' type='string' value='ColorCenterRight'/>
					<Property name='Default' type='vector4' value='(0.0, 0.0, 0.0, 1.0)'/>
				</Property>
				<Property name='ColorRight' type='vector4'>
					<Property name='DisplayName' type='string' value='ColorRight'/>
					<Property name='Default' type='vector4' value='(0.0, 0.0, 0.0, 1.0)'/>
				</Property>
				<Property name='ColorPositions' type='vector3'>
					<Property name='DisplayName' type='string' value='ColorPositions'/>
					<Property name='Default' type='vector3' value='(0.0, 0.5, 1.0)'/>
				</Property>
				<Property name='TileSize' type='float'>
					<Property name='DisplayName' type='string' value='TileSize'/>
					<Property name='Default' type='float' value='60.0'/>
				</Property>
				<Property name='Angle' type='float'>
					<Property name='DisplayName' type='string' value='Angle'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='AnimationSpeed' type='float'>
					<Property name='DisplayName' type='string' value='AnimationSpeed'/>
					<Property name='Default' type='float' value='1.0'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"ColorLeft",  &setColorLeft,  &getColorLeft),
		D2D1_VALUE_TYPE_BINDING(L"ColorCenterLeft",  &setColorMiddleLeft,  &getColorMiddleLeft),
		D2D1_VALUE_TYPE_BINDING(L"ColorCenterRight",  &setColorMiddleRight,  &getColorMiddleRight),
		D2D1_VALUE_TYPE_BINDING(L"ColorRight",  &setColorRight,  &getColorRight),
		D2D1_VALUE_TYPE_BINDING(L"ColorPositions",  &setColorPositions,  &getColorPositions),
		D2D1_VALUE_TYPE_BINDING(L"TileSize",  &setTileSize,  &getTileSize),
		D2D1_VALUE_TYPE_BINDING(L"Angle",  &setAngle,  &getAngle),
		D2D1_VALUE_TYPE_BINDING(L"AnimationSpeed",  &setAnimationSpeed,  &getAnimationSpeed),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_VoronoiGradientEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateVoronoiGradientEffect
	);
	return hr;
}

HRESULT __stdcall VoronoiGradientEffect::CreateVoronoiGradientEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new VoronoiGradientEffect();
	return S_OK;
}

VoronoiGradientEffect::VoronoiGradientEffect() {
	Constants.colorPositions = D2D1::Vector4F(0.0f, 0.25f, 0.75f, 1.0f);
	Constants.leftColor = D2D1::Vector4F(0.3f, 0.55f, 0.69f, 1.0f);
	Constants.middleLeftColor = D2D1::Vector4F(0.031f, 0.08f, 0.08f, 1.0f);
	Constants.middleRightColor = D2D1::Vector4F(0.031f, 0.08f, 0.08f, 1.0f);
	Constants.rightColor = D2D1::Vector4F(0.7f, 0.2f, 0.2f, 1.0f);
	Constants.timePassedInMillis = 0;
	Constants.angle = 0.0f;
	Constants.animationSpeed = 1.0f;
	Constants.tileSize = 60.0f;
	Constants.dimensions = D2D1::Vector2F();
}

VoronoiGradientEffect::~VoronoiGradientEffect() {

}

const GUID VoronoiGradientEffect::getEffectGuid() const {
	return CLSID_VoronoiGradientEffect;
}
const GUID VoronoiGradientEffect::getEffectShaderGuid() const {
	return CLSID_VoronoiGradientEffectPixelShader;
}