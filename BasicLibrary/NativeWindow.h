#pragma once

#include <unordered_map>
#include <memory> // for smart pointers
#include <string> //for std::wstring
#include <Windows.h>
#include "RoutedEvent.h"
#include "ResourceManager.h"
#include "CreateParam.h"

namespace Yupei
{
	
	class NativeWindow
	{
	public:
		NativeWindow(ParamType _param);
		void CreateHandle();
		HWND GetHandle()
		{
			// lazy 
			/*if (controlHandle == nullptr)
			{
				CreateHandle();
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
		void AddToTable(std::shared_ptr<NativeWindow> window)
		{
			NativeWindow::idToWindowMap.insert(std::make_pair(NativeWindow::globalID, std::move(window)));
			++NativeWindow::globalID;
		}
		
		static std::unordered_map<int, std::shared_ptr<NativeWindow>> idToWindowMap;
		virtual void ProcessMessage(WPARAM wParam, LPARAM lParam) = 0;
	private:
		HWND controlHandle = nullptr;
		ParamType param = nullptr;
		static int globalID;
		int myID;
		//CComPtr<ID2D1HwndRenderTarget> renderTarget;
		//PublicResource resources;
		//float dpiX, dpiY;
		void Initialize();
	};
}
