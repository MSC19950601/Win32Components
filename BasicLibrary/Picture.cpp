#include "Picture.h"
#include "Helper.h"
#include "ResourceManager.h"
#include <cctype>
#include <cassert>
namespace Yupei
{
	Picture::Picture(int id)
	{
		auto imageResHandle = ::FindResource(nullptr, MAKEINTRESOURCE(id), MAKEINTRESOURCE(id));
		auto dataHandle = ::LoadResource(GetApplicationInstance(), imageResHandle);
		void* imageFile = ::LockResource(dataHandle);
		auto imageFileSize = ::SizeofResource(GetApplicationInstance(), imageResHandle);
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		InitializeWithBuffer(wicFactory, static_cast<unsigned char*>(imageFile), imageFileSize);
	}
	Picture::Picture(HBITMAP gdiBitmap)
	{
		IWICStream* stream = nullptr;
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		wicFactory->CreateBitmapFromHBITMAP(
			gdiBitmap,
			nullptr,
			WICBitmapIgnoreAlpha,
			&bitmap);
		auto data = GetAllData();
		InitializeWithBuffer(wicFactory, data.first, data.second);
	}
	std::pair<BYTE*, UINT> Picture::GetBitmapData(const WICRect& rect)
	{
		IWICBitmapLock* lock = nullptr;
		bitmap->Lock(&rect, 
			WICBitmapLockRead | WICBitmapLockWrite,
			&lock);
		BYTE* dataPtr = nullptr;
		UINT size = 0;
		lock->GetDataPointer(&size, &dataPtr);
		lock->Release();
		return std::make_pair(dataPtr, size);
	}
	std::pair<BYTE*, UINT> Picture::GetAllData()
	{
		auto size = GetPixels();
		WICRect rect = { 0,0,static_cast<INT>(size.first),static_cast<INT>(size.second) };
		return GetBitmapData(rect);
	}
	void Picture::FillWithFrames(IWICBitmapDecoder* decoder)
	{
		UINT count;
		decoder->GetFrameCount(&count);
		frameDecoders.resize(count);
		for (UINT i = 0;i < count;++i)
		{
			decoder->GetFrame(i, &frameDecoders[i]);
		}
	}
	Picture::Picture(std::vector<unsigned char> buffer)
	{
		CComPtr<IWICStream> stream ;
		bitmapData = std::move(buffer);
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		InitializeWithBuffer(
			wicFactory,
			&bitmapData[0],
			bitmapData.size());
	}
	Picture::Picture(const std::wstring & fileName)
	{
		//TODO
	}
	CComPtr<ID2D1Bitmap> Yupei::Picture::GetD2DBitmap(ID2D1RenderTarget* target, UINT frameID) const
	{
		CComPtr<ID2D1Bitmap> res;
		CComPtr<IWICFormatConverter> converter;
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		auto frame = frameDecoders[frameID];
		wicFactory->CreateFormatConverter(
			&converter);
		converter->Initialize(
			frame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeCustom);
		target->CreateBitmapFromWicBitmap(converter,
			&res);
		return res;

	}
	CComPtr<ID2D1Bitmap> Picture::GetClippedD2DBitmap(ID2D1RenderTarget * target, const WICRect& rect, UINT frameID) const
	{
		CComPtr<ID2D1Bitmap> res;
		CComPtr<IWICFormatConverter> converter;
		CComPtr<IWICBitmapClipper> clipper;
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		auto frame = frameDecoders[frameID];
		wicFactory->CreateFormatConverter(
			&converter);
		wicFactory->CreateBitmapClipper(&clipper);
		clipper->Initialize(frame, &rect);
		HRESULT hr = converter->Initialize(
			clipper,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeCustom);
		hr = target->CreateBitmapFromWicBitmap(converter,
			&res);
		return res;
	}
	void Picture::InitializeWithBuffer(
		IWICImagingFactory* factory, 
		unsigned char * _buffer, 
		std::size_t count)
	{
		CComPtr<IWICStream> stream;
		CComPtr<IWICBitmapDecoder> decoder;
		factory->CreateStream(
			&stream
			);
		stream->InitializeFromMemory(
			static_cast<BYTE*>(_buffer),
			count);
		factory->CreateDecoderFromStream(
			stream,
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&decoder);
		FillWithFrames(decoder.p);
	}
	void Picture::SaveBitmapToFile(const std::wstring & path, IWICBitmap* bitmap)
	{
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		CComPtr<IWICStream> stream;
		CComPtr<IWICBitmapEncoder> encoder;
		CComPtr<IWICBitmapFrameEncode> frameEncoder;
		CComPtr<ID2D1Bitmap> newBitmap;
		WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
		wicFactory->CreateStream(&stream);
		UINT width, height;
		bitmap->GetSize(&width, &height);
		stream->InitializeFromFilename(path.data(), GENERIC_WRITE);
		HRESULT hr = wicFactory->CreateEncoder(Picture::PictureTypeParser(path),
			nullptr,
			&encoder);
		hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
		hr = encoder->CreateNewFrame(&frameEncoder, nullptr);
		hr = frameEncoder->Initialize(nullptr);
		hr = frameEncoder->SetSize(width, height);
		hr = frameEncoder->SetPixelFormat(&format);
		hr = frameEncoder->WriteSource(bitmap,nullptr);
		hr = frameEncoder->Commit();
		hr = encoder->Commit();
	}
	GUID Picture::PictureTypeParser(const std::wstring & path)
	{
		auto pos = path.find_last_of(L'.');
		auto posfix = std::wstring(path.begin() + pos + 1,
			path.begin()+path.find_first_of(L'\0'));
		for (auto& c : posfix) 
			c = std::tolower(c);
		if (posfix == L"png")
		{
			return GUID_ContainerFormatPng;
		}
		else if (posfix == L"gif")
		{
			return GUID_ContainerFormatGif;
		}
		if (posfix == L"jpeg")
		{
			return GUID_ContainerFormatJpeg;
		}
		if (posfix == L"BMP")
		{
			return GUID_ContainerFormatBmp;
		}
		else assert(0);
		//return GUID_ContainerFormatPng;
	}
}


