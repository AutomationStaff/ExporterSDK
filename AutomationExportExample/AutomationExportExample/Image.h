/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
************************************************** */

#pragma once

#include "AutomationExportExample.h"

class AuExpImage
{
public:

	static void SaveImageFile(const AuCarExpTexture* texture, const wchar_t* dir);

private:
	static void SaveBufferToPNG(const wchar_t* fullFilename, const void* srcBuffer, unsigned int width, unsigned int height, unsigned int pitch);
	static void SaveBufferToDDS(const wchar_t* fullFilename, const AuCarExpTexture* texture);
};