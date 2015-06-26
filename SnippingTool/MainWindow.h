#pragma once

#include "resource.h"
#include "..\BasicLibrary\WindowBase.h"
#include "..\BasicLibrary\Menu.h"
#include <d2d1.h>
#include <wincodec.h>

class Toolbar;
class ToolbarCtrl;
class SnippingWindow;
namespace Yupei
{
	class Picture;
}

class MainWindow : public Yupei::WindowBase,public std::enable_shared_from_this<MainWindow>
{
public:
	friend class SnippingManager;
	friend class ToolbarCtrl;
	//friend class SnippingWindow;
	void OnResize(UINT width, UINT height) override;
	void OnRender() override;
	void Initialize();
	static std::shared_ptr<MainWindow> GetInstance();
	CComPtr<ID2D1Bitmap> bitmapToDraw;
private:
	MainWindow();
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
	void InitializeContextMenu();
	
	std::shared_ptr<Yupei::Menu> mainMenu;
	CComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
	CComPtr<IWICBitmap> tempBitmap;
	CComPtr<ID2D1Bitmap> tempD2DBitmap;
	std::shared_ptr<Toolbar> toolbar;
	std::shared_ptr<Toolbar> afterToolbar;
	std::shared_ptr<Yupei::Menu> snippingMenu;
	std::shared_ptr<ToolbarCtrl> toolbarCtrl;
	std::shared_ptr<SnippingWindow> snippingWindow;
	int toolbarCommands[3] = { NewCommand,CancelCommand,SettingsCommand };
};				 

