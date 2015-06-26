#include "SnippingWindow.h"
#include "..\BasicLibrary\Helper.h"
#include "ToolbarCtrl.h"
#include "SnippingManager.h"

void SnippingWindow::OnRender()
{
	if (screenshot != nullptr)
	{
		auto fullRect = D2D1::RectF(0.f, 0.f, 
			PixelsToDipsX<float>(resolutionX), 
			PixelsToDipsY<float>(resolutionY));
		auto rt = GetRenderTarget();
		rt->DrawBitmap(screenshot, fullRect);
		rt->PushLayer(D2D1::LayerParameters(),
			topLayer);
		rt->FillRectangle(fullRect, whiteBrush);
		rt->PopLayer();
		rt->PushLayer(D2D1::LayerParameters(toolbarCtrl->manager->RectangleToShot), shownLayer);
		rt->DrawBitmap(screenshot, fullRect);
		rt->PopLayer();
	}
}

void SnippingWindow::AddEvents()
{
	LeftMouseUp.AddHandler(
		std::bind(
			&ToolbarCtrl::OnSnippingLeftUp,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
	LeftMouseDown.AddHandler(
		std::bind(
			&ToolbarCtrl::OnSnippingLeftDown,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
	MouseMove.AddHandler(
		std::bind(
			&ToolbarCtrl::OnSnippingMouseMove,
			toolbarCtrl.get(),
			std::placeholders::_1,
			std::placeholders::_2));
}

SnippingWindow::SnippingWindow(HWND _parent)
	:WindowBase()
{
	auto up = std::make_unique<Yupei::CreateParam>(L"YupeiWindow");
	up->ParentWindow = _parent;
	SetCreateParam(std::move(up));
	auto rect = Yupei::DeviceHelper::GetResolution(GetWindowHandle());
	resolutionX = Yupei::GetRECTWidth<UINT>(rect);
	resolutionY = Yupei::GetRECTHeight<UINT>(rect);
	auto rt = GetRenderTarget();
	rt->CreateLayer(&topLayer);
	rt->CreateLayer(&shownLayer);
	rt->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White, 0.5f), &whiteBrush);
	
}
