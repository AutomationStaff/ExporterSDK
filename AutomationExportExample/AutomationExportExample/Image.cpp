/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
************************************************** */


#include "stdafx.h"

#include "DDSHeader.h"

#include <objidl.h>
#include <gdiplus.h>
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib,"gdiplus.lib")






void AuExpImage::SaveImageFile(const AuCarExpTexture* texture, const wchar_t* dir)
{
	std::wstring filename = dir;
	filename += L"\\";

	filename += texture->GetName();

	switch (texture->GetFormat())
	{
	//case AuCarExpPixelFormat_Unknown:
	case AuCarExpPixelFormat_A8R8G8B8:
	case AuCarExpPixelFormat_X8R8G8B8:
	case AuCarExpPixelFormat_A8B8G8R8:
	case AuCarExpPixelFormat_X8B8G8R8:
	case AuCarExpPixelFormat_GreyScale:
		filename += L".png";
		SaveBufferToPNG(filename.c_str(), texture->GetPixelBuffer(), texture->GetWidth(), texture->GetHeight(), texture->GetPitch());
		break;

	case AuCarExpPixelFormat_DXT1:
	case AuCarExpPixelFormat_DXT2:
	case AuCarExpPixelFormat_DXT3:
	case AuCarExpPixelFormat_DXT4:
	case AuCarExpPixelFormat_DXT5:
	case AuCarExpPixelFormat_BC4:
	case AuCarExpPixelFormat_BC5:
		filename += L".dds";
		SaveBufferToDDS(filename.c_str(), texture);
		break;
	}
}




int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void AuExpImage::SaveBufferToPNG(const wchar_t* fullFilename, const void* srcBuffer, unsigned int width, unsigned int height, unsigned int pitch)
{
	// initialize gdiplus
	Gdiplus::GdiplusStartupInput si;
	ULONG_PTR token;
	Gdiplus::GdiplusStartup(&token, &si, 0);

	//file format information:
	CLSID clsid;
	GetEncoderClsid(L"image/png", &clsid);

	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, width, height);

	bitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	unsigned int* destPixels = (unsigned int*)bitmapData.Scan0;

	if (pitch == width)
	{
		//greyscale texture:
		unsigned char* srcPixels = (unsigned char*)srcBuffer;

		for (int y = 0; y < (int)height; y++)
		{
			for (int x = 0; x < (int)width; x++)
			{
				unsigned char pixel = srcPixels[y * width + x];
				destPixels[y * bitmapData.Stride / 4 + x] = ((pixel << 24) | (pixel << 16) | (pixel << 8) | (pixel));
			}
		}
	}
	else
	{
		unsigned int* srcPixels = (unsigned int*)srcBuffer;

		for (int y = 0; y < (int)height; y++)
		{
			for (int x = 0; x < (int)width; x++)
			{
				destPixels[y * bitmapData.Stride / 4 + x] = srcPixels[y * pitch / 4 + x];
			}
		}
	}

	bitmap->UnlockBits(&bitmapData);

	//save to file:
	Gdiplus::Status saveStatus = bitmap->Save(fullFilename, &clsid, NULL);

	if (saveStatus != Gdiplus::Ok)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen(fullFilename) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			fullFilename, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}


	delete bitmap;

	Gdiplus::GdiplusShutdown(token);
}

void AuExpImage::SaveBufferToDDS(const wchar_t* fullFilename, const AuCarExpTexture* texture)
{
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fullFilename, L"wb");

	if (file)
	{
		//file header:
		DDSHeader header(texture);

		fwrite(&header, sizeof(DDSHeader), 1, file);

		if (header.NeedsExtendedHeader())
		{
			DDSHeaderExtended headerExtended(texture);
			fwrite(&headerExtended, sizeof(DDSHeaderExtended), 1, file);
		}


		//pixel data:
		fwrite(texture->GetPixelBuffer(), texture->GetBufferSize(), 1, file);

		//additional mips:
		for (unsigned int i = 0; i < texture->GetAdditionalMipCount(); i++)
		{
			if (texture->GetAdditionalMipMap(i))
			{
				fwrite(texture->GetAdditionalMipMap(i)->GetPixelBuffer(), texture->GetAdditionalMipMap(i)->GetBufferSize(), 1, file);
			}
		}

		fclose(file);
	}
}