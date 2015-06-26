#pragma once

#include <string>
#include <Windows.h>
#include <memory>

namespace Yupei
{

	struct WindowClass
	{
	public:
		static std::shared_ptr<WindowClass> GetInstance(std::wstring _className);
	private:
		explicit WindowClass(std::wstring _classname);
		void Initialize();
		void Register();
		WNDCLASS _windowClass{};
		std::wstring className;
		std::shared_ptr<WindowClass> nextClass;
		static std::shared_ptr<WindowClass> headClass;
	};
	struct CreateParam
	{
		CreateParam(std::wstring className, std::wstring windowName = std::wstring{},
			DWORD style = WS_OVERLAPPEDWINDOW, DWORD exStyle = 0, HWND parent = nullptr);
		std::wstring ClassName;
		std::wstring WindowName;
		DWORD Style = 0;
		DWORD ExStyle = 0;
		HWND ParentWindow = nullptr;
		int PosX = CW_USEDEFAULT;
		int PosY = CW_USEDEFAULT;
		int WindowWidth = CW_USEDEFAULT;
		int WindowHeight = CW_USEDEFAULT;
	};

	using ParamType = std::unique_ptr<CreateParam>;
};
