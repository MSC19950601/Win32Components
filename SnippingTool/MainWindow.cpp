#include "MainWindow.h"
#include "ToolBar.h"
#include "..\BasicLibrary\Menu.h"
#include "..\BasicLibrary\Application.h"
#include "ImageCapture.h"
#include "..\BasicLibrary\Picture.h"

const wchar_t* const MainWindow::windowTitle = L"Snipping Tool";

MainWindow::MainWindow()
	:WindowBase(windowTitle),
	toolbar(Toolbar::CreateInstance(this, 
		TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT, TBSTYLE_EX_DRAWDDARROWS)),
	snippingMenu(Yupei::Menu::CreateInstance(IDR_SNIP_MENU))
{
	
}


void MainWindow::OnResize(UINT width, UINT height)
{
	toolbar->OnResize();
}

void MainWindow::Initialize()
{
	InitializeContextMenu();
	InitializeToolbar();
	SetWindowStyle(GetWindowStyle() | WS_CLIPCHILDREN);
	toolbar->Show();
	auto size = toolbar->GetTotalSize();
	SetClientSizeWithPhysic(size.first, size.second);
	//SetFixed(true);
	toolbar->MouseUp[ToolbarCommands::NewCommand].AddHandler(
		[this](void*, ToolbarMouseArgs* args)
	{
		//this->SetClientSizeWithPhysic(500,500);
		
		//Yupei::Application::ExitApplication();
	});
}

void MainWindow::InitializeToolbar()
{
	toolbar->AddImages(std::vector<INT_PTR>{IDB_SNIPPING_ICON, IDB_CANCEL_ICON, IDB_SETTINGS_ICON}, 50, 50, RGB(255, 255, 255));
	std::vector<TBBUTTON> buttons =
	{
		{
			2 * ToolbarButtonsIndex::NewIndex,
			ToolbarCommands::NewCommand,
			TBSTATE_ENABLED,
			BTNS_CHECK | BTNS_DROPDOWN,
			{ 0 },
			0,
			reinterpret_cast<INT_PTR>(L"New") 
		},
		{
			2 * ToolbarButtonsIndex::CancelIndex,
			ToolbarCommands::CancelCommand,
			TBSTATE_ENABLED,
			0,
			{ 0 },
			0,
			reinterpret_cast<INT_PTR>(L"Cancel") 
		},
		{
			2 * ToolbarButtonsIndex::SettingsIndex,
			ToolbarCommands::SettingsCommand,
			TBSTATE_ENABLED ,
			0,
			{ 0 },
			0,
			reinterpret_cast<INT_PTR>(L"Settings") 
		}
	};
	toolbar->AddButtons(buttons);
	toolbar->SetDropdownMenu(ToolbarCommands::NewCommand, snippingMenu->GetSubMenu(0));
}

void MainWindow::InitializeContextMenu()
{
	snippingMenu->SetWindow(shared_from_this());
}
