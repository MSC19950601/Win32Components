#include "Picture.h"
#include "Helper.h"
#include "ResourceManager.h"

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
		WICRect rect = { 0,0,size.first,size.second };
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
	Picture::Picture(std::vector<unsigned char>&& buffer)
	{
		CComPtr<IWICStream> stream ;
		auto wicFactory = PublicResource::GetGraphicsResource().WicImagingFactory.p;
		InitializeWithBuffer(
			wicFactory,
			&buffer[0],
			buffer.size());
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
}


