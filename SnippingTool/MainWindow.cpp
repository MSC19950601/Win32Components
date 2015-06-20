#include "MainWindow.h"
#include "ToolBar.h"

const wchar_t* const MainWindow::windowTitle = L"Snipping Tool";

MainWindow::MainWindow()
:WindowBase(windowTitle),
toolbar(std::make_shared<Toolbar>(
			this, 
			TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT, TBSTYLE_EX_DRAWDDARROWS)
		)
{
	InitializeToolbar();
	SetWindowStyle(GetWindowStyle() | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	toolbar->Show();
	auto size = toolbar->GetTotalSize();
	SetClientSizeWithPhysic(size.first, size.second);
	SetFixed(true);
	toolbar->MouseUp[ToolbarCommands::CancelCommand].AddHandler(
		[this](void*, ToolbarMouseArgs* args)
	{
		::MessageBox(GetWindowHandle(), L"Hello", L"", 0);
	});
}


void MainWindow::OnResize(UINT width, UINT height)
{
	toolbar->OnResize();
}

void MainWindow::InitializeToolbar()
{
	toolbar->AddToTable();
	toolbar->AddImages(std::vector<INT_PTR>{IDB_SNIPPING_ICON, IDB_CANCEL_ICON, IDB_SETTINGS_ICON}, 50, 50, RGB(255, 255, 255));
	std::vector<TBBUTTON> buttons =
	{
		{2*ToolbarButtonsIndex::NewIndex,
		ToolbarCommands::NewCommand,
		TBSTATE_ENABLED,
		BTNS_CHECK | BTNS_DROPDOWN,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"New") },
		{2*ToolbarButtonsIndex::CancelIndex,
		ToolbarCommands::CancelCommand,TBSTATE_ENABLED,0,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"Cancel") },
		{2*ToolbarButtonsIndex::SettingsIndex,
		ToolbarCommands::SettingsCommand,TBSTATE_ENABLED ,0,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"Settings") }
	};
	toolbar->AddButtons(buttons);
}
