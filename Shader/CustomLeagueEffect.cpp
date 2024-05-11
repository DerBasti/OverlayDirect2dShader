#include "CustomLeagueEffect.h"
#include "../IO/FileReader.h"
#include "../ProjectFilePathHandler.h"

extern "C" {
	const GUID CLSID_LeagueBaseSourceEffect = { 0x8285adc8, 0x2453, 0x41d7, { 0x9f, 0xf9, 0x2b, 0x45, 0xbf, 0x87, 0xc8, 0x42 } };
};
#ifndef XML
#define XML(text) TEXT(#text)
#endif

IFACEMETHODIMP CustomLeagueBaseEffect::Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {
	std::string path = (ProjectFilePathHandler::GetDefaultFilePath() + std::string("Design/") + std::string(getShaderFileName()) + std::string(".cso"));
	FileInputReader fir(path.c_str());
	uint32_t fileLength = (uint32_t)fir.getFileSize();
	std::shared_ptr<char> shaderData = fir.readStringWrapped(fileLength);
	auto hr = pContextInternal->LoadPixelShader(getEffectShaderGuid(), (uint8_t*)shaderData.get(), fileLength);
	if (hr != S_OK) {
		logger.logError("Failed loading shader from expected path: ", path.c_str());
		return hr;
	}	
	if (needsInternalEffects()) {
		onInitInternalEffects(pContextInternal, pTransformGraph);
	}
	else {
		pTransformGraph->SetSingleTransformNode(this);
	}
	return S_OK;
}

IFACEMETHODIMP CustomLeagueBaseEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType) {
	uint32_t dataLength = 0;
	updateConstantsBuffer();
	HRESULT hr = onPrepareForRender(changeType);
	return hr;
}

IFACEMETHODIMP CustomLeagueBaseEffect::SetGraph(_In_ ID2D1TransformGraph* pGraph) {
	return E_NOTIMPL; // There's no need to implement this on effects with static input counts.
}

IFACEMETHODIMP CustomLeagueBaseEffect::SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo) {
	drawInfo = pRenderInfo;
	return pRenderInfo->SetPixelShader(getEffectShaderGuid());
}

IFACEMETHODIMP CustomLeagueBaseEffect::MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const {
	if (inputRectCount != GetInputCount())
	{
		return E_FAIL;
	}
	for (uint32_t i = 0; i < inputRectCount; i++) {
		pInputRects[i] = getOutputRect(*pOutputRect);
	}
	return S_OK;
}

IFACEMETHODIMP CustomLeagueBaseEffect::MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects, _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects, UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect) {
	HRESULT hr = S_OK;

	if (inputRectCount != GetInputCount())
	{
		return E_FAIL;
	}
	if (GetInputCount() == 0) {
		pOutputRect->left = getX();
		pOutputRect->top = getY();
		pOutputRect->right = getWidth() + getX();
		pOutputRect->bottom = getHeight() + getY();
		PrepareForRender(D2D1_CHANGE_TYPE_CONTEXT);
		ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
		return S_OK;
	}
	//RECT invisibleSubRect = RECT{ 0,0,0,0 };
	for (uint32_t i = 0; i < inputRectCount; i++) {
		pOutputRect[i] = pInputRects[i];
	}
	setWidth(pInputRects[0].right - pInputRects[0].left);
	setHeight(pInputRects[0].bottom - pInputRects[0].top);
	PrepareForRender(D2D1_CHANGE_TYPE_CONTEXT);

	ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
	return hr;
}

IFACEMETHODIMP CustomLeagueBaseEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L* pInvalidOutputRect) const {
	*pInvalidOutputRect = D2D1::RectL(0, 0, getWidth(), getHeight());

	return S_OK;
}

IFACEMETHODIMP_(ULONG) CustomLeagueBaseEffect::AddRef() {
	refCount++;
	return refCount;
}

IFACEMETHODIMP_(ULONG) CustomLeagueBaseEffect::Release() {
	refCount--;
	if (refCount <= 0) {
		delete this;
		return 0;
	}
	return refCount;
}

