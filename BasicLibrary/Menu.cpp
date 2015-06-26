#include "Menu.h"
#include "..\BasicLibrary\Helper.h"
#include "..\BasicLibrary\WindowBase.h"
#include <windowsx.h>

namespace Yupei
{
	std::unordered_map<int, Event<RoutedEventArgs>> Menu::OnCommand{};
	std::vector<std::shared_ptr<Menu>> Menu::contextMenus{};
	std::unordered_map<HMENU, std::shared_ptr<Menu>> Menu::menusMap{};
	std::unique_ptr<RoutedEventArgs> Menu::routedArguments = std::make_unique<RoutedEventArgs>();
	Menu::Menu(int resourceID)
		:Menu(resourceID, RECT{})
	{
		
	}
	Menu::Menu(int resourceID, const RECT & rect)
		: menuHandle(::LoadMenu(GetApplicationInstance(), MAKEINTRESOURCE(resourceID))), menuRegion(rect)
	{
		assert(Menu::menusMap.find(menuHandle) == Menu::menusMap.end());
	}
	MenuItemState Menu::GetItemState(UINT itemID) const
	{
		tempInfo.fMask = MIIM_STATE;
		GetInfo(itemID);
		return static_cast<MenuItemState>(tempInfo.fState);
	}
	std::wstring Menu::GetItemString(UINT itemID) const
	{
		std::wstring res;
		tempInfo.fMask = MIIM_TYPE | MIIM_STRING;
		tempInfo.fType = MFT_STRING;
		tempInfo.dwTypeData = nullptr;
		GetInfo(itemID);
		res.resize(tempInfo.cch + 1);
		tempInfo.dwTypeData = &res[0];
		GetInfo(itemID);
		return res;
	}
	void Menu::SetItemString(UINT itemID, const std::wstring & str)
	{
		tempInfo.fMask = MIIM_TYPE | MIIM_STRING;
		tempInfo.fType = MFT_STRING;
		tempInfo.dwTypeData = const_cast<wchar_t*>(&str[0]);
		SetInfo(itemID);
	}
	
	void Menu::SetSubMenu(UINT itemID, HMENU _subMenu)
	{
		tempInfo.fMask = MIIM_SUBMENU;
		tempInfo.hSubMenu = _subMenu;
		SetInfo(itemID);
	}

	std::shared_ptr<Menu> Menu::GetSubMenu(UINT itemID)
	{
		auto _menuhandle = ::GetSubMenu(menuHandle, itemID);
		auto it = Menu::menusMap.find(_menuhandle);
		if (it == menusMap.end())
		{
			auto ptr = Menu::CreateInstance(*this);
			ptr->menuHandle = _menuhandle;
			return ptr;
		}
		else return it->second;
	}

	

	void Menu::Show(int x, int y)
	{
		::TrackPopupMenuEx(
			menuHandle,
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			x, y, parentWindow.lock()->GetWindowHandle(), nullptr);
	}
	void Menu::DispatchContextMenuMessage(WPARAM wParam, LPARAM lParam)
	{
		POINT point = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
		for (auto&& menu : Menu::contextMenus)
		{
			if (PtInRect(&menu->menuRegion, point)) return menu->Show(point.x, point.y);
		}
	}
}


