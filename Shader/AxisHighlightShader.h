#ifndef __DIRECT2D_AXIS_HIGHLIGHTER_EFFECT__
#define __DIRECT2D_AXIS_HIGHLIGHTER_EFFECT__

#include "CustomLeagueEffect.h"
#include <chrono>

DEFINE_GUID(CLSID_AxisHighlighterEffect, 0xe160834a, 0x349a, 0x461f, 0xb9, 0x4e, 0x2e, 0x61, 0xca, 0xf0, 0xa, 0xe0);
DEFINE_GUID(CLSID_AxisHighlighterEffectPixelShader, 0x387344c8, 0xe069, 0x4f48, 0xab, 0x7f, 0x77, 0x4e, 0x6d, 0x15, 0x4e, 0x81);

enum AxisHighlighterEffectProperty {
	AXIS_HIGHLIGHTER_PROPERTY_WIDTH_AND_HEIGHT,
	AXIS_HIGHLIGHTER_PROPERTY_AXISPOINT_FIRST,
	AXIS_HIGHLIGHTER_PROPERTY_AXISPOINT_SECOND,
	AXIS_HIGHLIGHTER_PROPERTY_AXISPOINT_LEFTSIDE_COLOR,
	AXIS_HIGHLIGHTER_PROPERTY_AXISPOINT_RIGHTSIDE_COLOR,
};

class AxisHighlighterEffect : public CustomLeagueBaseEffect {
private:
	struct {
		float width;
		float height;
		D2D_VECTOR_2F axisPoint1;
		D2D_VECTOR_2F axisPoint2;
		D2D_VECTOR_4F leftsideColor;
		D2D_VECTOR_4F rightsideColor;
		uint32_t timePassed;
	} Constants;
	std::chrono::system_clock::duration startTimepoint;
	AxisHighlighterEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "AxisHighlightShader";
	}
	virtual BYTE* getConstantData(uint32_t* dataLength) const {
		*dataLength = sizeof(Constants);
		return (BYTE*)&Constants;
	}
	virtual void updateConstantsBuffer();

	virtual const GUID getEffectGuid() const;
	virtual const GUID getEffectShaderGuid() const;
public:
	virtual ~AxisHighlighterEffect() {

	}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateAxisHighlighterEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline D2D1_VECTOR_2F getWidthAndHeight() const {
		return D2D1::Vector2F((float)getWidth(), (float)getHeight());
	}
	inline HRESULT setWidthAndHeight(D2D1_VECTOR_2F vector) {
		Constants.width = vector.x;
		Constants.height = vector.y;
		return S_OK;
	}

	inline D2D1_VECTOR_2F getAxisFirstPoint() const {
		return Constants.axisPoint1;
	}
	inline D2D1_VECTOR_2F getAxisSecondPoint() const {
		return Constants.axisPoint2;
	}
	inline HRESULT setAxisFirstPoint(D2D1_VECTOR_2F axisPoint) {
		Constants.axisPoint1 = axisPoint;
		return S_OK;
	}
	inline HRESULT setAxisSecondPoint(D2D1_VECTOR_2F axisPoint) {
		Constants.axisPoint2 = axisPoint;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getAxisLeftsideColor() const {
		return Constants.leftsideColor;
	}
	inline HRESULT setAxisLeftsideColor(D2D1_VECTOR_4F leftColor) {
		Constants.leftsideColor = leftColor;
		return S_OK;
	}
	inline D2D1_VECTOR_4F getAxisRightsideColor() const {
		return Constants.rightsideColor;
	}
	inline HRESULT setAxisRightsideColor(D2D1_VECTOR_4F rightsideColor) {
		Constants.rightsideColor = rightsideColor;
		return S_OK;
	}
	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}
};

#endif //__DIRECT2D_AXIS_HIGHLIGHTER_EFFECT__