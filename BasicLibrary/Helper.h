#pragma once

#include <Windows.h>

namespace Yupei
{
	template<typename Type>
	inline void MoveRect(RECT& rect,Type x,Type y)
	{
		rect.left += static_cast<LONG>(x);
		rect.top += static_cast<LONG>(y);
		rect.right += static_cast<LONG>(x);
		rect.bottom += static_cast<LONG>(y);
	}

	inline HINSTANCE GetApplicationInstance() noexcept
	{
		return ::GetModuleHandle(nullptr);
	}

	struct DeviceHelper
	{
	public:
		static RECT GetResolution(HWND _windowHandle)
		{
			MONITORINFO info = { sizeof(MONITORINFO) };
			::GetMonitorInfo(
				::MonitorFromWindow(_windowHandle, MONITOR_DEFAULTTONEAREST),
				&info);
			return info.rcMonitor;
		}
	private:

	};
}