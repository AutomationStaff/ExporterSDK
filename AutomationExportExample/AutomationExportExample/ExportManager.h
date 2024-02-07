/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#pragma once

#include "AutomationExportExample.h"

class AuExpManager
{
public:

	static AuExpManager* Instance() { return s_Instance; }
	static void CreateInstance() { if (!s_Instance) s_Instance = new AuExpManager(); }
	static void DestroyInstance() { delete s_Instance; s_Instance = nullptr; }


	AuCarExpErrorCode Init(const AuCarExpCarData* carData);

	//Getting pixel data is a threaded operation within Automation, which means that we should not assume pixel data within these textures is complete until EndExport() is called
	//So what we do is save the reference and save to file at the end of the export
	void AddImage(const AuCarExpTexture* texture);
	void SaveImages();

	void SaveMesh(const AuCarExpMesh* mesh, const wchar_t* name);

	AuCarExpErrorCode GetExportDirectory(std::wstring& ExportDirectory) const;

	std::wstring m_ExportDirectory;
private:

	AuExpManager();
	~AuExpManager() {}

	static AuExpManager* s_Instance;

	std::vector<const AuCarExpTexture*> m_AllImages;

	std::map<std::wstring, int> m_MeshNameCounts;
};


