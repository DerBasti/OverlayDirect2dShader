#include "FlareEffect.h"

extern "C" {
	const GUID CLSID_FlareEffect = { 0xb84690e5, 0xe61e, 0x4b49, { 0x90, 0xb6, 0xcf, 0x2d, 0xfd, 0x1f, 0xa5, 0x3d } };
	const GUID CLSID_FlareEffectPixelShader = { 0x24ca6b76, 0x174c, 0x4d60, { 0x81, 0xea, 0x8c, 0x7a, 0xd9, 0xe3, 0x98, 0x7b } };
};

FlareEffect::FlareEffect() {
	Constants.outerColor = D2D1::Vector4F(1.0f, 0.0f, 0.0f, 1.0f);
	Constants.innerColor = D2D1::Vector4F(1.0f, 1.0f, 0.2f, 0.5f);
	Constants.dimensions = D2D1::Vector2F(800.0f, 600.0f);
	Constants.noiseTextureDimensions = D2D1::Vector2F(800.0f, 600.0f);
	Constants.flareHeight = D2D1::Vector3F(0.5f, 0.6f, 0.65f);
	Constants.direction = FlareEffectDirection::TOP_TO_BOTTOM;
}

FlareEffect::~FlareEffect() {

}

HRESULT FlareEffect::Register(_In_ ID2D1Factory1* pFactory) {
	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
				<Property name='DisplayName' type='string' value='Wave Effect'/>
				<Property name='Author' type='string' value='Dieser Basti'/>
				<Property name='Category' type='string' value='Rect Pixelshader'/>
				<Property name='Description' type='string' value='Prettification of a simple rectangle'/>
				<Inputs minimum='2' maximum='2'>
					<Input name="Canvas"/>
					<Input name="NoiseTexture"/>
				</Inputs>
				<Property name='FlareOuterColor' type='vector4'>
					<Property name='DisplayName' type='string' value='FlareOuterColor'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
				<Property name='FlareInnerColor' type='vector4'>
					<Property name='DisplayName' type='string' value='FlareInnerColor'/>
					<Property name='Default' type='vector4' value='(1.0,1.0,1.0,1.0)'/>
				</Property>
				<Property name='Dimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='Dimensions'/>
					<Property name='Default' type='vector2' value='(800.0,600.0)'/>
				</Property>
					<Property name='NoiseTextureDimensions' type='vector2'>
					<Property name='DisplayName' type='string' value='NoiseTextureDimensions'/>
					<Property name='Default' type='vector2' value='(800.0,600.0)'/>
					</Property>
				<Property name='FlareHeight' type='vector3'>
					<Property name='DisplayName' type='string' value='FlareHeight'/>
					<Property name='Default' type='vector3' value='(0.5, 0.6, 0.65)'/>
				</Property>
				<Property name='FlareDirection' type='int32'>
					<Property name='DisplayName' type='string' value='FlareHeight'/>
					<Property name='Default' type='int32' value='0'/>
				</Property>
			</Effect>
		);

	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"FlareOuterColor",  &setFlareOuterColor,  &getFlareOuterColor),
		D2D1_VALUE_TYPE_BINDING(L"FlareInnerColor",  &setFlareInnerColor,  &getFlareInnerColor),
		D2D1_VALUE_TYPE_BINDING(L"Dimensions",  &setDimensions,  &getDimensions),
		D2D1_VALUE_TYPE_BINDING(L"NoiseTextureDimensions",  &setNoiseTextureDimensions,  &getNoiseTextureDimensions),
		D2D1_VALUE_TYPE_BINDING(L"FlareHeight",  &setFlareHeight,  &getFlareHeight),
		D2D1_VALUE_TYPE_BINDING(L"FlareDirection",  &setFlareDirection,  &getFlareDirection),
	};
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_FlareEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateFlareEffect
	);
	return hr;
}

HRESULT __stdcall FlareEffect::CreateFlareEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new FlareEffect();
	return S_OK;
}

void FlareEffect::onInitInternalEffects(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {
	pTransformGraph->AddNode(this);
	pTransformGraph->ConnectToEffectInput(0, this, 0);
	pTransformGraph->ConnectToEffectInput(1, this, 1);
	pTransformGraph->SetPassthroughGraph(1);
	pTransformGraph->SetOutputNode(this);
}

bool FlareEffect::needsInternalEffects() const {
	return false;
}

IFACEMETHODIMP_(UINT32) FlareEffect::GetInputCount() const {
	return 2;
}

const GUID FlareEffect::getEffectGuid() const {
	return CLSID_FlareEffect;
}

const GUID FlareEffect::getEffectShaderGuid() const {
	return CLSID_FlareEffectPixelShader;
}