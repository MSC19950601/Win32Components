#pragma once

#include <memory>
#include "..\BasicLibrary\WindowBase.h"

class SnippingWindow : public Yupei::WindowBase
{
public:
	void OnRender() override
	{
		if (screenshot != nullptr)
		{
			GetRenderTarget()->DrawBitmap(screenshot);
		}
	}
	void SetScreenshot(CComPtr<ID2D1Bitmap> shot)
	{
		screenshot = std::move(shot);
	}
private:
	CComPtr<ID2D1Bitmap> screenshot;
};


class SnippingManager
{
private:
	std::shared_ptr<
};