#include "PrettifyRectShader.h"
#include "../IO/FileReader.h"
#include "../ProjectFilePathHandler.h"
#include <chrono>

extern "C" {
	const GUID CLSID_PrettifyRectEffect = { 0x808476ef, 0xb34d, 0x44ee, { 0xb0, 0x67, 0xa4, 0xcb, 0xeb, 0x31, 0xd8, 0x8b } };
	const GUID CLSID_PrettifyRectEffectPixelShader = { 0xc6815887, 0xb5b2, 0x4cde, { 0xb1, 0xcd, 0x6f, 0x83, 0xd8, 0x81, 0xda, 0xd } };
};

#ifndef XML
#define XML(text) TEXT(#text)
#endif

PrettifyRectEffect::PrettifyRectEffect() {
	this->refCount = 1;
	Constants.progress = 0.0f;
	Constants.waveFrequency = 0.30f;
	Constants.waveCenterHeightPercentage = 0.25f;
	Constants.waveStrength = 0.2f;
	Constants.waveThickness = 0.01f;
	Constants.seed = (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

HRESULT PrettifyRectEffect::Register(_In_ ID2D1Factory1* pFactory) {
	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
				<!--System Properties-->
				<Property name='DisplayName' type='string' value='Wave Effect'/>
				<Property name='Author' type='string' value='Dieser Basti'/>
				<Property name='Category' type='string' value='Rect Pixelshader'/>
				<Property name='Description' type='string' value='Prettification of a simple rectangle'/>
				<Inputs>
					<Input name='Source'/>
				</Inputs>
				<Property name='Progress' type='float'>
					<Property name='DisplayName' type='string' value='Progress'/>
					<Property name='Min' type='float' value='0.0'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.01'/>
				</Property>
				<Property name='WaveFrequency' type='float'>
					<Property name='DisplayName' type='string' value='WaveFrequency'/>
					<Property name='Min' type='float' value='0.0'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.01'/>
				</Property>
				<Property name='WaveCenterHeight' type='float'>
					<Property name='DisplayName' type='string' value='WaveCenterHeight'/>
					<Property name='Min' type='float' value='0.0'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.01'/>
				</Property>
				<Property name='WaveStrength' type='float'>
					<Property name='DisplayName' type='string' value='WaveStrength'/>
					<Property name='Min' type='float' value='0.01'/>
					<Property name='Max' type='float' value='1000.0'/>
					<Property name='Default' type='float' value='0.0'/>
				</Property>
				<Property name='WaveThickness' type='float'>
					<Property name='DisplayName' type='string' value='WaveThickness'/>
					<Property name='Min' type='float' value='0.01'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.001'/>
				</Property>
				<Property name='WaveResetProgress' type='float'>
					<Property name='DisplayName' type='string' value='WaveResetProgress'/>
					<Property name='Min' type='float' value='0.01'/>
					<Property name='Max' type='float' value='1.0'/>
					<Property name='Default' type='float' value='0.001'/>
				</Property>
			</Effect>
		);
	
	D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"Progress",  &setProgress,  &getProgress),
		D2D1_VALUE_TYPE_BINDING(L"WaveFrequency",  &setWaveFrequency,  &getWaveFrequency),
		D2D1_VALUE_TYPE_BINDING(L"WaveCenterHeight",  &setWaveCenterHeightPercentage,  &getWaveCenterHeightPercentage),
		D2D1_VALUE_TYPE_BINDING(L"WaveStrength",  &setWaveStrength,  &getWaveStrength),
		D2D1_VALUE_TYPE_BINDING(L"WaveThickness",  &setWaveThickness,  &getWaveThickness),
		D2D1_VALUE_TYPE_BINDING(L"WaveResetProgress",  &setResetableProgress,  &getResetableProgress),
	};
	// Register the effect using the data defined above.
	return pFactory->RegisterEffectFromString(
		CLSID_PrettifyRectEffect,
		propertyXml,
		bindings,
		ARRAYSIZE(bindings),
		CreatePrettifyRectEffect
	);
}

