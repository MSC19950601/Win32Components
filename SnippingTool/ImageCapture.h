#pragma once

#include "..\BasicLibrary\Picture.h"

namespace Yupei
{
	class WindowBase;
}

//reference https://msdn.microsoft.com/en-us/library/dd183402

class ImageCapture
{
public:
	//_windowHandle is required for identifing the monitor.
	static std::vector<unsigned char> GetFullscreenPic(HWND _windowHandle);

private:

	static void InitializeBitmapInfoHeader(BITMAPINFOHEADER& bi,const BITMAP& bmp);

	static void InitializeBitmapFileHeader(BITMAPFILEHEADER& bmfHeader, DWORD bitmapSize);

	static DWORD GetBmpSize(const BITMAPINFOHEADER& bi, const BITMAP & bmp)
	{
		return ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
	}

};
