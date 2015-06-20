#pragma once

#include "RoutedEvent.h"
#include "ResourceManager.h"
#include <Windows.h>
#include <memory>

namespace Yupei
{

	extern LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam,LPARAM lParam);

	struct WindowClass
	{
		static const wchar_t* const DefaultWindowClassName ;

		WindowClass();

		WNDCLASSEX windowClass;
		
	};
	
	struct GraphicsResource;

	class WindowBase
	{
	public:

		friend class ApplicationService;

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
		
		void SetWindowSizeWithPhysic(UINT width, UINT height);
		void SetClientSizeWithPhysic(UINT width, UINT height);
		void SetWindowSizeWithDip(UINT width, UINT height)
		{
			SetWindowSizeWithPhysic(
				DipsToPixelsX<UINT>(width),
				DipsToPixelsY<UINT>(height));
		}
		void SetClientSizeWithDip(UINT width, UINT height)
		{
			SetClientSizeWithPhysic(
				DipsToPixelsX<UINT>(width),
				DipsToPixelsY<UINT>(height));
		}
		
		RECT GetWindowPos() const;
		void Show();

		virtual void OnResize(UINT width, UINT height) {}
		virtual void OnRender() {}

		BOOL MoveWindowToWithDip(
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

		BOOL MoveWindowToWithPhysics(
			int x,
			int y,
			int width,
			int height,
			BOOL isRepaint = TRUE)
		{
			return ::MoveWindow(windowHandle,
				x,
				y,
				width,
				height,
				TRUE);
		}

		void SetFixed(bool isFixed = true)
		{
			if (isFixed)
			{
				SetWindowStyle(GetWindowStyle() & ~WS_THICKFRAME);
			}
			else
			{
				SetWindowStyle(GetWindowStyle() | WS_THICKFRAME);
			}
		}

		void Close()
		{
			::PostMessage(windowHandle, WM_CLOSE, 0, 0);
		}

		auto GetD2DFactory()
		{
			return resources.GetGraphicsResource().D2DFactory;
		}

		auto GetDWriteFactory()
		{
			return resources.GetGraphicsResource().DWriteFactory;
		}

		auto GetWicImagingFactory()
		{
			return resources.GetGraphicsResource().WicImagingFactory;
		}

		//Events

		Event<CreateArgs> WinCreate;
		Event<MouseArgs> MouseDown;
		Event<MouseArgs> MouseUp;
	protected:
		HWND windowHandle = nullptr;

		PublicResource resources;

		float dpiX, dpiY;

		ID2D1HwndRenderTarget* renderTarget = nullptr;
		virtual void OnCreate(void* sender, CreateArgs* args)
		{
			RoutedInvoke(WinCreate, sender, args);
		}
	private:
		struct BasicEventArguments
		{
			std::unique_ptr<CreateArgs> CreateArguments = std::make_unique<CreateArgs>();

		};
		BasicEventArguments eventArguments;
		void SetResources(const PublicResource& _resources) noexcept;
		void ProcessCreate(HWND _windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
		{
			// do nothing
			OnCreate(this, eventArguments.CreateArguments.get());
		}
		
		HWND Create(const std::wstring& title);
		void Initialize();
		void InitializeRenderTarget()
		{
			GetD2DFactory()->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(windowHandle),
				&renderTarget);
		}
		void InitializeDpi()
		{
			HDC hdc = GetDC(nullptr);
			dpiX = static_cast<float>(GetDeviceCaps(hdc, LOGPIXELSX));
			dpiY = static_cast<float>(GetDeviceCaps(hdc, LOGPIXELSY)) ;
			ReleaseDC(nullptr, hdc);
		}
		
	};
}
