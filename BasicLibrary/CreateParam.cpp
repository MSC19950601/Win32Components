#include "CreateParam.h"
#include "Helper.h"

namespace Yupei
{
	std::shared_ptr<WindowClass> WindowClass::headClass{};
	CreateParam::CreateParam(std::wstring className, std::wstring windowName,
		DWORD style, DWORD exStyle, HWND parent)
		:ClassName(std::move(className)),
		WindowName(std::move(windowName)),
		Style(style),
		ExStyle(exStyle),
		ParentWindow(parent)
	{

	}
	std::shared_ptr<WindowClass> WindowClass::GetInstance(std::wstring _className)
	{
		struct WindowClassWrapper : WindowClass 
		{
			WindowClassWrapper(std::wstring name)
				:WindowClass(std::move(name))
			{

			}
		};
		if (headClass == nullptr)
		{
			headClass = std::make_shared<WindowClassWrapper>(std::move(_className));
			headClass->Register();
			return headClass;
		}
		else
		{
			auto start = headClass;
			while (start != nullptr && start->className != _className)
			{
				start = start->nextClass;
			}
			if (start != nullptr) return start;
			else
			{
				auto sp = std::make_shared<WindowClassWrapper>(std::move(_className));
				sp->Register();
				sp->nextClass = headClass;
				headClass = sp;
				return sp;
			}
		}
	}
	WindowClass::WindowClass(std::wstring _className)
		:className(std::move(_className))
	{
		_windowClass.lpszClassName = className.c_str();
		Initialize();
	}
	void WindowClass::Initialize()
	{
		_windowClass.cbWndExtra = sizeof(char*);
		_windowClass.hInstance = GetApplicationInstance();
		_windowClass.lpfnWndProc = &DefWindowProc;
		_windowClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		_windowClass.style = CS_HREDRAW | CS_VREDRAW;
	}
	void WindowClass::Register()
	{
		::RegisterClass(&_windowClass);
	}
}