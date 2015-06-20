#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

namespace Yupei
{
	struct GraphicsResource
	{
		friend struct PublicResource;
		CComPtr<ID2D1Factory> D2DFactory;
		CComPtr<IDWriteFactory> DWriteFactory;
		CComPtr<IWICImagingFactory> WicImagingFactory;
		//CComPtr<IWICComponentFactory> WicComponentFactory;
	private:
		GraphicsResource();
	};

	struct PublicResource
	{
	public:
		GraphicsResource& GetGraphicsResource()
		{
			return independentResource;
		}
		static PublicResource& GetInstance()
		{
			static PublicResource resource;
			return resource;
		}
	private:
		PublicResource();
		void InitializeCommonControls();
		void InitializeCom();
		
		GraphicsResource independentResource;
	};
}
