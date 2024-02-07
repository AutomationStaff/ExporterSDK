/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#include "stdafx.h"
#include <Shlobj.h>


size_t FindDirDelimiter(std::wstring dir, size_t start)
{
	size_t slashPos = dir.find(L"\\", start);

	if (slashPos == std::wstring::npos)
	{
		return dir.find(L"/", start);
	}

	return slashPos;
}




AuExpManager* AuExpManager::s_Instance = nullptr;

AuExpManager::AuExpManager()
{
}

AuCarExpErrorCode AuExpManager::Init(const AuCarExpCarData* carData)
{
	
	GetExportDirectory(m_ExportDirectory);

	m_ExportDirectory += L"\\";
	m_ExportDirectory += carData->GetCarName();//TODO: sanitise filename

	//ensure target directory exists:
	DWORD att = GetFileAttributes(m_ExportDirectory.c_str());

	if (att == INVALID_FILE_ATTRIBUTES)
	{
		//create directory, one level at a time:
		size_t slashPos = FindDirDelimiter(m_ExportDirectory, 0);
		size_t offset = 0;

		while (slashPos != std::wstring::npos)
		{
			CreateDirectory(m_ExportDirectory.substr(offset, slashPos - offset).c_str(), nullptr);
			slashPos = FindDirDelimiter(m_ExportDirectory, slashPos + 1);
		}

		//last one:
		CreateDirectory(m_ExportDirectory.c_str(), nullptr);

		att = GetFileAttributes(m_ExportDirectory.c_str());
	}

	if (att != INVALID_FILE_ATTRIBUTES && att & FILE_ATTRIBUTE_DIRECTORY)
	{
		//directory exists, all good to go:
		return AuCarExpErrorCode_Success;
	}
	else
	{
		std::wstring error = L"Could not create directory: ";
		error += m_ExportDirectory;

		MessageBox(nullptr, error.c_str(), TEXT("Error creating directory"), MB_OK);

		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}
}


void AuExpManager::AddImage(const AuCarExpTexture* texture)
{
	if (!texture)
	{
		return;
	}

	//only add unique textures
	for (unsigned int i = 0; i < m_AllImages.size(); i++)
	{
		if (m_AllImages[i] == texture)
		{
			return;
		}
	}

	m_AllImages.push_back(texture);
}

void AuExpManager::SaveImages()
{
	for (unsigned int i = 0; i < m_AllImages.size(); i++)
	{
		AuExpImage::SaveImageFile(m_AllImages[i], m_ExportDirectory.c_str());
	}
}

void AuExpManager::SaveMesh(const AuCarExpMesh* mesh, const wchar_t* name)
{
	if (!mesh)
	{
		return;
	}

	std::wstring filename = m_ExportDirectory + L"\\";
	filename += name;

	if (m_MeshNameCounts.find(name) != m_MeshNameCounts.end())
	{
		m_MeshNameCounts[name]++;

		wchar_t buf[16];
		_itow_s(m_MeshNameCounts[name], buf, 10);

		filename += L"_";
		filename += buf;
	}
	else
	{
		m_MeshNameCounts[name] = 0;
	}

	filename += L".obj";

	AuExpMesh::SaveMeshFile(mesh, filename.c_str());
}

AuCarExpErrorCode AuExpManager::GetExportDirectory(std::wstring& ExportDirectory) const
{
	TCHAR path[MAX_PATH];

	//get the user's documents directory:
	if (SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, SHGFP_TYPE_CURRENT, path) == S_OK)
	{
		ExportDirectory = path;
		ExportDirectory += L"\\Automation Export Example";
	}

	else
	{
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}
	
	return AuCarExpErrorCode_Success;
}
