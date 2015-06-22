#include "ToolbarCtrl.h"
#include "ImageCapture.h"
#include "..\BasicLibrary\Application.h"
#include <memory>

ToolbarCtrl::ToolbarCtrl(std::shared_ptr<Toolbar> _toolbar)
	:toolbar(std::move(_toolbar)),
	snippingWindow(std::make_shared<SnippingWindow>())
{
}

void ToolbarCtrl::OnNewClicked(void * sender, ToolbarMouseArgs * args)
{
	auto bitmap = ImageCapture::GetFullscreenPic(snippingWindow->GetWindowHandle());
	auto wicbitmap = std::make_unique<Yupei::Picture>(std::move(bitmap));
	snippingWindow->SetScreenshot(wicbitmap->GetD2DBitmap
		(snippingWindow->GetRenderTarget()));
	//::InvalidateRect(GetWindowHandle(), nullptr, TRUE);
}
