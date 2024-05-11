#ifndef __EDGE_DETECTION_COLORIZER_EFFECT__
#define __EDGE_DETECTION_COLORIZER_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_EdgeDetectionColorizerEffect, 0x2240e67c, 0x110b, 0x4c10, 0x87, 0xa, 0x3c, 0x51, 0x56, 0x35, 0x94, 0xe7);
DEFINE_GUID(CLSID_EdgeDetectionColorizerEffectPixelShader, 0x80b21189, 0x1fab, 0x4c91, 0x89, 0x9b, 0xd3, 0x32, 0xe5, 0xde, 0x6a, 0x32);

enum EdgeDetectionColorizerProperties {
	EDGE_DETECTION_COLORIZER_COLOR,
	EDGE_DETECTION_COLORIZER_MAX_PROGRESS,
	EDGE_DETECTION_COLORIZER_CURRENT_PROGRESS,
};

class EdgeDetectionColorizerEffect : public CustomLeagueBaseEffect {
private:
	struct {
		D2D1_VECTOR_4F color;
		float width;
		float height;
		uint32_t maxProgress;
		uint32_t progress;
	} Constants;
protected:
	EdgeDetectionColorizerEffect();

	virtual const char* getShaderFileName() const {
		return "EdgeDetectionColorizerEffect";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer() {
		Constants.width = (float)getWidth();
		Constants.height = (float)getHeight();
	}

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~EdgeDetectionColorizerEffect();
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateEdgeDetectionColorizerEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_4F getColorization() const {
		return Constants.color;
	}
	inline HRESULT setColorization(D2D1_VECTOR_4F color) {
		Constants.color = color;
		return S_OK;
	}
	inline uint32_t getMaxProgress() const {
		return Constants.maxProgress;
	}
	inline HRESULT setMaxProgress(uint32_t newProgress) {
		Constants.maxProgress = newProgress;
		return S_OK;
	}
	inline uint32_t getCurrentProgress() const {
		return Constants.progress;
	}
	inline HRESULT setCurrentProgress(uint32_t newProgress) {
		Constants.progress = newProgress;
		return S_OK;
	}
};

#endif //__EDGE_DETECTION_COLORIZER_EFFECT__