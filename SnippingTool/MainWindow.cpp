#include "MainWindow.h"

const wchar_t* const MainWindow::windowTitle = L"Snipping Tool";

MainWindow::MainWindow()
	:WindowBase(windowTitle)
{
	InitializeToolbar();
	SetWindowStyle(GetWindowStyle() | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	toolbar->Show();
}


void MainWindow::OnResize(UINT width, UINT height)
{
	toolbar->OnResize();
}

void MainWindow::InitializeToolbar()
{
	toolbar = new Toolbar(this, TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT, TBSTYLE_EX_DRAWDDARROWS);
	toolbar->AddImages(std::vector<INT_PTR>{IDB_SNIPPING_ICON, IDB_CANCEL_ICON, IDB_SETTINGS_ICON}, 50, 50, RGB(255, 255, 255));
	std::vector<TBBUTTON> buttons =
	{
		{2*ToolBarButtonsIndex::NewIndex,
		ToolBarCommands::NewCommand,
		TBSTATE_ENABLED,
		BTNS_CHECK | BTNS_DROPDOWN,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"New") },
		{2*ToolBarButtonsIndex::CancelIndex,
		ToolBarCommands::CancelCommand,TBSTATE_ENABLED,0,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"Cancel") },
		{2*ToolBarButtonsIndex::SettingsIndex,
		ToolBarCommands::SettingsCommand,TBSTATE_ENABLED ,0,{ 0 },0,
		reinterpret_cast<INT_PTR>(L"Settings") }
	};
	toolbar->AddButtons(buttons);
}
