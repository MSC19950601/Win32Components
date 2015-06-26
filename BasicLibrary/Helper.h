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

	template<typename RectType>
	inline void CorrectRect(RectType& rect)
	{
		if (rect.left > rect.right) std::swap(rect.left, rect.right);
		if (rect.bottom < rect.top) std::swap(rect.top, rect.bottom);
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

	template<typename Type,typename RectType>
	inline Type GetRECTWidth(const RectType& rect)
	{
		return static_cast<Type>(rect.right - rect.left);
	}

	template<typename Type, typename RectType>
	inline Type GetRECTHeight(const RectType& rect)
	{
		return static_cast<Type>(rect.bottom - rect.top);
	}
}