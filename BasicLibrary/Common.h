#pragma once

#include <cstddef>
#include <functional>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <atlbase.h>
#include <wincodecsdk.h>
#include <CommCtrl.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"ComCtl32.lib")
#pragma comment(lib,"Uxtheme.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#undef max
#undef min

namespace Yupei
{
	using ATL::CComPtr;

	inline HINSTANCE GetApplicationInstance() noexcept
	{
		return ::GetModuleHandle(nullptr);
	}
}