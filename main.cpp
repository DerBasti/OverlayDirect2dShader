// MyShaderProject.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\ContentPanel.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\WindowCreator.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\Direct2DFactory.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\ImageViewer.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\Button.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\Animation.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Logging\ErrorCodeTranslator.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\PrettifyRectShader.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\GradientOverdrawEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\HighlightEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\MistWaveEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\BorderHighlighterEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\ColorizeImageEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\AxisHighlightShader.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\FlareEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\GUI\ColoredBox.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\ImageBorderHighlightEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\EdgeDetectionColorizerEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\CutImageEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\VoronoiLinesEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\VoronoiGradientEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\SparkFlyingEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\SmokeTornadoEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\WindTornadoEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\GlassShatteredEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\UnderwaterEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\FlyingSquaresBackgroundEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\BurningGroundEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\VibratingBorderEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\FrostedGlassEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Shader\WavingParticlesEffect.h"
#include "D:\Programmieren\CPP\Broadcaster\Common\Data\ItemDatabase.h"


class ShaderTest : public ContentPanel {
private:
	float progress;
	float secondProgress;
	float cutProgress;
	std::shared_ptr<Direct2DBitmap> bitmap;
	std::shared_ptr<Direct2DBitmap> otherBitmap;
	Microsoft::WRL::ComPtr<ID2D1Effect> effectToTest;
	GUIMultiGradientColor multiGradientColor;
	ImageViewerSettings* settings;
	PerformanceClock clock;
protected:
	virtual void onRendererReady() {
		otherBitmap = Direct2DFactory::GetInstance()->createBitmap(L"D:\\Programmieren\\CPP\\WhalepowerBroadcaster\\RELEASE\\Champions\\Splash\\3.jpg", getRenderer()->getRenderTarget());
		settings = new ImageViewerSettings(otherBitmap, getLocalDimensions());
		settings->setImageOffsets(RECT{ 100,100,720,480 });
		getRenderer()->getDeviceContext()->CreateEffect(CLSID_WavingParticlesEffect, effectToTest.GetAddressOf());
		effectToTest->SetValue(WavingParticlesProperty::WAVING_PARTICLES_PROPERTY_DIMENSIONS, D2D1::Vector2F(getDimensions().right, getDimensions().bottom));
		settings->addEffectToUse(effectToTest);
		/*
		bitmap = Direct2DFactory::GetInstance()->createBitmap(L"D:\\BayerMatrix.png", getRenderer()->getRenderTarget());
		effectToTest = Direct2DFactory::GetInstance()->createEffectWithSize(getRenderer()->getDeviceContext(), CLSID_VibratingBorderEffect, getLocalDimensions());
		otherBitmap = Direct2DFactory::GetInstance()->createBitmap(L"D:\\Programmieren\\CPP\\WhalepowerBroadcaster\\RELEASE\\Champions\\Splash\\3.jpg", getRenderer()->getRenderTarget());
		effectToTest->SetValue(VIBRATING_BORDER_EFFECT_PROPERTY_DIMENSIONS, D2D1::Vector2F(getDimensions().right, getDimensions().bottom));
		effectToTest->SetValue(VIBRATING_BORDER_EFFECT_PROPERTY_COLOR, GUIColor::WHALE_RED.asD2DVector4f());
		effectToTest->SetInput(1, bitmap->getBitmap().Get());
		*/
	}
	virtual void onDraw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderer, RECT rect) {
		ImageViewer::DrawOnGraphics(renderer, settings);
		getRenderer()->getDeviceContext()->DrawImage(effectToTest.Get());
	}
public:
	ShaderTest(Drawable* parent, RECT rect) : ContentPanel(parent, rect, false) {
		onRendererReady();
	}
	virtual ~ShaderTest() {

	}
	inline void updateProgress() {

	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int iCmd) {
	WindowCreatorStyle style(std::wstring(L"ShaderTest"), 1600, 900);
	HWND mainWindow = WindowCreator::CreateMainWindow(hInstance, &style, nullptr);
	auto mainWindowDrawable = Drawable::AsDrawable(mainWindow);
	HRESULT result = PrettifyRectEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = GradientOverdrawEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = HighlightEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = MistWaveEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = AxisHighlighterEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = BorderHighlighterEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = ColorizeImageEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = ImageBorderHighlightEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = FlareEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = EdgeDetectionColorizerEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = CutImageEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = VoronoiLinesEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = SparkFlyingEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = SmokeTornadoEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = WindTornadoEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = VoronoiGradientEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = VoronoiGradientEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = UnderwaterEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = FlyingSquaresBackgroundEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = BurningGroundEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = VibratingBorderEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = GlassShatteredEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = FrostedGlassEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	result = WavingParticlesEffect::Register(Direct2DFactory::GetInstance()->GetDXFactory());
	std::string error = ErrorCodeTranslator::GetErrorCodeString(GetLastError());
	const float xOffsetPercentage = 0.2435f;
	const float widthPercentage = 0.523f;
	const float heightPercentage = 0.093f;
	float y = -1.0f;
	float x = 0.7f;
	float angle = atan(x / y);
	std::unordered_map<size_t, size_t> firstMap{ {0,1}, {1,2} };
	std::unordered_map<size_t, size_t> secondMap{ {2,3}, {3,4} };

	std::unordered_map<size_t, size_t> copyMap = firstMap;
	copyMap.insert(secondMap.begin(), secondMap.end());

	RECT overviewPosition = { static_cast<int32_t>(0),
		0,
		1600,
		900
	};
	//ImageViewer* box = new ImageViewer(mainWindowDrawable.get(), overviewPosition, L"D:\\champselect.png");
	ShaderTest* test = new ShaderTest(mainWindowDrawable.get(), RECT{ 208,144,1280,720 }); //overviewPosition);
	test->addEndTimerCallback(0x00, DrawableTimerCallback([&]() {
		return true;
		}));
	SetTimer(test->getWindowHandle(), 0x00, 10, nullptr);
	ShowWindow(mainWindow, iCmd);
	UpdateWindow(mainWindow);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	WindowCreator::onShutdown();
	std::cout << "Hello World!\n";
	return 0;
}