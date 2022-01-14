/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
************************************************** */



#include "stdafx.h"
#include "DDSHeader.h"

const unsigned int DX10_FOURCC = 0x30315844;

const unsigned int DXT1_FOURCC = 0x31545844;
const unsigned int DXT2_FOURCC = 0x32545844;
const unsigned int DXT3_FOURCC = 0x33545844;
const unsigned int DXT4_FOURCC = 0x34545844;
const unsigned int DXT5_FOURCC = 0x35545844;

const unsigned int BC4_FOURCC = DX10_FOURCC;
const unsigned int BC5_FOURCC = 0x32495441;

DDSHeader::DDSHeader(const AuCarExpTexture* texture) :
	ID(0x20534444),
	Size(0x7C),
	Flags(0x021007),
	Height(texture->GetHeight()),
	Width(texture->GetWidth()),
	PitchOrLinearSize(0),//set for uncompressed
	Depth(0),
	MipMapCount(texture->GetAdditionalMipCount() + 1),
	Reserved2(0)
{

	//set reserved to 0:
	for (int i = 0; i < 11; i++)
	{
		Reserved1[i] = 0;
	}


	//pixelformat:
	FormatSize = 0x20;
	FormatFlags = 0x4;
	FormatFourCC = 0;//uncompressed
	FormatRGBBitCount = 0;
	FormatRBitMask = 0;
	FormatGBitMask = 0;
	FormatBBitMask = 0;
	FormatRGBAlphaBitMask = 0;

	//caps:
	Caps1 = 0x401008;
	Caps2 = 0;
	CapsReserved[0] = 0;
	CapsReserved[1] = 0;

	switch (texture->GetFormat())
	{
	case AuCarExpPixelFormat_DXT1:
		FormatFourCC = DXT1_FOURCC;
		break;
	case AuCarExpPixelFormat_DXT2:
		FormatFourCC = DXT2_FOURCC;
		break;
	case AuCarExpPixelFormat_DXT3:
		FormatFourCC = DXT3_FOURCC;
		break;
	case AuCarExpPixelFormat_DXT4:
		FormatFourCC = DXT4_FOURCC;
		break;
	case AuCarExpPixelFormat_DXT5:
		FormatFourCC = DXT5_FOURCC;
		break;
	case AuCarExpPixelFormat_BC4:
		FormatFourCC = BC4_FOURCC;
		break;
	case AuCarExpPixelFormat_BC5:
		FormatFourCC = BC5_FOURCC;
		break;
	default: //uncompressed dds file:
		FormatFourCC = 0;
		Flags = 0x1007;
		FormatFlags = 0x1 | 0x40;//alpha & rgb
		FormatRGBBitCount = 32;
		FormatRBitMask = 0x00FF0000;
		FormatGBitMask = 0x0000FF00;
		FormatBBitMask = 0x000000FF;
		FormatRGBAlphaBitMask = 0xFF000000;
		break;
	}
}

bool DDSHeader::NeedsExtendedHeader() const
{
	return FormatFourCC == DX10_FOURCC;
}



//Extended header:
DDSHeaderExtended::DDSHeaderExtended(const AuCarExpTexture* texture) :
	Format(27),//R8G8B8A8
	Dimension(3),//2D texture
	MiscFlags(0),
	ArraySize(1),
	MiscFlags2(0)
{
	//format matches DirectX DXGI_FORMAT enum:
	switch (texture->GetFormat())
	{
	case AuCarExpPixelFormat_DXT1:
		Format = 70;
		break;
	case AuCarExpPixelFormat_DXT2:
		Format = 74;
		break;
	case AuCarExpPixelFormat_DXT3:
		Format = 74;
		break;
	case AuCarExpPixelFormat_DXT4:
		Format = 77;
		break;
	case AuCarExpPixelFormat_DXT5:
		Format = 77;
		break;
	case AuCarExpPixelFormat_BC4:
		Format = 80;
		break;
	case AuCarExpPixelFormat_BC5:
		Format = 83;
		break;
	case AuCarExpPixelFormat_GreyScale:
		Format = 60;
		break;
	}
}