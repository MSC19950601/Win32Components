#pragma once

#include "..\BasicLibrary\Yupei.h"


class Toolbar
{
public:
	Toolbar(Yupei::WindowBase* parent,DWORD styles,DWORD exStyles)
	{
		toolbarHandle = ::CreateWindowEx(
			0,
			TOOLBARCLASSNAME, 
			nullptr,
			WS_CHILD | styles, 
			0, 0, 0, 0,
			parent->GetWindowHandle(), 
			nullptr,
			Yupei::GetApplicationInstance(),
			nullptr);
		assert(toolbarHandle != nullptr);
		::SendMessage(toolbarHandle, TB_SETEXTENDEDSTYLE, 0, static_cast<LPARAM>(exStyles));
	}

	void AddButtons(const std::vector<TBBUTTON>& buttons);
	void AddImages(const std::vector<INT_PTR>& images,int width,int height, const COLORREF& colorFrom);
	void AddImage(INT_PTR index,const COLORREF& colorFrom);
	void OnResize()
	{
		::SendMessage(toolbarHandle, TB_AUTOSIZE, 0, 0);
	}
	void Show()
	{
		::ShowWindow(toolbarHandle, SW_SHOWNORMAL);
	}
private:
	HWND toolbarHandle;
	HIMAGELIST imageList;
};
