#pragma once

#include "Common.h"

namespace Yupei
{
	struct DeviceIndependentResource
	{
		CComPtr<ID2D1Factory> D2DFactory;
		CComPtr<IDWriteFactory> DWriteFactory;
		CComPtr<IWICImagingFactory> WicImagingFactory;
		//CComPtr<IWICComponentFactory> WicComponentFactory;
	};

}
