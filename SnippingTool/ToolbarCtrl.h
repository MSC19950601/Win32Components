#pragma once

#include <memory>
#include "MainWindow.h"


class Toolbar;
struct ToolbarMouseArgs;


namespace Yupei
{
	class WindowBase;
}

class ToolbarCtrl
{
public:

	ToolbarCtrl(std::shared_ptr<Toolbar> _toolbar);
	void OnNewClicked(void* sender,ToolbarMouseArgs* args);
	void OnCancelClicked(void* sender, ToolbarMouseArgs* args);
	void OnSettingClicked(void* sender, ToolbarMouseArgs* args);
	void SetView(std::shared_ptr<MainWindow> window)
	{
		mainWindow = std::move(window);
	}
private:
	std::shared_ptr<Toolbar> toolbar;
	std::weak_ptr<MainWindow> mainWindow;
	std::shared_ptr<SnippingWindow> snippingWindow;
};