HRESULT __stdcall PrettifyRectEffect::CreatePrettifyRectEffect(_Outptr_ IUnknown** ppEffectImpl) {
	ppEffectImpl[0] = (ID2D1EffectImpl*)new PrettifyRectEffect();
	if (*ppEffectImpl == nullptr) {
		return E_OUTOFMEMORY;
	}
	return S_OK;
}

// Declare ID2D1EffectImpl implementation methods.
IFACEMETHODIMP PrettifyRectEffect::Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {
	std::string path = (ProjectFilePathHandler::GetDefaultFilePath() + std::string("Design/PrettifyRectShader.cso"));
	FileInputReader fir(path.c_str());
	uint32_t fileLength = (uint32_t)fir.getFileSize();
	std::shared_ptr<char> shaderData = fir.readStringWrapped(fileLength);
	auto hr = pContextInternal->LoadPixelShader(CLSID_PrettifyRectEffectPixelShader, (const byte*)shaderData.get(), fileLength);
	if (hr != S_OK) {
		return hr;
	}
	pTransformGraph->SetSingleTransformNode(this);
	return S_OK;
}

IFACEMETHODIMP PrettifyRectEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType) {
	return drawInfo->SetPixelShaderConstantBuffer(reinterpret_cast<const BYTE*>(&Constants), sizeof(Constants));
}

IFACEMETHODIMP PrettifyRectEffect::SetGraph(_In_ ID2D1TransformGraph* pGraph) {
	return E_NOTIMPL; // There's no need to implement this on effects with static input counts.
}

// Declare ID2D1DrawTransform implementation methods.
IFACEMETHODIMP PrettifyRectEffect::SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo) {
	drawInfo = pRenderInfo;
	return drawInfo->SetPixelShader(CLSID_PrettifyRectEffectPixelShader);
}

// Declare ID2D1Transform implementation methods.
IFACEMETHODIMP PrettifyRectEffect::MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const {
	if (inputRectCount != 1)
	{
		return E_FAIL;
	}
	pInputRects[0] = *pOutputRect;
	return S_OK;
}

IFACEMETHODIMP PrettifyRectEffect::MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,	_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
	UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect) {
	HRESULT hr = S_OK;

	if (inputRectCount != 1)
	{
		return E_FAIL;
	}

	inputRect = pInputRects[0];
	*pOutputRect = pInputRects[0];

	Constants.width = (float)(inputRect.right - inputRect.left);
	Constants.height = (float)(inputRect.bottom - inputRect.top);
	PrepareForRender(D2D1_CHANGE_TYPE_CONTEXT);

	ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
	return hr;
}
IFACEMETHODIMP PrettifyRectEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L* pInvalidOutputRect) const {
	*pInvalidOutputRect = inputRect;

	return S_OK;
}

// Declare IUnknown implementation methods.
IFACEMETHODIMP_(ULONG) PrettifyRectEffect::AddRef() {
	refCount++;
	return refCount;
}
IFACEMETHODIMP_(ULONG) PrettifyRectEffect::Release() {
	refCount--;
	if (refCount <= 0) {
		delete this;
		return 0;
	}
	return refCount;
}
IFACEMETHODIMP PrettifyRectEffect::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput) {
	*ppOutput = nullptr;
	HRESULT hr = S_OK;

	if (riid == __uuidof(ID2D1EffectImpl))
	{
		*ppOutput = reinterpret_cast<ID2D1EffectImpl*>(this);
	}
	else if (riid == __uuidof(ID2D1DrawTransform))
	{
		*ppOutput = static_cast<ID2D1DrawTransform*>(this);
	}
	else if (riid == __uuidof(ID2D1Transform))
	{
		*ppOutput = static_cast<ID2D1Transform*>(this);
	}
	else if (riid == __uuidof(ID2D1TransformNode))
	{
		*ppOutput = static_cast<ID2D1TransformNode*>(this);
	}
	else if (riid == __uuidof(IUnknown))
	{
		*ppOutput = this;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	if (*ppOutput != nullptr)
	{
		AddRef();
	}

	return hr;
}