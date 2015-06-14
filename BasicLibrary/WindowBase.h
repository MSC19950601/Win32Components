#pragma once

#include "Common.h"


namespace Yupei
{
	extern LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam,LPARAM lParam);

	struct WindowClass
	{
		static const wchar_t* const DefaultWindowClassName ;

		WindowClass();

		WNDCLASSEX windowClass;
	};
	
	struct DeviceIndependentResource;

	class WindowBase
	{
	public:

		friend class Application;

		friend LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		WindowBase();
		WindowBase(const std::wstring& title);
		
		
		HWND GetWindowHandle() const noexcept
		{
			return windowHandle;
		}

		virtual void ProcessMessage(HWND _windowHandle, UINT message, WPARAM wParam, LPARAM lParam, bool& wasHandled, LRESULT& result) {}
		DWORD SetWindowStyle(DWORD style)
		{
			return static_cast<DWORD>(SetWindowInfo(GWL_STYLE, static_cast<LONG_PTR>(style)));
		}
		DWORD SetExStyle(DWORD style)
		{
			return static_cast<DWORD>(SetWindowInfo(GWL_EXSTYLE, static_cast<LONG_PTR>(style)));
		}
		DWORD GetWindowStyle() const
		{
			return static_cast<DWORD>(GetWindowInfo(GWL_STYLE));
		}
		DWORD GetExStyle() const
		{
			return static_cast<DWORD>(GetWindowInfo(GWL_EXSTYLE));
		}

		LONG_PTR GetWindowInfo(int index) const
		{
			return ::GetWindowLongPtr(
				windowHandle,
				index);
		}

		LONG_PTR SetWindowInfo(int index, LONG_PTR info) 
		{
			return ::SetWindowLongPtr(
				windowHandle,
				index,
				info);
		}

		template <typename U,typename T>
		U DipsToPixelsX(T x) const noexcept
		{
			return static_cast<U>( static_cast<float>(x) * dpiX /96.f );
		}

		template <typename U, typename T>
		U DipsToPixelsY(T y) const noexcept
		{
			return static_cast<U>(static_cast<float>(y) * dpiY / 96.f);
		}
		
		void SetWindowSize(UINT width, UINT height);
		void SetClientSize(UINT width, UINT height);
		void SetFixedSize(UINT width, UINT height);
		RECT GetWindowPos() const;
		void Show();

		virtual void OnResize(UINT width, UINT height) {}
		virtual void OnRender() {}

		BOOL MoveWindowTo(
			int x,
			int y,
			int width,
			int height,
			BOOL isRepaint = TRUE)
		{
			return ::MoveWindow(windowHandle,
				DipsToPixelsX<int>(x),
				DipsToPixelsY<int>(y),
				DipsToPixelsX<int>(width),
				DipsToPixelsY<int>(height),
				TRUE);
		}
	protected:
		HWND windowHandle = nullptr;

		CComPtr<ID2D1Factory> d2dFactory;
		CComPtr<IDWriteFactory> dWriteFactory;
		CComPtr<IWICImagingFactory> wicImagingFactory;

		float dpiX, dpiY;

		ID2D1HwndRenderTarget* renderTarget = nullptr;

	private:
		void SetResources(const DeviceIndependentResource* _resources) noexcept;

		WPARAM Run();
		HWND Create(const std::wstring& title);
		void Initialize();
		void InitializeRenderTarget()
		{
			d2dFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(windowHandle),
				&renderTarget);
		}
		void InitializeDpi()
		{
			d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		}
		
	};
}
