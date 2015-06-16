#pragma once

#include "..\BasicLibrary\Yupei.h"


class Toolbar
{
public:
	Toolbar(Yupei::WindowBase* parent, DWORD styles, DWORD exStyles);

	void AddButtons(const std::vector<TBBUTTON>& buttons);
	void AddImages(const std::vector<INT_PTR>& images,int width,int height, const COLORREF& colorFrom);
	void AddImage(INT_PTR index,const COLORREF& colorFrom);
	void OnResize()
	{
		::SendMessage(toolbarHandle, TB_AUTOSIZE, 0, 0);
	}
	std::pair<UINT, UINT> GetButtonSize(int index);
	std::pair<UINT, UINT> GetTotalSize();
	void Show()
	{
		OnResize();
		::ShowWindow(toolbarHandle, SW_SHOWNORMAL);
	}
private:
	HWND toolbarHandle;
	HIMAGELIST imageList;
	std::vector<int> toolbarCommands;
};
