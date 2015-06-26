#include "NativeWindow.h"
#include "CreateParam.h"
#include "Helper.h"
#include <utility> // for std::move
#include <cassert>
#include <CommCtrl.h>

namespace Yupei
{
	int NativeWindow::globalID = 0;
	std::unordered_map<int, std::shared_ptr<NativeWindow>> NativeWindow::idToWindowMap{};
	
	NativeWindow::NativeWindow(ParamType _param)
		:param(std::move(_param))
	{
		Initialize();
	}
	void NativeWindow::Initialize()
	{
		CreateHandle();

		assert(controlHandle != nullptr);
	}
	void NativeWindow::CreateHandle()
	{
		controlHandle = ::CreateWindow(
			//TOOLBARCLASSNAME
			param->ClassName.c_str(),
			param->WindowName.c_str(),
			param->Style,
			param->PosX,
			param->PosY,
			param->WindowWidth,
			param->WindowHeight,
			param->ParentWindow,
			reinterpret_cast<HMENU>(NativeWindow::globalID),
			GetApplicationInstance(),
			nullptr
			);
	}
}


