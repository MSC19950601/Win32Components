#include "WindowBase.h"
#include "IndependentResource.h"
#include "Helper.h"


namespace Yupei
{
	WindowBase::WindowBase()
		:WindowBase(L"")
	{
		
	}
	WindowBase::WindowBase(const std::wstring & title)
	{
		windowHandle = Create(title);
	}
	void WindowBase::SetWindowSize(UINT width, UINT height)
	{
		auto rect = GetWindowPos();
		MoveWindowTo(rect.left, rect.top, rect.left+static_cast<int>(width), rect.top+static_cast<int>(height));
	}
	void WindowBase::SetClientSize(UINT width, UINT height)
	{
		auto rect = GetWindowPos();
		if (GetWindowStyle() == WS_OVERLAPPED)
		{
			auto captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
			auto borderWidth = ::GetSystemMetrics(SM_CXBORDER);
			width += static_cast<UINT>(borderWidth) << 1;
			height += static_cast<UINT>(captionHeight);
			MoveWindowTo(rect.left, rect.top, rect.left + width, rect.top + height);
		}
		else
		{
			RECT target = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
			//TRUE or FALSE ?
			::AdjustWindowRect(&target, GetWindowStyle(), FALSE);
			MoveWindowTo(rect.left, rect.top, rect.left+static_cast<int>(target.right - target.left), rect.top+static_cast<int>(target.bottom - target.top));
		}
	}
	void WindowBase::SetFixedSize(UINT width, UINT height)
	{
		SetWindowSize(width, height);
		SetWindowStyle(WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
	}
	RECT WindowBase::GetWindowPos() const
	{
		RECT rect;
		::GetWindowRect(windowHandle, &rect);
		return rect;
	}
	void WindowBase::Show()
	{
		::ShowWindow(windowHandle, SW_SHOWNORMAL);
		::UpdateWindow(windowHandle);
	}
	void WindowBase::SetResources(const DeviceIndependentResource * _resources) noexcept
	{
		d2dFactory = _resources->D2DFactory;
		dWriteFactory = _resources->DWriteFactory;
		wicImagingFactory = _resources->WicImagingFactory;
		Initialize();
	}

	WPARAM WindowBase::Run()
	{
		MSG messages;
		while (true)
		{
			if (::PeekMessage(&messages,nullptr,0,0,PM_REMOVE))
			{
				if (messages.message == WM_QUIT) break;
				::TranslateMessage(&messages);
				::DispatchMessage(&messages);
			}
			else
			{
				OnRender();
			}
		}
		return messages.wParam;
	}
	HWND WindowBase::Create(const std::wstring& title)
	{
		static WindowClass wndClass; // register window class once
		return ::CreateWindow(
			WindowClass::DefaultWindowClassName,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			nullptr,
			nullptr,
			GetApplicationInstance(),
			static_cast<void*>(this));
	}
	void WindowBase::Initialize()
	{
		InitializeRenderTarget();
		InitializeDpi();
	}
	LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowBase* mainWindow = nullptr;
		LRESULT result = -1;
		bool wasHandled = false;
		if (message == WM_NCCREATE)
		{
			auto pcs = reinterpret_cast<CREATESTRUCT*>(lParam);
			mainWindow = static_cast<WindowBase*>(pcs->lpCreateParams);
			::SetWindowLongPtr(
				windowHandle,
				GWLP_USERDATA,
				PtrToUlong(mainWindow));
			wasHandled = false;
		}
		else
		{
			mainWindow = reinterpret_cast<WindowBase*>(
				::GetWindowLongPtr(
					windowHandle,
					GWLP_USERDATA));
			if (mainWindow)
			{
				switch (message)
				{
				case WM_PAINT:
				{
					auto renderTarget = mainWindow->renderTarget;
					if (renderTarget)
					{
						renderTarget->BeginDraw();
						renderTarget->Clear(0);
						mainWindow->OnRender();
						HRESULT hr = renderTarget->EndDraw();
						assert(SUCCEEDED(hr));
						//wasHandled = true;
					}
				}
					break;
				case WM_SIZE:
				{
					auto renderTarget = mainWindow->renderTarget;
					if (renderTarget)
					{
						//Resize() uses Physical pixels
						renderTarget->Resize(D2D1::SizeU(
							static_cast<UINT>(LOWORD(lParam)),
							static_cast<UINT>(HIWORD(lParam))));
					}
				}
					break;
				case WM_DESTROY:
					::PostQuitMessage(0);
					break;
				default:
					break;
				}
				mainWindow->ProcessMessage(windowHandle, message, wParam, lParam, wasHandled, result);
			}
		}
		
		if (!wasHandled) return ::DefWindowProc(windowHandle, message, wParam, lParam);
		else return result;
	}

	const wchar_t* const WindowClass::DefaultWindowClassName = L"YupeiWindow";

	WindowClass::WindowClass()
	{
		::ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.lpfnWndProc = &WindowProc;
		windowClass.hInstance = GetApplicationInstance();
		windowClass.lpszClassName = DefaultWindowClassName;
		windowClass.cbWndExtra = sizeof(char*);
		windowClass.style = CS_VREDRAW | CS_HREDRAW;
		::RegisterClassEx(&windowClass);
	}
}



