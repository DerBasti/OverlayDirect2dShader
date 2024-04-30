#ifndef __DIRECT2D_WAVE_SHADER__
#define __DIRECT2D_WAVE_SHADER__

#include <d2d1_1.h>
#include <d2d1effectauthor.h>  
#include <d2d1effecthelpers.h>
#include <wrl/client.h>
#include <inttypes.h>

DEFINE_GUID(CLSID_WaveEffect, 0x4ab0f341, 0x473b, 0x4924, 0xb4, 0x2a, 0xda, 0x65, 0x86, 0x0, 0x43, 0x53);
DEFINE_GUID(CLSID_WaveEffectVertexShader, 0x1d2c6971, 0xf2fd, 0x4d26, 0xb1, 0xa5, 0xe7, 0x52, 0x98, 0x27, 0xa6, 0xfb);

enum WaveProperty {
	WAVE_PROPERTY_OFFSET = 0,
	WAVE_PROPERTY_STRENGTH
};

class WaveEffect : public ID2D1EffectImpl, public ID2D1DrawTransform {
private:
	struct Vertex {
		float x;
		float y;
	};
	struct
	{
		D2D_MATRIX_4X4_F matrix;
		float sizeX;
		float sizeY;
		float waveOffset;
		float waveStrength;
		float outputX;
		float outputY;
	} drawingConstants;
	static constexpr uint32_t TESSELATION_AMOUNT = 72;
	static constexpr float SKEW_OFFSET = -0.4f;
	static constexpr float MARGIN_LEFT = 0.00f;
	static constexpr float MARGIN_RIGHT = 0.00f;
	static constexpr float MARGIN_TOP = 0.0f;
	static constexpr float MARTIN_BOTTOM = 0.0f;
	static constexpr float WAVE_HEIGHT = 60.0f;

	uint32_t vertexAmount;
	LONG m_refCount; // Internal ref count used by AddRef() and Release() methods.
	Microsoft::WRL::ComPtr<ID2D1VertexBuffer> vertexBuffer;
	ID2D1DrawInfo* drawInfo;
	D2D1_RECT_L inputRect;
	float waveOffset;
	float waveStrength;

	// Constructor should be private since it should never be called externally.
	WaveEffect();
	HRESULT generateVertexMesh(Vertex** vertexBuffer);
public:
	// 2.1 Declare ID2D1EffectImpl implementation methods.
	IFACEMETHODIMP Initialize(
		_In_ ID2D1EffectContext* pContextInternal,
		_In_ ID2D1TransformGraph* pTransformGraph
	);

	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);    
	
	IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pTransformGraph)
	{
		return E_NOTIMPL; // There's no need to implement this on effects with static input counts.
	}

	// 2.2 Declare effect registration methods.
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT CreateEffect(_Outptr_ IUnknown** ppEffectImpl);

	// Declare ID2D1DrawTransform implementation methods.
	IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pDrawInfo);

	// Declare ID2D1Transform implementation methods.
	IFACEMETHODIMP MapOutputRectToInputRects(
		_In_ const D2D1_RECT_L* pOutputRect,
		_Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
		UINT32 inputRectCount
	) const;

	IFACEMETHODIMP MapInputRectsToOutputRect(
		_In_reads_(inputRectCount) CONST D2D1_RECT_L *pInputRects,
		_In_reads_(inputRectCount) CONST D2D1_RECT_L *pInputOpaqueSubRects,
		UINT32 inputRectCount,
		_Out_ D2D1_RECT_L *pOutputRect,
		_Out_ D2D1_RECT_L *pOutputOpaqueSubRect
	);

	IFACEMETHODIMP MapInvalidRect(
		UINT32 inputIndex,
		D2D1_RECT_L invalidInputRect,
		_Out_ D2D1_RECT_L *pInvalidOutputRect
	) const;


	// Declare ID2D1TransformNode implementation methods.
	IFACEMETHODIMP_(UINT32) GetInputCount() const { return 1; }

	// 2.3 Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

	inline float getWaveOffset() const {
		return waveOffset;
	}
	inline HRESULT setWaveOffset(float newOffset) {
		this->waveOffset = newOffset;
		if (this->waveOffset >= 180.f) {
			this->waveOffset -= 180.0f;
		}
		return S_OK;
	}
	inline float getWaveStrength() const {
		return waveStrength;
	}
	inline HRESULT setWaveStrength(float newOffset) {
		this->waveStrength = newOffset;
		return S_OK;
	}
};

#endif //__DIRECT2D_WAVE_SHADER__