IFACEMETHODIMP CustomLeagueBaseEffect::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput) {
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

/*
HRESULT CustomLeagueSourceEffect::Register(_In_ ID2D1Factory1* pFactory) {
	PCWSTR propertyXml =
		XML(
			<?xml version='1.0'?>
			<Effect>
				<Property name='DisplayName' type='string' value='Wave Effect'/>
				<Property name='Author' type='string' value='Dieser Basti'/>
				<Property name='Category' type='string' value='Rect Pixelshader'/>
				<Property name='Description' type='string' value='Prettification of a simple rectangle'/>
				<Inputs>
				</Inputs>
			</Effect>
		);
	HRESULT hr = pFactory->RegisterEffectFromString(
		CLSID_LeagueBaseSourceEffect,
		propertyXml,
		nullptr,
		0,
		CreateLeagueSourceEffect
	);
	return hr;
}

HRESULT __stdcall CustomLeagueSourceEffect::CreateLeagueSourceEffect(_Outptr_ IUnknown** ppEffectImpl) {
	*ppEffectImpl = (ID2D1EffectImpl*)new CustomLeagueSourceEffect();
	return S_OK;
}
IFACEMETHODIMP CustomLeagueSourceEffect::Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {
	pTransformGraph->SetSingleTransformNode(this);
	return S_OK;
}

IFACEMETHODIMP CustomLeagueSourceEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType) {
	uint32_t dataLength = 0;
	updateConstantsBuffer();
	HRESULT hr = onPrepareForRender(changeType);
	return hr;
}

IFACEMETHODIMP CustomLeagueSourceEffect::SetGraph(_In_ ID2D1TransformGraph* pGraph) {
	return E_NOTIMPL; // There's no need to implement this on effects with static input counts.
}

IFACEMETHODIMP CustomLeagueSourceEffect::MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const {
	if (inputRectCount != 1)
	{
		return E_FAIL;
	}
	pInputRects[0] = *pOutputRect;
	return S_OK;
}

IFACEMETHODIMP CustomLeagueSourceEffect::MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects, _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects, UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect) {
	HRESULT hr = S_OK;

	if (inputRectCount != GetInputCount())
	{
		return E_FAIL;
	}
	if (GetInputCount() == 0) {
		pOutputRect->left = pOutputRect->top = 0;
		pOutputRect->right = 1920;
		pOutputRect->bottom = 1080;
		PrepareForRender(D2D1_CHANGE_TYPE_CONTEXT);
		ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
		return S_OK;
	}

	*pOutputRect = pInputRects[0];

	setWidth(pInputRects[0].right - pInputRects[0].left);
	setHeight(pInputRects[0].bottom - pInputRects[0].top);
	PrepareForRender(D2D1_CHANGE_TYPE_CONTEXT);

	ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
	return hr;
}

IFACEMETHODIMP CustomLeagueSourceEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L* pInvalidOutputRect) const {
	*pInvalidOutputRect = D2D1::RectL(0, 0, getWidth(), getHeight());

	return S_OK;
}

IFACEMETHODIMP_(ULONG) CustomLeagueSourceEffect::AddRef() {
	refCount++;
	return refCount;
}

IFACEMETHODIMP_(ULONG) CustomLeagueSourceEffect::Release() {
	refCount--;
	if (refCount <= 0) {
		delete this;
		return 0;
	}
	return refCount;
}

IFACEMETHODIMP CustomLeagueSourceEffect::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput) {
	*ppOutput = nullptr;
	HRESULT hr = S_OK;

	if (riid == __uuidof(ID2D1EffectImpl))
	{
		*ppOutput = reinterpret_cast<ID2D1EffectImpl*>(this);
	}
	else if (riid == __uuidof(ID2D1SourceTransform))
	{
		*ppOutput = static_cast<ID2D1SourceTransform*>(this);
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

HRESULT __declspec(nothrow) __stdcall CustomLeagueSourceEffect::SetRenderInfo(_In_ ID2D1RenderInfo *renderInfo) {
	renderInfo->SetCached(FALSE);
	//renderInfo->SetOutputBuffer(D2D1_BUFFER_PRECISION_8BPC_UNORM, D2D1_CHANNEL_DEPTH_1);
	return S_OK;
}

HRESULT __declspec(nothrow) __stdcall CustomLeagueSourceEffect::Draw(_In_ ID2D1Bitmap1 *target, _In_ CONST D2D1_RECT_L *drawRect, D2D1_POINT_2U targetOrigin) {
	if (!pixelData) {
		auto pixelFormat = target->GetPixelFormat();
		width = 1024;
		height = 1024;
		pixelData = new uint8_t[width * height * 4];
		memset(pixelData, 0, width*height * 4);
	}

	auto eff = (ID2D1Effect*)this;
	D2D1_RECT_U rect = D2D1::RectU(targetOrigin.x, targetOrigin.y, drawRect->right, drawRect->bottom);
	D2D1_MAPPED_RECT mappedRect;
	mappedRect.bits = pixelData;
	mappedRect.pitch = ((width + 3) & ~3) * 4;
	HRESULT hr = target->Map(D2D1_MAP_OPTIONS_WRITE | D2D1_MAP_OPTIONS_DISCARD, &mappedRect);
	memset(mappedRect.bits, 255, width*height * 4);
	hr = target->Unmap();
	return S_OK;
}
*/