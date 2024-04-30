#include "RegisterShader.h"

#include "../Shader/PrettifyRectShader.h"
#include "../Shader/HighlightEffect.h"
#include "../Shader/BorderHighlighterEffect.h"
#include "../Shader/MistWaveEffect.h"
#include "../Shader/WaveEffect.h"
#include "../Shader/FlareEffect.h"
#include "../Shader/AxisHighlightShader.h"
#include "../Shader/BurningGroundEffect.h"
#include "../Shader/ColorizeImageEffect.h"
#include "../Shader/FlyingSquaresBackgroundEffect.h"
#include "../Shader/GradientOverdrawEffect.h"
#include "../Shader/ImageBorderHighlightEffect.h"
#include "../Shader/EdgeDetectionColorizerEffect.h"
#include "../Shader/CutImageEffect.h"
#include "../Shader/SmokeTornadoEffect.h"
#include "../Shader/WindTornadoEffect.h"
#include "../Shader/VoronoiGradientEffect.h"
#include "../Shader/VoronoiLinesEffect.h"
#include "../Shader/SparkFlyingEffect.h"
#include "../Shader/UnderwaterEffect.h"
#include "../Shader/GlassShatteredEffect.h"
#include "../Shader/FrostedGlassEffect.h"

std::vector<std::pair<GUID, std::function<HRESULT(ID2D1Factory1*)>>> RegisterShaderFactory::GetShadersForRegistration() {
	std::vector<std::pair<GUID, std::function<HRESULT(ID2D1Factory1*)>>> registrationMap;

	registrationMap.push_back(std::make_pair(CLSID_PrettifyRectEffect, [](ID2D1Factory1* factory) { return PrettifyRectEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_HighlightEffect, [](ID2D1Factory1* factory) { return HighlightEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_BorderHighlighterEffect, [](ID2D1Factory1* factory) { return BorderHighlighterEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_MistWaveEffect, [](ID2D1Factory1* factory) { return MistWaveEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_WaveEffect, [](ID2D1Factory1* factory) { return WaveEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_FlareEffect, [](ID2D1Factory1* factory) { return FlareEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_AxisHighlighterEffect, [](ID2D1Factory1* factory) { return AxisHighlighterEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_BurningGroundEffect, [](ID2D1Factory1* factory) { return BurningGroundEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_ColorizeImageEffect, [](ID2D1Factory1* factory) { return ColorizeImageEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_FlyingSquaresBackgroundEffect, [](ID2D1Factory1* factory) { return FlyingSquaresBackgroundEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_GradientOverdrawEffect, [](ID2D1Factory1* factory) { return GradientOverdrawEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_ImageBorderHighlightEffect, [](ID2D1Factory1* factory) { return ImageBorderHighlightEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_EdgeDetectionColorizerEffect, [](ID2D1Factory1* factory) { return EdgeDetectionColorizerEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_CutImageEffect, [](ID2D1Factory1* factory) { return CutImageEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_SmokeTornadoEffect, [](ID2D1Factory1* factory) { return SmokeTornadoEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_WindTornadoEffect, [](ID2D1Factory1* factory) { return WindTornadoEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_VoronoiGradientEffect, [](ID2D1Factory1* factory) { return VoronoiGradientEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_VoronoiLinesEffect, [](ID2D1Factory1* factory) { return VoronoiLinesEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_SparkFlyingEffect, [](ID2D1Factory1* factory) { return SparkFlyingEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_UnderwaterEffect, [](ID2D1Factory1* factory) { return UnderwaterEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_GlassShatteredEffect, [](ID2D1Factory1* factory) { return GlassShatteredEffect::Register(factory); }));
	registrationMap.push_back(std::make_pair(CLSID_FrostedGlassEffect, [](ID2D1Factory1* factory) { return FrostedGlassEffect::Register(factory); }));

	return registrationMap;
}