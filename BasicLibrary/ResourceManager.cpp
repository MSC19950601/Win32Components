#include "ResourceManager.h"
#include <CommCtrl.h>
#include <Windows.h>

namespace Yupei
{
	PublicResource::PublicResource()
	{
		InitializeCommonControls();
		InitializeCom();
	}
	void PublicResource::InitializeCommonControls()
	{
		INITCOMMONCONTROLSEX initStruct = { sizeof(INITCOMMONCONTROLSEX),
			ICC_WIN95_CLASSES };
		::InitCommonControlsEx(&initStruct);
	}
	void PublicResource::InitializeCom()
	{
		::CoInitialize(nullptr);
	}
	GraphicsResource::GraphicsResource()
	{
		::D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&D2DFactory);
		::DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&DWriteFactory));
		::CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&WicImagingFactory));
	}
}


