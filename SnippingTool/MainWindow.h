#pragma once

#include "..\BasicLibrary\Yupei.h"
#include "ToolBar.h"
#include "resource.h"

class MainWindow : public Yupei::WindowBase
{
public:
	MainWindow();

	void OnResize(UINT width, UINT height) override;


private:
	
	static const wchar_t* const windowTitle;
	static const int MainWindowWidth = 316;
	static const int MainWindowHeight = 125;
	static const int ToolBarButtonCount = 3;
	enum ToolBarCommands
	{
		NewCommand = 10000,
		CancelCommand = 10001,
		SettingsCommand = 10002
	};
	enum ToolBarButtonsIndex
	{
		NewIndex,
		CancelIndex,
		SettingsIndex
	};

	void InitializeToolbar();
	
	HMENU mainMenuHandle = nullptr;
	Toolbar* toolbar;

	int toolbarCommands[3] = { NewCommand,CancelCommand,SettingsCommand };
};				 

