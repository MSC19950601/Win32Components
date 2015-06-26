#pragma once

#include <memory>
#include "..\BasicLibrary\WindowBase.h"
#include "..\BasicLibrary\Application.h"
#include "MainWindow.h"
#include "SnippingWindow.h"
#include "ImageCapture.h"
#include "..\BasicLibrary\Helper.h"
#include "ToolBar.h"
#include "..\BasicLibrary\FileDialog.h"


class SnippingManager
{
public:
	friend class ToolbarCtrl;
	SnippingManager(std::shared_ptr<MainWindow> window,
		std::shared_ptr<SnippingWindow> _snipping)
		:mainWindow(std::move(window)),
		snippingWindow(std::move(_snipping)),
		fileDialog(std::make_unique<Yupei::FileDialog>(mainWindow.lock()->GetWindowHandle()))
	{
		auto rect = Yupei::DeviceHelper::GetResolution(snippingWindow->GetWindowHandle());
		snippingWindow->SetWindowStyle(WS_POPUPWINDOW);
		resolutionX = Yupei::GetRECTWidth<UINT>(rect);
		resolutionY = Yupei::GetRECTHeight<UINT>(rect);
		snippingWindow->SetClientSizeWithPhysic(resolutionX, resolutionY);
		snippingWindow->SetLeftTop(0, 0);
		snippingWindow->Show(SW_HIDE);
		snippingWindow->SetWindowsStyle();
		fileDialog->SetFiler(L"Í¼Æ¬ÎÄ¼þ\0*.JPEG;*.PNG;*.BMP\0\0");
	}
	void TakeShot();
	void HideSnipping()
	{
		snippingWindow->Show(SW_HIDE);
		//snippingWindow->Close();
	}
	void UnCheckButton()
	{
		auto sp = mainWindow.lock();
		if (sp)
		{
			sp->toolbar->SetChecked(MainWindow::ToolbarCommands::NewCommand, FALSE);
		}
	}
	void SavePicture();
	void SetIsSnipping(bool _issnipp)
	{
		isSnipping = _issnipp;
		snippingWindow->SetIsResizable(isSnipping);
	}
	bool GetIsSnipping() const
	{
		return isSnipping;
	}
	enum class Status
	{
		Entire,
		WindowShot,
		AnyPath,
		Rectangle
	};

	Status NowStatus = Status::Rectangle;
	bool IsSnippingDone = false;
	bool IsHolding = false;
	D2D1_RECT_F RectangleToShot;
private:
	std::unique_ptr<Yupei::Picture> sourcePic;
	
	bool isSnipping = false;
	std::shared_ptr<SnippingWindow> snippingWindow;
	std::weak_ptr<MainWindow> mainWindow;
	UINT resolutionX, resolutionY;
	std::unique_ptr<Yupei::FileDialog> fileDialog;
};