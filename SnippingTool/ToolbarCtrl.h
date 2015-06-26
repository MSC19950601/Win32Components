#pragma once

#include <memory>
#include <d2d1.h>
#include <atlbase.h>

class Toolbar;
struct ToolbarMouseArgs;
class SnippingManager;
class MainWindow;

namespace Yupei
{
	class WindowBase;
	struct RoutedEventArgs;
	struct  MouseArgs;
}

class ToolbarCtrl
{
public:
	friend class SnippingWindow;
	ToolbarCtrl(std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<SnippingWindow> window);
	//ToolbarCtrl(std::shared_ptr<Toolbar> _toolbar);
	void OnNewClicked(void* sender,ToolbarMouseArgs* args);
	void OnMenuNewClicked(void* sender, Yupei::RoutedEventArgs* args)
	{
		OnNewClicked(sender, nullptr);
	}
	void OnCancelClicked(void* sender, ToolbarMouseArgs* args);
	void OnRectangleClicked(void* sender, ToolbarMouseArgs* args);
	void OnSettingClicked(void* sender, ToolbarMouseArgs* args);
	void OnEntireSelect(void* sender, Yupei::RoutedEventArgs* args);
	void OnMainWindowRender();
	void OnSnippingWindowRender();
	void OnSnippingLeftDown(void* sender, Yupei::MouseArgs* arg);
	void OnSnippingLeftUp(void* sender, Yupei::MouseArgs* arg);
	void OnSnippingMouseMove(void* sender, Yupei::MouseArgs* arg);
	void OnSaveClicked(void* sender, Yupei::RoutedEventArgs* args);
	void SetView(std::shared_ptr<MainWindow> window)
	{
		mainWindow = std::move(window);
	}
private:
	//std::shared_ptr<Toolbar> toolbar;
	std::weak_ptr<MainWindow> mainWindow;
	std::weak_ptr<SnippingWindow> snippingWindow;
	std::shared_ptr<SnippingManager> manager;
	std::pair<UINT, UINT> orignalSize;
	bool isAdded = false;
	bool deleteable = false;
};
