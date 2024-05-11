#ifndef __IMAGE_FADEOUT_EFFECT__
#define __IMAGE_FADEOUT_EFFECT__

#include "CustomLeagueEffect.h"

DEFINE_GUID(CLSID_ImageFadeoutEffect, 0x1065fac7, 0x8c4f, 0x493b, 0xaa, 0x45, 0x81, 0x84, 0x88, 0x24, 0xa8, 0xc);
DEFINE_GUID(CLSID_ImageFadeoutEffectPixelShader, 0xd09e369e, 0x263b, 0x4ff8, 0x99, 0x8e, 0x96, 0xb, 0x60, 0x15, 0x6a, 0xe9);

enum ImageFadeoutDirection {
	LEFT_TO_RIGHT,
	TOP_TO_BOTTOM,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP,
	INVALID_FADEOUT_DIRECTION
};

class ImageFadeoutEffect : public CustomLeagueBaseEffect {
private:
	struct {
		float width;
		float height;
		ImageFadeoutDirection direction;
		float cutoffPerctage;
	} Constants;

	ImageFadeoutEffect();
protected:
	virtual const char* getShaderFileName() const {
		return "ImageFadeoutEffect";
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
	virtual ~ImageFadeoutEffect() {

	}
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT __stdcall CreateImageFadeoutEffect(_Outptr_ IUnknown** ppEffectImpl);

	inline ImageFadeoutDirection getFadeoutDirection() const {
		return Constants.direction;
	}
	inline HRESULT setFadeoutDirection(ImageFadeoutDirection dir) {
		Constants.direction = dir;
		return S_OK;
	}
	inline float getCutoffPercentage() const {
		return Constants.cutoffPerctage;
	}
	inline HRESULT setCutoffPercentage(float percentage) {
		Constants.cutoffPerctage = percentage;
		return S_OK;
	}

	inline virtual IFACEMETHODIMP_(UINT32) GetInputCount() const {
		return 1;
	}
};

#endif //__IMAGE_FADEOUT_EFFECT__