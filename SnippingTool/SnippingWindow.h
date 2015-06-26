#pragma once

#include "..\BasicLibrary\WindowBase.h"
#include <memory>

class ToolbarCtrl;

class SnippingWindow : public Yupei::WindowBase,public std::enable_shared_from_this<SnippingWindow>
{
public:
	static std::shared_ptr<SnippingWindow> GetInstance(HWND _parentWindow)
	{
		auto sp = std::shared_ptr<SnippingWindow>(new SnippingWindow(_parentWindow));
		WindowBase::AddWindow(sp.get());
		
		return sp;
	}
	void OnRender() override;
	void ProcessMessage(HWND hwnd, UINT umsg, WPARAM, LPARAM lParam, bool& wasHandled, LRESULT& result) override
	{
		switch (umsg)
		{
		case WM_SETFOCUS:
			wasHandled = true;
			result = 0;
			break;
		case WM_ACTIVATE:
			wasHandled = true;
			result = 0;
			break;
		case WM_SIZE:
			/*wasHandled = true;
			result = 0;*/
			if (!isResizable)
			{
				Show(SW_SHOWNA);
				//wasHandled = true;
				//result = 0;
			}
			break;
		default:
			break;
		}
	}
	void SetScreenshot(CComPtr<ID2D1Bitmap> shot)
	{
		screenshot = std::move(shot);
	}
	/*void SetIsSnipping(bool _isSnipp)
	{
		if (!_isSnipp) HideWindow();
		else Show();
		isSnipping = _isSnipp;
	}
	bool GetIsSnipping() const
	{
		return isSnipping;
	}*/
	void SetIsResizable(bool _isre)
	{
		isResizable = _isre;
	}
	void SetCtrl(std::shared_ptr<ToolbarCtrl> _ctrl)
	{
		toolbarCtrl = std::move(_ctrl);
		AddEvents();
	}
private:
	void AddEvents();
	SnippingWindow(HWND _parent);
	bool isResizable = false;
	std::shared_ptr<ToolbarCtrl> toolbarCtrl;
	CComPtr<ID2D1Bitmap> screenshot;
	UINT resolutionX, resolutionY;
	CComPtr<ID2D1Layer> topLayer;
	CComPtr<ID2D1Layer> shownLayer;
	CComPtr<ID2D1SolidColorBrush> whiteBrush;
};
