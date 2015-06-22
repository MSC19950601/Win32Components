#pragma once

#include "resource.h"
#include "..\BasicLibrary\WindowBase.h"
#include "..\BasicLibrary\Menu.h"

class Toolbar;


class MainWindow : public Yupei::WindowBase,public std::enable_shared_from_this<MainWindow>
{
public:
	MainWindow();

	void OnResize(UINT width, UINT height) override;

	void Initialize();
	void OnRender() override
	{
		if (screenshot != nullptr && screenshot != nullptr)
		{
			
			//GetRenderTarget()->DrawBitmap(screenshot);
		}
	}
	void SetScreenshot(CComPtr<ID2D1Bitmap> shot)
	{
		screenshot = std::move(shot);
	}

private:
	
	static const wchar_t* const windowTitle;
	static const int MainWindowWidth = 316;
	static const int MainWindowHeight = 125;
	static const int ToolBarButtonCount = 3;
	enum ToolbarCommands
	{
		NewCommand = 10000,
		CancelCommand = 10001,
		SettingsCommand = 10002
	};
	enum ToolbarButtonsIndex
	{
		NewIndex,
		CancelIndex,
		SettingsIndex
	};

	
	CComPtr<ID2D1Bitmap> screenshot;

	void InitializeToolbar();
	void InitializeContextMenu();
	
	HMENU mainMenuHandle = nullptr;

	std::shared_ptr<Toolbar> toolbar;
	std::shared_ptr<Yupei::Menu> snippingMenu;

	int toolbarCommands[3] = { NewCommand,CancelCommand,SettingsCommand };
};				 

