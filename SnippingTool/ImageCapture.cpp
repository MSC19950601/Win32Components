#include "ImageCapture.h"
#include "..\BasicLibrary\WindowBase.h"
#include "..\BasicLibrary\Helper.h"

std::vector<unsigned char> ImageCapture::GetFullscreenPic(HWND _windowHandle)
{
	//take the screenshot.
	std::vector<unsigned char> res;
	HDC screenDC = ::GetDC(nullptr);
	HDC memDC = ::CreateCompatibleDC(screenDC);
	auto screenRect = Yupei::DeviceHelper::GetResolution(_windowHandle);
	int width = static_cast<int>(screenRect.right - screenRect.left);
	int height = static_cast<int>(screenRect.bottom - screenRect.top);
	HBITMAP screenBitmap = ::CreateCompatibleBitmap(screenDC, 
		width,height
		);
	// We finally get what we want!
	HBITMAP oldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(memDC, screenBitmap));
	::BitBlt(
		memDC,
		0, 0,
		width,
		height,
		screenDC,
		0, 0,
		SRCCOPY);
	
	BITMAP bmpScreen;
	::GetObject(screenBitmap, sizeof(BITMAP), &bmpScreen);
	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	ImageCapture::InitializeBitmapInfoHeader(bi, bmpScreen);
	ImageCapture::InitializeBitmapFileHeader(bmfHeader, ImageCapture::GetBmpSize(bi, bmpScreen));

	res.resize(bmfHeader.bfSize);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(
		memDC, 
		screenBitmap, 
		0,
		static_cast<UINT>(bmpScreen.bmHeight),
		&res[0] + bmfHeader.bfOffBits,
		reinterpret_cast<BITMAPINFO*>(&bi), 
		DIB_RGB_COLORS);

	std::memcpy(&res[0], &bmfHeader, sizeof(BITMAPFILEHEADER));
	std::memcpy(&res[0] + sizeof(BITMAPFILEHEADER),&bi , sizeof(BITMAPINFOHEADER));
	screenBitmap = reinterpret_cast<HBITMAP>(::SelectObject(memDC, oldBitmap));
	::DeleteDC(memDC);
	::ReleaseDC(nullptr, screenDC);
	::DeleteObject(screenBitmap);
	return res;
}

void ImageCapture::InitializeBitmapInfoHeader(BITMAPINFOHEADER & bi, const BITMAP& bmp)
{
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmp.bmWidth;
	bi.biHeight = bmp.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
}

void ImageCapture::InitializeBitmapFileHeader(BITMAPFILEHEADER & bmfHeader, DWORD bitmapSize)
{

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = bitmapSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM  
}



