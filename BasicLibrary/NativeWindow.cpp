#include "NativeWindow.h"
#include <utility> // for std::move
#include <cassert>

namespace Yupei
{
	int NativeWindow::globalID = 0;
	std::unordered_map<int, std::shared_ptr<NativeWindow>> NativeWindow::idToWindowMap{};
	CreateParam::CreateParam(const std::wstring& className, const std::wstring& windowName,
		DWORD style, DWORD exStyle, HWND parent)
		:ClassName(className),
		WindowName(windowName),
		Style(style),
		ExStyle(exStyle),
		ParentWindow(parent)
	{
		
	}
	NativeWindow::NativeWindow(std::unique_ptr<CreateParam> _param)
		:param(std::move(_param))
	{
		Initialize();
	}
	void NativeWindow::Initialize()
	{
		controlHandle = CreateHandle();
		assert(controlHandle != nullptr);
	}
}


