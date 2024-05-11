#ifndef __DIRECT2D_CUSTOM_LEAGUE_EFFECT__
#define __DIRECT2D_CUSTOM_LEAGUE_EFFECT__

#include <d2d1_1.h>
#include <d2d1effectauthor.h>  
#include <d2d1effecthelpers.h>
#include <wrl/client.h>
#include <inttypes.h>
#include "../Logging/Logger.h"
#include "../PerformanceClock.h"

DEFINE_GUID(CLSID_LeagueBaseSourceEffect, 0x8285adc8, 0x2453, 0x41d7, 0x9f, 0xf9, 0x2b, 0x45, 0xbf, 0x87, 0xc8, 0x42);

#ifndef XML
#define XML(text) TEXT(#text)
#endif

class CustomLeagueBaseEffect : public ID2D1EffectImpl, public ID2D1DrawTransform {
private:
	ROSEThreadedLogger logger;
	ID2D1DrawInfo* drawInfo;
	uint32_t width;
	uint32_t height;
	uint32_t refCount;
	Microsoft::WRL::ComPtr<ID2D1Effect> sourceEffect;
	PerformanceClock timePassedClock;
protected:
	CustomLeagueBaseEffect() {
		drawInfo = nullptr;
		width = height = 0; 
		refCount = 1; 
	}

	virtual HRESULT onPrepareForRender(D2D1_CHANGE_TYPE type) {
		uint32_t length = 0;
		auto buffer = getConstantData(&length);
		drawInfo->SetPixelShaderConstantBuffer(buffer, length);
		return S_OK;
	}

	virtual const char* getShaderFileName() const {
		return "CustomLeagueEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		(*dataLength) = 0;
		return nullptr;
	}
	virtual void updateConstantsBuffer() {

	}
	virtual bool needsInternalEffects() const {
		return false;
	}
	virtual void onInitInternalEffects(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph) {

	}
	virtual void onHandleAdditionalInputRects(UINT32 inputRectCount, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects) const {

	}

	virtual RECT getOutputRect(const RECT& otherRect) const {
		return otherRect;
	}
	
	virtual const GUID getEffectGuid() const {
		return GUID();
	}
	virtual const GUID getEffectShaderGuid() const {
		return GUID();
	}

	inline virtual uint32_t getX() const {
		return 0;
	}
	inline virtual uint32_t getY() const {
		return 0;
	}
	inline uint32_t getWidth() const {
		return width;
	}
	inline uint32_t getHeight() const {
		return height;
	}
	inline HRESULT setWidth(uint32_t newWidth) {
		this->width = newWidth;
		return S_OK;
	}
	inline HRESULT setHeight(uint32_t newHeight) {
		this->height = newHeight;
		return S_OK;
	}
	inline uint32_t getTimePassed() const {
		return static_cast<uint32_t>(timePassedClock.getDuration());
	}
public:
	virtual ~CustomLeagueBaseEffect() {	}

	// Declare ID2D1EffectImpl implementation methods.
	IFACEMETHODIMP Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph);
	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);
	STDMETHOD(SetGraph)(_In_ ID2D1TransformGraph *transformGraph);
	//IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);
	IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);
	IFACEMETHODIMP MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const;
	IFACEMETHODIMP MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects, _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects, UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect);
	IFACEMETHODIMP MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L* pInvalidOutputRect) const;
	// Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}
};

/*
class CustomLeagueSourceEffect : public ID2D1EffectImpl, public ID2D1SourceTransform {
private:
	ID2D1DrawInfo* drawInfo;
	uint32_t width;
	uint32_t height;
	uint32_t refCount;
	uint8_t* pixelData;
protected:
	CustomLeagueSourceEffect() {
		width = height = 0;
		pixelData = nullptr;
		refCount = 1;
	}

	virtual HRESULT onPrepareForRender(D2D1_CHANGE_TYPE type) {
		uint32_t length = 0;
		auto buffer = getConstantData(&length);
		return S_OK;
	}

	virtual const char* getShaderFileName() const {
		return "CustomLeagueSourceEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		(*dataLength) = 0;
		return nullptr;
	}
	virtual void updateConstantsBuffer() {
	}

	virtual const GUID getEffectShaderGuid() const {
		return GUID();
	}
	virtual const GUID getEffectGuid() const {
		return GUID();
	}
	inline uint32_t getWidth() const {
		return width;
	}
	inline uint32_t getHeight() const {
		return height;
	}
	inline void setWidth(float newWidth) {
		this->width = newWidth;
	}
	inline void setHeight(float newHeight) {
		this->height = newHeight;
	}
public:
	virtual ~CustomLeagueSourceEffect() {	
		delete[] pixelData;
		pixelData = nullptr;
	}
	// Declare ID2D1EffectImpl implementation methods.
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateLeagueSourceEffect(_Outptr_ IUnknown** ppEffectImpl);
	IFACEMETHODIMP Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph);
	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);
	IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);
	IFACEMETHODIMP MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect, _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const;
	IFACEMETHODIMP MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects, _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects, UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect);
	IFACEMETHODIMP MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect, _Out_ D2D1_RECT_L* pInvalidOutputRect) const;
	// Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

	IFACEMETHODIMP SetRenderInfo(_In_ ID2D1RenderInfo *renderInfo);
	IFACEMETHODIMP Draw(_In_ ID2D1Bitmap1 *target, _In_ CONST D2D1_RECT_L *drawRect, D2D1_POINT_2U targetOrigin);

	// Declare ID2D1TransformNode implementation methods.
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 0;
	}
};
*/

#endif //__DIRECT2D_CUSTOM_LEAGUE_EFFECT__