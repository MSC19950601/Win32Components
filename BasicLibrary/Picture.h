#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <d2d1.h>
#include <wincodec.h>
#include <string>
#include <vector>

namespace Yupei
{
	class Picture
	{
	public:
		Picture() = default;
		explicit Picture(int id);//load from resource
		explicit Picture(const std::wstring& fileName);//load from file
		explicit Picture(std::vector<unsigned char> buffer);
		Picture(HBITMAP gdiBitmap); // load from GDI Bitmap
		CComPtr<ID2D1Bitmap> GetD2DBitmap(ID2D1RenderTarget* target,UINT frameID = 0) const;
		CComPtr<ID2D1Bitmap> GetClippedD2DBitmap(ID2D1RenderTarget* target,const WICRect& rect, UINT frameID = 0) const;
		std::pair<BYTE*,UINT> GetBitmapData(const WICRect& rect);
		std::pair<BYTE*, UINT> GetAllData();
		std::pair<UINT, UINT> GetPixels() const
		{
			UINT width, height;
			bitmap->GetSize(&width, &height);
			return std::make_pair(width, height);
		}
		void InitializeWithBuffer(IWICImagingFactory* factory,
			unsigned char* _buffer,
			std::size_t count);
		static void SaveBitmapToFile(const std::wstring& path,
			IWICBitmap* bitmap);
	private:
		static GUID PictureTypeParser(const std::wstring& path);
		void FillWithFrames(IWICBitmapDecoder* decoder);
		CComPtr<IWICBitmap> bitmap;
		std::vector<CComPtr<IWICBitmapFrameDecode>> frameDecoders;
		std::vector<unsigned char> bitmapData;
	};
}