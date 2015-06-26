#include "MainWindow.h"
#include "ToolBar.h"
#include "..\BasicLibrary\Menu.h"
#include "..\BasicLibrary\Application.h"
#include "ImageCapture.h"
#include "..\BasicLibrary\Picture.h"
#include "ToolbarCtrl.h"
#include "..\BasicLibrary\CreateParam.h"
#include "resource.h"
#include "SnippingWindow.h"
#include <functional>

const wchar_t* const MainWindow::windowTitle = L"Snipping Tool";

MainWindow::MainWindow()
	:WindowBase(std::make_unique<Yupei::CreateParam>(L"YupeiWindow", windowTitle)),
	toolbar(Toolbar::CreateInstance(this, 
		TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT, TBSTYLE_EX_DRAWDDARROWS)),
	snippingMenu(Yupei::Menu::CreateInstance(IDR_SNIP_MENU)),
	snippingWindow(SnippingWindow::GetInstance(GetWindowHandle())),
	mainMenu(Yupei::Menu::CreateInstance(IDR_MAIN_MENU))
	//toolbarCtrl(std::make_shared<ToolbarCtrl>())
{
	SetFixed();
	/*renderTarget->CreateCompatibleRenderTarget(
		&bitmapRenderTarget);*/
	//SetMainMenu(mainMenu.get());
	
}


void MainWindow::OnResize(UINT width, UINT height)
{
	toolbar->OnResize();
}

void MainWindow::OnRender()
{
	toolbarCtrl->OnMainWindowRender();
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
		std::bind(&ToolbarCtrl::OnNewClicked, 
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2)
		);
	toolbar->MouseUp[ToolbarCommands::CancelCommand].AddHandler(
		std::bind(
			&ToolbarCtrl::OnCancelClicked, 
			toolbarCtrl.get(), 
			std::placeholders::_1,
			std::placeholders::_2));
	Yupei::Menu::OnCommand[ID_ENTIRE].AddHandler(
		std::bind(
			&ToolbarCtrl::OnEntireSelect,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
	Yupei::Menu::OnCommand[ID_FILE_NEWSCREENSHOT].AddHandler(
		std::bind(&ToolbarCtrl::OnMenuNewClicked,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
	Yupei::Menu::OnCommand[ID_FILE_SAVEAS].AddHandler(
		std::bind(&ToolbarCtrl::OnSaveClicked,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
}

std::shared_ptr<MainWindow> MainWindow::GetInstance()
{
	auto sp = std::shared_ptr<MainWindow>(new MainWindow());
	sp->toolbarCtrl = std::make_shared<ToolbarCtrl>(sp,sp->snippingWindow);
	sp->snippingWindow->SetCtrl(sp->toolbarCtrl);
	return sp;
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
