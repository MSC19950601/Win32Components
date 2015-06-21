#pragma once

#include "..\BasicLibrary\NativeWindow.h"
#include "..\BasicLibrary\WindowBase.h"
#include <CommCtrl.h>
#include <utility>

struct ToolbarRoutedArgs : Yupei::RoutedEventArgs
{
	int CommandID;
};

struct ToolbarMouseArgs : ToolbarRoutedArgs
{
	int PosX, PosY;
};

namespace Yupei
{
	class Menu;
}
	

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
	RECT GetButtonRect(UINT id)
	{
		RECT rect;
		SendMess(TB_GETRECT, static_cast<WPARAM>(id), reinterpret_cast<LPARAM>(&rect));
		return rect;
	}
	void SetDropdownMenu(UINT id,std::shared_ptr<Yupei::Menu> menu);
	std::pair<UINT, UINT> GetButtonSize(int index);
	std::pair<UINT, UINT> GetTotalSize();
	void Show()
	{
		OnResize();
		NativeWindow::Show();
	}
	void OnClick(WPARAM wParam, LPARAM lParam);
	void OnDropdown(WPARAM wParam, LPARAM lParam);
	template<typename... Args>
	static std::shared_ptr<Toolbar> CreateInstance(Args&&... args)
	{
		auto ptr = std::make_shared<Toolbar>(std::forward<Args>(args)...);
		ptr->AddToTable();
		return ptr;
	}
	std::unordered_map<int,Yupei::Event<ToolbarMouseArgs>> MouseUp;
	std::unordered_map<int, Yupei::Event<ToolbarMouseArgs>> Dropdown;
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


