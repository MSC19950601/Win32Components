#pragma once

#include "..\BasicLibrary\NativeWindow.h"
#include "..\BasicLibrary\WindowBase.h"
#include <CommCtrl.h>

struct ToolbarRoutedArgs : Yupei::RoutedEventArgs
{
	int CommandID;
};

struct ToolbarMouseArgs : ToolbarRoutedArgs
{
	int PosX, PosY;
};

class Toolbar : public Yupei::NativeWindow
{
public:
	Toolbar(Yupei::WindowBase* parent, DWORD styles, DWORD exStyles);

	void AddButtons(const std::vector<TBBUTTON>& buttons);
	void AddImages(const std::vector<INT_PTR>& images,int width,int height, const COLORREF& colorFrom);
	void AddImage(INT_PTR index,const COLORREF& colorFrom);
	void OnResize()
	{
		SendMess(TB_AUTOSIZE, 0, 0);
	}
	std::pair<UINT, UINT> GetButtonSize(int index);
	std::pair<UINT, UINT> GetTotalSize();
	void Show()
	{
		OnResize();
		NativeWindow::Show();
	}
	virtual void OnClick(WPARAM wParam, LPARAM lParam)
	{
		auto lpnm = reinterpret_cast<LPNMMOUSE>(lParam);
		auto& arg = tempArgs.tempMouseArgs;
		arg->CommandID = lpnm->dwItemSpec;
		arg->PosX = lpnm->pt.x;
		arg->PosY = lpnm->pt.y;
		arg->wasHandled = false;
		auto it = MouseUp.find(arg->CommandID);
		//assert(it != MouseUp.end());
		if (it != MouseUp.end())
		{
			Yupei::RoutedInvoke(it->second, this, arg.get());
		}
	}
	std::unordered_map<int,Yupei::Event<ToolbarMouseArgs>> MouseUp;
	std::unordered_map<int, Yupei::Event<ToolbarMouseArgs>> MouseDown;
private:
	struct TempEventArgs
	{
		std::unique_ptr<ToolbarMouseArgs> tempMouseArgs = std::make_unique<ToolbarMouseArgs>();
	};
	TempEventArgs tempArgs;
	HIMAGELIST imageList;
	std::vector<int> toolbarCommands;
	
	// Inherited via NativeWindow
	virtual void ProcessMessage(WPARAM wParam, LPARAM lParam) override;
};
