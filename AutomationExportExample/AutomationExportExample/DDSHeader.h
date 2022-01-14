/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
************************************************** */

#pragma once

#include "AutomationExportExample.h"


struct DDSHeader
{
	unsigned int ID;
	unsigned int Size;
	unsigned int Flags;
	unsigned int Height;
	unsigned int Width;
	unsigned int PitchOrLinearSize;
	unsigned int Depth;
	unsigned int MipMapCount;
	unsigned int Reserved1[11];

	//pixel format:
	int FormatSize;
	int FormatFlags;
	int FormatFourCC;
	int FormatRGBBitCount;
	int FormatRBitMask;
	int FormatGBitMask; 
	int FormatBBitMask;
	int FormatRGBAlphaBitMask;

	//caps:
	int Caps1;
	int Caps2;
	int CapsReserved[2];

	//reserved:
	int Reserved2;


	DDSHeader(const AuCarExpTexture* texture);

	bool NeedsExtendedHeader() const;
};



struct DDSHeaderExtended
{
	unsigned int Format;
	unsigned int Dimension;
	unsigned int MiscFlags;
	unsigned int ArraySize;
	unsigned int MiscFlags2;

	DDSHeaderExtended(const AuCarExpTexture* texture);
};