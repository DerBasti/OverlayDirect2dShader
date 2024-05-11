#ifndef __DIRECT2D_PRETTIFY_RECT_EFFECT__
#define __DIRECT2D_PRETTIFY_RECT_EFFECT__

#include <d2d1_1.h>
#include <d2d1effectauthor.h>  
#include <d2d1effecthelpers.h>
#include <wrl/client.h>
#include <inttypes.h>
#include <algorithm>

DEFINE_GUID(CLSID_PrettifyRectEffect, 0x808476ef, 0xb34d, 0x44ee, 0xb0, 0x67, 0xa4, 0xcb, 0xeb, 0x31, 0xd8, 0x8b);
DEFINE_GUID(CLSID_PrettifyRectEffectPixelShader, 0xc6815887, 0xb5b2, 0x4cde, 0xb1, 0xcd, 0x6f, 0x83, 0xd8, 0x81, 0xda, 0xd);

enum PrettifyRectProperty {
	PRETTIFY_RECT_PROGRESS,
	PRETTIFY_RECT_WAVE_FREQUENCY,
	PRETTIFY_RECT_WAVE_CENTER_HEIGHT_PERCENTAGE,
	PRETTIFY_RECT_WAVE_STRENGTH,
	PRETTIFY_RECT_WAVE_THICKNESS,
	PRETTIFY_RECT_DIMENSIONS,
};

struct WaveData {
	float waveCenterHeightPercentage;
	float waveStrength;
	float waveFrequency;
	float waveThickness;
};

class PrettifyRectEffect : public ID2D1EffectImpl, public ID2D1DrawTransform {
private:
	PrettifyRectEffect();
	ID2D1DrawInfo* drawInfo;
	int32_t refCount;
	struct {
		float width;
		float height;
		float progress;
		float waveFrequency;
		float waveCenterHeightPercentage;
		float waveStrength;
		float waveThickness;
		float seed;
	} Constants;
	bool manuallySetDimensions;
	RECT inputRect;
public:
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreatePrettifyRectEffect(_Outptr_ IUnknown** ppEffectImpl);

	// Declare ID2D1EffectImpl implementation methods.
	IFACEMETHODIMP Initialize(_In_ ID2D1EffectContext* pContextInternal,_In_ ID2D1TransformGraph* pTransformGraph);
	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);
	IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);
	IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);
	IFACEMETHODIMP MapOutputRectToInputRects(_In_ const D2D1_RECT_L* pOutputRect,_Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects, UINT32 inputRectCount) const;
	IFACEMETHODIMP MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,	_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects, UINT32 inputRectCount,_Out_ D2D1_RECT_L* pOutputRect, _Out_ D2D1_RECT_L* pOutputOpaqueSubRect);
	IFACEMETHODIMP MapInvalidRect(UINT32 inputIndex,D2D1_RECT_L invalidInputRect,_Out_ D2D1_RECT_L* pInvalidOutputRect) const;
	// Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

	// Declare ID2D1TransformNode implementation methods.
	IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return manuallySetDimensions ? 0 : 1;
	}
	inline float getProgress() const {
		return Constants.progress;
	}
	inline HRESULT setProgress(float newProgress) {
		Constants.progress = (std::min)(1.0f, newProgress/30.0f);
		return S_OK;
	}
	inline float getResetableProgress() const {
		if (Constants.width <= 0.0f) {
			return 0.0f;
		}
		float playingFinishedPixel = (float)sin(Constants.waveFrequency * 200.0f) * 180.0f / 3.1415f;
		float returnPixel = (float)sin(Constants.waveFrequency * 100.0f) * 180.0f / 3.1415f;

		float strength = (Constants.waveThickness * Constants.height / 2.0f);
		float waveCenter = (Constants.height * Constants.waveCenterHeightPercentage);
		float expectedWaveCenterEnd = waveCenter * playingFinishedPixel * Constants.waveStrength + waveCenter;

		
		//Freq: 0.01 => -4.25
		//Freq: 0.05 => -1,5 // -0,55 pro Punkt
		//Freq: 0.15 => -1.08 // -0,042 pro Punkt
		//Freq = 0.7 = -1.05f /// ~0,0001 pro Punkt
		//Constants.waveFrequency = 0.15f => -1.08f

		return 0.0f; //2.08
		//return -1.0 - (1.0f / sqrt(playingFinishedPixel - playingStartedPixel - 1));
	}
	inline HRESULT setResetableProgress(float prog) {
		return S_OK;
	}
	inline float getWaveFrequency() const {
		return Constants.waveFrequency;
	}
	inline HRESULT setWaveFrequency(float freq) {
		Constants.waveFrequency = freq;
		return S_OK;
	}
	inline float getWaveCenterHeightPercentage() const {
		return Constants.waveCenterHeightPercentage;
	}
	inline HRESULT setWaveCenterHeightPercentage(float percentage) {
		Constants.waveCenterHeightPercentage = (std::max)(0.0f, (std::min)(1.0f, percentage));
		return S_OK;
	}
	inline float getWaveStrength() const {
		return Constants.waveStrength;
	}
	inline HRESULT setWaveStrength(float strength) {
		Constants.waveCenterHeightPercentage = (std::max)(0.01f, (std::min)(1.0f, strength));
		return S_OK;
	}
	inline float getWaveThickness() const {
		return Constants.waveThickness;
	}
	inline HRESULT setWaveThickness(float strength) {
		Constants.waveThickness = (std::max)(0.0001f, (std::min)(1.0f, strength));
		return S_OK;
	}
};

#endif //__DIRECT2D_PRETTIFY_RECT_EFFECT__