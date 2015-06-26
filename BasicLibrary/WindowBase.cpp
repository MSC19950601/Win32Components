#include "WindowBase.h"
#include "IndependentResource.h"
#include "Helper.h"
#include "Application.h"
#include "NativeWindow.h"
#include "CreateParam.h"
#include "Menu.h"
#include <cassert>
#include <windowsx.h>
#undef GetWindowStyle

namespace Yupei
{
	std::unordered_map<HWND, WindowBase*> WindowBase::windowsMap{};
	std::unordered_map<UINT_PTR, Event<RoutedEventArgs>> WindowBase::TimerEvent{};
	WindowBase::WindowBase(ParamType param)
		:_createParam(std::move(param)),
		resources(PublicResource::GetInstance()),
		_windowClass(WindowClass::GetInstance(L"YupeiWindow"))
	{
		Initialize();
	}
	void WindowBase::SetMainMenu(Menu * _menu)
	{
		if(_menu != nullptr)
			::SetMenu(windowHandle, _menu->GetMenuHandle());
		else ::SetMenu(windowHandle, nullptr);
	}
	void WindowBase::SetWindowSizeWithPhysic(UINT width, UINT height)
	{
		auto rect = GetWindowPos();
		MoveWindowToWithPhysics(rect.left, rect.top, static_cast<int>(width), static_cast<int>(height));
	}
	void WindowBase::SetClientSizeWithPhysic(UINT width, UINT height)
	{
		auto rect = GetWindowPos();
		if (GetWindowStyle() == WS_OVERLAPPED)
		{
			auto captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
			auto borderWidth = ::GetSystemMetrics(SM_CXBORDER);
			width += static_cast<UINT>(borderWidth) << 1;
			height += static_cast<UINT>(captionHeight);
			MoveWindowToWithPhysics(rect.left, rect.top, width, height);
		}
		else
		{
			RECT target = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
			//TRUE or FALSE ?
			::AdjustWindowRect(&target, GetWindowStyle(), FALSE);
			MoveWindowToWithPhysics(rect.left, rect.top, static_cast<int>(target.right - target.left), static_cast<int>(target.bottom - target.top));
		}
	}
	RECT WindowBase::GetWindowPos() const
	{
		RECT rect;
		::GetWindowRect(windowHandle, &rect);
		return rect;
	}
	void WindowBase::Show(int _cmd)
	{
		::ShowWindow(windowHandle, _cmd);
		::UpdateWindow(windowHandle);
	}
	void WindowBase::SetLeftTop(int x, int y)
	{
		auto rect = GetWindowPos();
		MoveWindowToWithPhysics(
			x, y, GetRECTWidth<int>(rect), GetRECTHeight<int>(rect));
	}
	void WindowBase::SetCreateParam(ParamType _param)
	{
		assert(_createParam == nullptr);
		_createParam = std::move(_param);
		Initialize();
	}
	void WindowBase::SetResources(const PublicResource& _resources) noexcept
	{
		resources = _resources;
		Initialize();
	}
	
	void WindowBase::PreProcessButtonMessage(WPARAM wParam, LPARAM lParam)
	{
		auto& arg = eventArguments.MouseArguments;
		arg->Reset();
		arg->X = GET_X_LPARAM(lParam);
		arg->Y = GET_Y_LPARAM(lParam);
		arg->IsCtrlDown = !!(wParam & MK_CONTROL);
		arg->IsShiftDown = !!(wParam & MK_SHIFT);
		arg->ButtonClicked = static_cast<MouseArgs::MouseButtons>(wParam);
	}

	void WindowBase::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
	{
		PreProcessButtonMessage(wParam, lParam);
		auto arg = eventArguments.MouseArguments.get();
		RoutedInvoke(LeftMouseDown, this, arg);	
	}
	void WindowBase::OnLeftButtonUp(WPARAM wParam, LPARAM lParam)
	{
		PreProcessButtonMessage(wParam, lParam);
		auto arg = eventArguments.MouseArguments.get();
		RoutedInvoke(LeftMouseUp, this, arg);

	}
	void WindowBase::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		PreProcessButtonMessage(wParam, lParam);
		auto arg = eventArguments.MouseArguments.get();
		RoutedInvoke(MouseMove, this, arg);
	}

	/*HWND WindowBase::Create(const std::wstring& title)
	{
		return Create(title, nullptr,POINT{ CW_USEDEFAULT,CW_USEDEFAULT });
	}*/
	void WindowBase::Create()
	{
		if (_createParam != nullptr)
		{
			
			windowHandle = ::CreateWindowEx(
				_createParam->ExStyle,
				_createParam->ClassName.c_str(),
				_createParam->WindowName.c_str(),
				_createParam->Style,
				_createParam->PosX,
				_createParam->PosY,
				_createParam->WindowWidth,
				_createParam->WindowHeight,
				_createParam->ParentWindow,
				nullptr,
				GetApplicationInstance(),
				static_cast<void*>(this));
			WindowBase::AddWindow(this);
			SubClass(WindowProc);
		}
	}
	void WindowBase::Initialize()
	{
		Create();
		if (windowHandle)
		{
			InitializeRenderTarget();
			InitializeDpi();
		}
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
			WindowBase::AddWindow(mainWindow);
			mainWindow->ProcessMessage(windowHandle, message, wParam, lParam, wasHandled, result);
		}
		else
		{
			mainWindow = WindowBase::GetWindow(windowHandle);/*reinterpret_cast<WindowBase*>(
				::GetWindowLongPtr(
					windowHandle,
					GWLP_USERDATA));*/
			if (mainWindow)
			{
				switch (message)
				{
				case WM_CREATE:
					mainWindow->ProcessCreate(windowHandle, message, wParam, lParam);
					break;
				case WM_PAINT:
				{
					auto renderTarget = mainWindow->renderTarget;
					if (renderTarget)
					{
						renderTarget->BeginDraw();
						renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
						mainWindow->OnRender();
						HRESULT hr = renderTarget->EndDraw();
						//assert(SUCCEEDED(hr));
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
						mainWindow->OnResize(static_cast<UINT>(LOWORD(lParam)),
							static_cast<UINT>(HIWORD(lParam)));
					}
				}
					break;
				case WM_DESTROY:
					::PostQuitMessage(0);
					break;
				case WM_LBUTTONDOWN:
					mainWindow->OnLeftButtonDown(wParam, lParam);
					break;
				case WM_LBUTTONUP:
					mainWindow->OnLeftButtonUp(wParam, lParam);
					break;
				case WM_MOUSEMOVE:
					mainWindow->OnMouseMove(wParam, lParam);
					break;
				case WM_COMMAND:
				{
					switch (HIWORD(wParam))
					{
					case 0:
						Menu::DispatchCommandMessage(wParam, lParam);
						// for Menu
						break;
					case 1:
						// for Accelerator
						break;
					default:
						NativeWindow::idToWindowMap[LOWORD(wParam)]->ProcessMessage(wParam, lParam);
						break;
					}
				}
				break;
				case WM_NOTIFY:
				{
					auto pNmhdr = reinterpret_cast<NMHDR*>(lParam);
					NativeWindow::idToWindowMap[pNmhdr->idFrom]->ProcessMessage(wParam, lParam);
				}
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

	VOID CALLBACK TimerProc(HWND _windowHandle, UINT message, UINT_PTR eventID, DWORD _time)
	{
		auto window = WindowBase::GetWindow(_windowHandle);
		return RoutedInvoke(WindowBase::TimerEvent[eventID], window, window->eventArguments.RoutedArguments.get());
	}

}



