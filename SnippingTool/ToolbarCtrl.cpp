#include "ToolbarCtrl.h"
#include "ImageCapture.h"
#include "..\BasicLibrary\Application.h"
#include <memory>
#include "SnippingWindow.h"
#include "SnippingManager.h"
#include "..\BasicLibrary\Event.h"
#include "..\BasicLibrary\Helper.h"
#include "MainWindow.h"
#include <wincodec.h>
#include <algorithm>

#undef min
ToolbarCtrl::ToolbarCtrl(std::shared_ptr<MainWindow> _mainWindow,
	std::shared_ptr<SnippingWindow> window)
	:mainWindow(std::move(_mainWindow)),
	snippingWindow(window),
	manager(std::make_shared<SnippingManager>(mainWindow.lock(),std::move(window)))
{
	auto sp = mainWindow.lock();
	auto snip = snippingWindow.lock();
	sp->RegisterTimer(160, 3);
	sp->AddTimer(3, [this, sp = sp.get(),snip = snip.get()](void* sender, Yupei::RoutedEventArgs*)
	{
		sp->ForceRedraw();
		snip->ForceRedraw();
	});
	
}

void ToolbarCtrl::OnNewClicked(void * sender, ToolbarMouseArgs * args)
{
	orignalSize = mainWindow.lock()->toolbar->GetTotalSize();
	auto sp = mainWindow.lock();
	
	sp->SetMainMenu(nullptr);
	manager->RectangleToShot = D2D1::RectF();
	sp->SetClientSizeWithPhysic(orignalSize.first, orignalSize.second);
	sp->AnimateHide(200, Yupei::WindowBase::AnimateType::Fade);
	manager->IsSnippingDone = false;
	sp->RegisterTimer(200, 1);
	
	
	deleteable = false;
	sp->AddTimer(1, [this, sp = sp.get()](void* sender, Yupei::RoutedEventArgs*)
	{
		if (!manager->GetIsSnipping())
		{
			manager->TakeShot();
			//bitmapToDraw = manager->sourcePic->GetD2DBitmap(sp->GetRenderTarget());
			manager->SetIsSnipping(true);
			sp->AnimateShow(200, Yupei::WindowBase::AnimateType::Fade);
			deleteable = true;
			
		}
	});
	isAdded = true;
	sp->RegisterTimer(300, 2);
	sp->AddTimer(2, [sp = sp.get(),this](void* sender, Yupei::RoutedEventArgs*)
	{
		if (isAdded && deleteable)
		{
			sp->DeleteTimer(1);
			isAdded = false;
		}
		
	});
	if (manager->NowStatus == SnippingManager::Status::Entire)
	{
		manager->HideSnipping();// We are done!
		manager->IsSnippingDone = true;
	}
	/*else
	{
		manager->SetIsSnipping(true);
	}*/
}

void ToolbarCtrl::OnCancelClicked(void * sender, ToolbarMouseArgs * args)
{
	manager->HideSnipping();
	manager->UnCheckButton();
	manager->SetIsSnipping(false);
	if (!manager->GetIsSnipping())
	{
		mainWindow.lock()->toolbar->SetChecked(MainWindow::ToolbarCommands::CancelCommand, FALSE);
	}
}

void ToolbarCtrl::OnRectangleClicked(void * sender, ToolbarMouseArgs * args)
{
	manager->NowStatus = SnippingManager::Status::Rectangle;
}

void ToolbarCtrl::OnEntireSelect(void * sender, Yupei::RoutedEventArgs * args)
{
	manager->NowStatus = SnippingManager::Status::Entire;
}

void ToolbarCtrl::OnMainWindowRender()
{
	if (manager->IsSnippingDone)
	{
		//draw the layer!
		auto sp = mainWindow.lock();
		auto rt = sp->GetRenderTarget();
		auto size = sp->bitmapToDraw->GetSize();
		auto brt = sp->bitmapRenderTarget;
		//rt->PushLayer(D2D1::LayerParameters(), sp->finalLayer);
		rt->DrawBitmap(sp->bitmapToDraw.p,
			D2D1::RectF(
				0.f,
				sp->PixelsToDipsY<float>(orignalSize.second),
				static_cast<float>(size.width),
				static_cast<float>(size.height + sp->PixelsToDipsY<float>(orignalSize.second))));
		brt->BeginDraw();
		brt->Clear();
		brt->DrawBitmap(
			sp->tempD2DBitmap,
			D2D1::RectF(
				0.f,
				sp->PixelsToDipsY<float>(orignalSize.second),
				static_cast<float>(size.width),
				static_cast<float>(size.height + sp->PixelsToDipsY<float>(orignalSize.second))));
		HRESULT hr = brt->EndDraw();
		assert(SUCCEEDED(hr));
	}
}

