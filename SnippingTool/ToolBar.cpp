#include "ToolBar.h"

void Toolbar::AddButtons(const std::vector<TBBUTTON>& buttons)
{
	::SendMessage(toolbarHandle, TB_BUTTONSTRUCTSIZE,
		static_cast<WPARAM>(sizeof(TBBUTTON)), 0);
	::SendMessage(toolbarHandle, TB_ADDBUTTONS, static_cast<WPARAM>(buttons.size()), reinterpret_cast<LPARAM>(&buttons[0]));
	::SendMessage(toolbarHandle, TB_AUTOSIZE, 0, 0);
}

void Toolbar::AddImages(const std::vector<INT_PTR>& images, int width, int height, const COLORREF& colorFrom)
{
	imageList = ::ImageList_Create(
		width, height,
		ILC_ORIGINALSIZE | ILC_COLOR32 | ILC_MASK,
		static_cast<int>(images.size()), 0);
	::SendMessage(toolbarHandle, TB_SETIMAGELIST,
		static_cast<WPARAM>(0),
		reinterpret_cast<LPARAM>(imageList));
	for (auto&& img : images)
	{
		AddImage(img, colorFrom);
	}
}

void Toolbar::AddImage(INT_PTR index, const COLORREF& colorFrom)
{
	DWORD backgroundColor = GetSysColor(COLOR_BTNFACE);
	COLORMAP colorMap;
	colorMap.from = colorFrom;
	colorMap.to = backgroundColor;
	HBITMAP hbm = CreateMappedBitmap(Yupei::GetApplicationInstance(), index, 0, &colorMap, 1);
	TBADDBITMAP tb;
	//Handle to the module instance with the executable file that contains a bitmap resource. 
	//To use bitmap handles instead of resource IDs, set this member to NULL. 
	tb.hInst = nullptr;
	tb.nID = reinterpret_cast<UINT_PTR>(hbm);
	ImageList_AddMasked(imageList, hbm, RGB(255,255,255));
	::SendMessage(toolbarHandle, TB_ADDBITMAP,
		static_cast<WPARAM>(0),
		reinterpret_cast<LPARAM>(&tb));
	DeleteObject(hbm);
}
