#pragma once

#include "resource.h"
#include "..\BasicLibrary\WindowBase.h"

class Toolbar;

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

	void InitializeToolbar();
	
	HMENU mainMenuHandle = nullptr;
	std::shared_ptr<Toolbar> toolbar;

	int toolbarCommands[3] = { NewCommand,CancelCommand,SettingsCommand };
};				 

