#pragma once

#include <unordered_map>
#include <memory> // for smart pointers
#include <string> //for std::wstring
#include <Windows.h>
#include "Helper.h" // for GetApplicationInstance()
#include "RoutedEvent.h"

namespace Yupei
{
	struct CreateParam
	{
		CreateParam(const std::wstring& className, const std::wstring& windowName,
			DWORD style, DWORD exStyle = 0, HWND parent = nullptr);
		std::wstring ClassName;
		std::wstring WindowName;
		DWORD Style = 0;
		DWORD ExStyle = 0;
		HWND ParentWindow = nullptr;
	};
	class NativeWindow : public std::enable_shared_from_this<NativeWindow>
	{
	public:
		NativeWindow(std::unique_ptr<CreateParam> _param);
		NativeWindow(HWND windowHandle)
			:controlHandle(windowHandle)
		{
			Initialize();
		}
		void SetPosX(int x)
		{
			posX = x;
		}
		void SetPosY(int y)
		{
			posY = y;
		}
		void SetWidth(int width)
		{
			windowWidth = width;
		}
		void SetHeight(int height)
		{
			windowHeight = height;
		}
		void SetLeftCorner(int x, int y)
		{
			SetPosX(x);
			SetPosY(y);
		}
		void SetRect(int x, int y, int width, int height)
		{
			SetLeftCorner(x, y);
			SetWidth(width);
			SetHeight(height);
		}
		HWND CreateHandle()
		{
			return ::CreateWindowEx(
				param->ExStyle,
				param->ClassName.c_str(),
				param->WindowName.c_str(),
				param->Style,
				posX,
				posY,
				windowWidth,
				windowHeight,
				param->ParentWindow,
				reinterpret_cast<HMENU>(NativeWindow::globalID),
				GetApplicationInstance(),
				nullptr
				);
		}
		HWND GetHandle()
		{
			// lazy 
			/*if (controlHandle == nullptr)
			{
				
			}*/
			return controlHandle;
		}
		void Show(int showMode = SW_SHOWNORMAL)
		{
			::ShowWindow(GetHandle(), showMode);
		}
		template<typename Type>
		Type SetAttribute(int index, Type value)
		{
			return reinterpret_cast<Type>(::SetWindowLongPtr(GetHandle(), index, reinterpret_cast<LONG_PTR>(value)));
		}
		template<typename Type>
		Type GetAttribute(int index)
		{
			return reinterpret_cast<Type>(::GetWindowLongPtr(controlHandle, index));
		}
		void SendMess(UINT message, WPARAM wParam, LPARAM lParam)
		{
			::SendMessage(GetHandle(), message, wParam, lParam);
		}
		void AddToTable()
		{
			NativeWindow::idToWindowMap.insert(std::make_pair(NativeWindow::globalID, shared_from_this()));
			++NativeWindow::globalID;
		}
		static std::unordered_map<int, std::shared_ptr<NativeWindow>> idToWindowMap;
		virtual void ProcessMessage(WPARAM wParam, LPARAM lParam) = 0;
		
	private:
		
		HWND controlHandle = nullptr;
		int posX = CW_USEDEFAULT;
		int posY = CW_USEDEFAULT;
		int windowWidth = CW_USEDEFAULT;
		int windowHeight = CW_USEDEFAULT;
		std::unique_ptr<CreateParam> param;
		static int globalID;

		void Initialize();
		
		
	};
}
