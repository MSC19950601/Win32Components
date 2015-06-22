#pragma once

#include <Windows.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cassert>
#include "RoutedEvent.h"

namespace Yupei
{
	enum class MenuItemState
	{
		Checked = MFS_CHECKED,
		Default = MFS_DEFAULT,
		Disabled = MFS_DISABLED,
		Enabled = MFS_ENABLED,
		Grayed = MFS_GRAYED,
		Highlight = MFS_HILITE,
		Unchecked = MFS_UNCHECKED,
		Unhighlight = MFS_UNHILITE
	};
	class WindowBase;

	//TODO
	//InsertMenu
	class Menu : public std::enable_shared_from_this<Menu>
	{
	public:
		HMENU GetMenuHandle() const
		{
			return menuHandle;
		}
		Menu()
			:Menu(RECT{})
		{
			
		}
		explicit Menu(int resourceID);
		Menu(const RECT& rect)
			:menuHandle(::CreatePopupMenu()),menuRegion(rect)
		{
			assert(Menu::menusMap.find(menuHandle) == Menu::menusMap.end());
		}
		Menu(int resourceID, const RECT& rect);
		MenuItemState GetItemState(UINT itemID) const;
		std::wstring GetItemString(UINT itemID) const;
		void SetItemString(UINT itemID, const std::wstring& str);
		~Menu()
		{
			::DestroyMenu(menuHandle);
		}
		void SetWindow(std::weak_ptr<WindowBase> window)
		{
			parentWindow = std::move(window);
		}
		void SetSubMenu(UINT itemID, HMENU _subMenu);
		std::shared_ptr<Menu> GetSubMenu(UINT itemID);
		template<typename... Args>
		static std::shared_ptr<Menu> CreateInstance(Args&&... args)
		{
			auto ptr = std::make_shared<Menu>(std::forward<Args>(args)...);
			ptr->AddToTable();
			menusMap.insert(std::make_pair(ptr->menuHandle, ptr));
			return ptr;
		}
		Menu(const Menu& m) = default;
		static std::unordered_map<int, Event<RoutedEventArgs>> OnCommand;
		void AddToTable()
		{
			Menu::contextMenus.push_back(shared_from_this());
		}
		void SetRegion(const RECT& rect)
		{
			menuRegion = rect;
		}
		void Show(int x, int y);
		static void DispatchContextMenuMessage(WPARAM wParam, LPARAM lParam);
	protected:
		HMENU menuHandle;
		std::weak_ptr<WindowBase> parentWindow;
		RECT menuRegion;
		bool isEnabled;
	private:
		mutable MENUITEMINFO tempInfo = { sizeof(MENUITEMINFO) };
		void GetInfo(UINT itemID) const
		{
			::GetMenuItemInfo(
				menuHandle,
				itemID,
				TRUE,
				&tempInfo);
		}
		void SetInfo(UINT itemID)
		{
			::SetMenuItemInfo(
				menuHandle,
				itemID,
				TRUE,
				&tempInfo);
		}
		
		static std::vector<std::shared_ptr<Menu>> contextMenus;
		static std::unordered_map<HMENU, std::shared_ptr<Menu>> menusMap;
	};

}