void ToolbarCtrl::OnSnippingWindowRender()
{
	//if (manager->GetIsSnipping())
	//{
	//	//draw the layer!
	//	auto sp = snippingWindow.lock();
	//	auto rt = sp->GetRenderTarget();
	//	rt->PushLayer(D2D1::LayerParameters(manager->RectangleToShot), sp->finalLayer);
	//	rt->DrawBitmap(bitmapToDraw);
	//	rt->PopLayer();
	//}
}

void ToolbarCtrl::OnSnippingLeftDown(void * sender, Yupei::MouseArgs * arg)
{
	
	if (manager->GetIsSnipping())
	{
		auto sp = mainWindow.lock();
		if (manager->NowStatus == SnippingManager::Status::Rectangle)
		{
			auto& rect = manager->RectangleToShot;
			rect.left = sp->PixelsToDipsX<float>(arg->X);
			rect.top = sp->PixelsToDipsY<float>(arg->Y);
		}
		manager->IsHolding = true;
	}
}

void ToolbarCtrl::OnSnippingLeftUp(void * sender, Yupei::MouseArgs * arg)
{
	if (manager->IsHolding)
	{	
		auto sp = mainWindow.lock();
		manager->UnCheckButton();
		//sp->tempBitmap.Release();
		//auto rt = sp->GetRenderTarget();
		
		auto size = sp->bitmapToDraw->GetPixelSize();
		
		//sp->bitmapRenderTarget.Release();
		//D2D1_RENDER_TARGET_PROPERTIES RTProps = D2D1::RenderTargetProperties();
		//RTProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

		
		if (manager->NowStatus == SnippingManager::Status::Rectangle)
		{
			auto rect = manager->RectangleToShot;
			sp->SetClientSizeWithDip((std::max)(size.width,orignalSize.first),size.height + orignalSize.second);
			snippingWindow.lock()->Show(SW_HIDE);
			sp->SetForeground();
			sp->SetMainMenu(sp->mainMenu.get());
			auto wicFactory = Yupei::PublicResource::GetGraphicsResource().WicImagingFactory;
			auto d2dFactory = Yupei::PublicResource::GetGraphicsResource().D2DFactory.p;
			sp->tempBitmap.Release();
			wicFactory->CreateBitmap(
				sp->DipsToPixelsX<UINT>(Yupei::GetRECTWidth<UINT>(rect)),
				sp->DipsToPixelsY<UINT>(Yupei::GetRECTHeight<UINT>(rect)),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapCacheOnLoad,
				&(sp->tempBitmap));
			sp->bitmapRenderTarget.Release();
			d2dFactory->CreateWicBitmapRenderTarget(
				sp->tempBitmap,
				D2D1::RenderTargetProperties(),
				reinterpret_cast<ID2D1RenderTarget**>(&(sp->bitmapRenderTarget)));
			sp->tempD2DBitmap = manager->sourcePic->GetClippedD2DBitmap(sp->bitmapRenderTarget,
				WICRect{
				sp->DipsToPixelsX<int>(std::min(rect.left,rect.right)),
				sp->DipsToPixelsY<int>(std::min(rect.top,rect.bottom)),
				sp->DipsToPixelsX<int>(std::abs(rect.right - rect.left)),
				sp->DipsToPixelsY<int>(std::abs(rect.bottom - rect.top))
			});
		}
		
		manager->IsSnippingDone = true;
		manager->IsHolding = false;
		manager->isSnipping = false;
	}
}

void ToolbarCtrl::OnSnippingMouseMove(void * sender, Yupei::MouseArgs * arg)
{
	if (manager->IsHolding)
	{
		auto sp = mainWindow.lock();
		auto snip = snippingWindow.lock();
		auto& rect = manager->RectangleToShot;
		rect.right = sp->PixelsToDipsX<float>(arg->X);
		rect.bottom = sp->PixelsToDipsY<float>(arg->Y);
		//Yupei::CorrectRect(rect);
		//sp->DipsToPixelsX
		sp->bitmapToDraw = manager->sourcePic->GetClippedD2DBitmap(sp->GetRenderTarget(),
			WICRect{
			sp->DipsToPixelsX<int>(std::min(rect.left,rect.right)),
			sp->DipsToPixelsY<int>(std::min(rect.top,rect.bottom)),
			sp->DipsToPixelsX<int>(std::abs(rect.right - rect.left)),
			sp->DipsToPixelsY<int>(std::abs(rect.bottom - rect.top))
		});
		
		snip->ForceRedraw();

	}
}

void ToolbarCtrl::OnSaveClicked(void * sender, Yupei::RoutedEventArgs * args)
{
	manager->SavePicture();
}
