#pragma once

#include "RoutedEvent.h"
#include "ResourceManager.h"
#include "CreateParam.h"
#include <unordered_map>
#include <Windows.h>
#include <memory>

namespace Yupei
{
	extern LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam,LPARAM lParam);
	extern VOID CALLBACK TimerProc(HWND _windowHandle, UINT message, UINT_PTR eventID, DWORD _time);
	struct GraphicsResource;
	class Menu;
	class WindowBase
	{
	public:

		friend class ApplicationService;

		friend LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		friend VOID CALLBACK TimerProc(HWND _windowHandle, UINT message, UINT_PTR eventID, DWORD _time);
		WindowBase(ParamType param);
		WindowBase()
			:WindowBase(nullptr)
		{

		}
		HWND GetWindowHandle() const noexcept
		{
			return windowHandle;
		}

		virtual void ProcessMessage(HWND _windowHandle, UINT message, WPARAM wParam, LPARAM lParam, bool& wasHandled, LRESULT& result) {}
		DWORD SetWindowStyle(DWORD style)
		{
			return static_cast<DWORD>(SetWindowInfo(GWL_STYLE, static_cast<LONG_PTR>(style)));
		}
		void SetMainMenu(Menu* _menu);
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

		void ForceRedraw()
		{
			::InvalidateRect(windowHandle, nullptr, TRUE);
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

		void SetForeground()
		{
			::SetForegroundWindow(windowHandle);
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

		template <typename U, typename T>
		U PixelsToDipsX(T x) const noexcept
		{
			return static_cast<U>(static_cast<float>(x) /( dpiX / 96.f));
		}

		template <typename U, typename T>
		U PixelsToDipsY(T y) const noexcept
		{
			return static_cast<U>(static_cast<float>(y) /( dpiY / 96.f));
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
		void Show(int _cmd = SW_SHOWNORMAL);

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

		void SetLeftTop(int x, int y);

		void HideWindow()
		{
			::CloseWindow(windowHandle);
		}

		void SetWindowsStyle(bool b = false)
		{
			if (b == true)
			{
				SetWindowStyle(GetWindowStyle() | WS_SIZEBOX | WS_SYSMENU);
			}
			else
			{
				SetWindowStyle(GetWindowStyle() &~WS_SIZEBOX & ~WS_SYSMENU&~WS_BORDER&~WS_CAPTION);
			}
		}

		void SetFixed(bool isFixed = true)
		{
			if (isFixed)
			{
				SetWindowStyle(GetWindowStyle() & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
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

		ID2D1HwndRenderTarget* GetRenderTarget() const
		{
			return renderTarget;
		}

		enum class AnimateType
		{
			Fade = AW_BLEND,
			CollapseToCenter = AW_CENTER,
			FromLeftToRight = AW_HOR_POSITIVE,
			FromRightToLeft = AW_HOR_NEGATIVE,
			Silde = AW_SLIDE,
			FromTopToBottom = AW_VER_POSITIVE,
			FromBottomToTop = AW_VER_NEGATIVE
		};
		void AnimateShow(DWORD _time, AnimateType _type)
		{
			Animate(_time, _type, true);
		}
		void AnimateHide(DWORD _time, AnimateType _type)
		{
			Animate(_time, _type, false);
		}
		
		void SetCreateParam(ParamType _param);

		void RegisterTimer(DWORD _time,UINT_PTR id)
		{
			::SetTimer(windowHandle, id, _time, TimerProc);
			//
		}
		template<typename Type>
		void AddTimer(UINT_PTR id, Type&& t)
		{
			TimerEvent[id].AddHandler(std::forward<Type>(t));
		}
		void DeleteTimer(UINT_PTR id)
		{
			auto it = TimerEvent.find(id);
			if (it != TimerEvent.end())
			{
				::KillTimer(windowHandle, id);
				TimerEvent.erase(it);
			}
			//TimerEvent.erase(id);
		}
		//Events

		Event<CreateArgs> WinCreate;
		Event<MouseArgs> LeftMouseDown;
		Event<MouseArgs> LeftMouseUp;
		Event<MouseArgs> MouseMove;
		
	protected:
		static WindowBase* GetWindow(HWND _windowHandle)
		{
			return windowsMap[_windowHandle];
		}
		static void AddWindow(WindowBase* _window)
		{
			windowsMap[_window->GetWindowHandle()] = std::move(_window);
		}
		static void RemoveWindow(HWND _windowHandle)
		{
			windowsMap.erase(_windowHandle);
		}
		HWND windowHandle = nullptr;

		PublicResource resources;

		float dpiX, dpiY;

		ID2D1HwndRenderTarget* renderTarget = nullptr;
		virtual void OnCreate(void* sender, CreateArgs* args)
		{
			RoutedInvoke(WinCreate, sender, args);
		}
		virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	private:
		static std::unordered_map<UINT_PTR, Event<RoutedEventArgs>> TimerEvent;
		void Animate(DWORD _time, AnimateType _type, bool isShow)
		{
			DWORD flag = static_cast<DWORD>(_type);
			flag |= (isShow ?AW_ACTIVATE  :AW_HIDE );
			::AnimateWindow(windowHandle, _time, flag);
		}

		struct BasicEventArguments
		{
			std::unique_ptr<CreateArgs> CreateArguments = std::make_unique<CreateArgs>();
			std::unique_ptr<RoutedEventArgs> RoutedArguments = std::make_unique<RoutedEventArgs>();
			std::unique_ptr<MouseArgs> MouseArguments = std::make_unique<MouseArgs>();
		};
		BasicEventArguments eventArguments;
		void SetResources(const PublicResource& _resources) noexcept;
		void ProcessCreate(HWND _windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
		{
			// do nothing
			OnCreate(this, eventArguments.CreateArguments.get());
		}
		void PreProcessButtonMessage(WPARAM wParam, LPARAM lParam);
		
		/*HWND Create(const std::wstring& title);*/
		void Create();
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
		void SubClass(WNDPROC _newProc)
		{
			_oldProc = reinterpret_cast<WNDPROC>(GetWindowInfo(GWLP_WNDPROC));
			SetWindowInfo(GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_newProc));
		}
		
		static std::unordered_map<HWND, WindowBase*> windowsMap;
	
		std::shared_ptr<WindowClass> _windowClass;
		ParamType _createParam = nullptr;
		WNDPROC _oldProc;
	};
}
