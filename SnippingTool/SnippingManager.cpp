#include "SnippingManager.h"
#include "MainWindow.h"

void SnippingManager::TakeShot()
{
	auto bitmap = ImageCapture::GetFullscreenPic(snippingWindow->GetWindowHandle());
	sourcePic = std::make_unique<Yupei::Picture>(std::move(bitmap));
	snippingWindow->SetScreenshot(sourcePic->GetD2DBitmap
		(snippingWindow->GetRenderTarget()));
	snippingWindow->SetLeftTop(0, 0);
	snippingWindow->Show();
	//snippingWindow->ForceRedraw();
}

void SnippingManager::SavePicture()
{
	auto sp = mainWindow.lock();
	auto path = fileDialog->Show(Yupei::FileDialog::FileDialogType::Save,
		L"²¶»ñ.PNG");
	Yupei::Picture::SaveBitmapToFile(path, sp->tempBitmap);
}
