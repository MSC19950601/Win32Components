#include "Application.h"

namespace Yupei
{
	Application::~Application()
	{
		::CoUninitialize();
	}
	Application::Application()
	{
		InitializeResources();
		InitializeCommonControls();
		InitializeCom();
	}
	void Application::InitializeCommonControls()
	{
		INITCOMMONCONTROLSEX initStruct = { sizeof(INITCOMMONCONTROLSEX),
			ICC_WIN95_CLASSES };
		::InitCommonControlsEx(&initStruct);
	}
	void Application::InitializeCom()
	{
		::CoInitialize(nullptr);
	}
	void Application::InitializeResources()
	{
		::D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&resources.D2DFactory);
		::DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&resources.DWriteFactory));
		::CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&resources.WicImagingFactory));
	}
}