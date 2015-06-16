#include "ToolBar.h"

Toolbar::Toolbar(Yupei::WindowBase * parent, DWORD styles, DWORD exStyles)
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

void Toolbar::AddButtons(const std::vector<TBBUTTON>& buttons)
{
	for (auto&& b : buttons)
	{
		toolbarCommands.push_back(b.idCommand);
	}
	::SendMessage(toolbarHandle, TB_BUTTONSTRUCTSIZE,
		static_cast<WPARAM>(sizeof(TBBUTTON)), 0);
	::SendMessage(toolbarHandle, TB_ADDBUTTONS, static_cast<WPARAM>(buttons.size()), reinterpret_cast<LPARAM>(&buttons[0]));
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

std::pair<UINT, UINT> Toolbar::GetButtonSize(int index)
{
	RECT rect;
	::SendMessage(toolbarHandle, TB_GETRECT, static_cast<WPARAM>(toolbarCommands[index]), reinterpret_cast<LPARAM>(&rect));
	return std::make_pair(static_cast<UINT>(rect.right - rect.left),
		static_cast<UINT>(rect.bottom - rect.top));
}

std::pair<UINT, UINT> Toolbar::GetTotalSize()
{
	SIZE size;
	::SendMessage(toolbarHandle, TB_GETMAXSIZE, 0, reinterpret_cast<LPARAM>(&size));
	return std::make_pair(size.cx, size.cy);
}
