/************************************************* *
** Copyright(c) 2019 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */


#include "stdafx.h"

#include "..\Include\AuCarExportDLL.h"





//Gets the plugin name, to be displayed in the drop-down list of available plugins
AuCarExpErrorCode AuCarExportDLL::GetExporterName(AuCarExpArray<wchar_t>& name, wchar_t const* locale)
{
	//Locale handling example (Automation default locale is "en-GB")
	//go go Google Translate (don't do this for actual plugin text :) )

	//your prefered strings handling method should go in here

	//Languages supported by Automation at the time of writing this:
	//(looking only at the language and not the region is enough for this example)
	if (wcsncmp(locale, L"zh", 2) == 0) //Chinese
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"导出插件示例");
	}
	else if (wcsncmp(locale, L"fr", 2) == 0) //French
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Exemple de plug-in d'exportation");
	}
	else if (wcsncmp(locale, L"de", 2) == 0) //German
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Plugin - Beispiel exportieren");
	}
	else if (wcsncmp(locale, L"id", 2) == 0) //Indonesian
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Ekspor contoh plugin");
	}
	else if (wcsncmp(locale, L"it", 2) == 0) //Italian
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Esempio di plugin di esportazione");
	}
	else if (wcsncmp(locale, L"ja", 2) == 0) //Japanese
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"プラグインの例をエクスポート");
	}
	else if (wcsncmp(locale, L"pl", 2) == 0) //Polish
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Przykład wtyczki eksportu");
	}
	else if (wcsncmp(locale, L"pt", 2) == 0) //Portugese
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Exemplo de plugin de exportação");
	}
	else if (wcsncmp(locale, L"ru", 2) == 0) //Russian
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Пример плагина экспорта");
	}
	else if (wcsncmp(locale, L"es", 2) == 0) //Spanish
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Ejemplo de complemento de exportación");
	}
	else if (wcsncmp(locale, L"tr", 2) == 0) //Turkish
	{
		wcscpy_s(name.GetData(), name.GetCount(), L"Eklentiyi dışa aktarma örneği");
	}
	else
	{
		//default to English
		wcscpy_s(name.GetData(), name.GetCount(), L"Export plugin example");
	}

	return AuCarExpErrorCode_Success;
}

//Gets the plugin version number
//This is for display in the UI only (to help when users report bugs, etc.). Automation does nothing else with this information.
AuCarExpErrorCode AuCarExportDLL::GetExporterVersion(unsigned int* VersionNumber)
{
	*VersionNumber = 0;

	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetExportDirectory(AuCarExpArray<wchar_t>& retDir)
{
	AuExpManager::CreateInstance();

	//tell Automation which directory the plugin will be exporting files to (so this can be displayed in a message to the user):
	std::wstring ExportDirectory;
	const AuCarExpErrorCode status = AuExpManager::Instance()->GetExportDirectory(ExportDirectory);

	wcscpy_s(retDir.GetData(), retDir.GetCount(), ExportDirectory.c_str());
	

	return status;
}


//Gets the number of user-supplied strings that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringDataCount(unsigned int* retCount)
{
	//we will want to get 2 lots of string information from the user:
	*retCount = 2;

	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied string information
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringData(AuCarExpArray<AuCarExpUIStringData>& stringData, wchar_t const* locale)
{
	if (stringData.GetCount() != 2)
	{
		//Automation has not given us the number of items we asked for
		//(this should never happen)
		return AuCarExpErrorCode_UnknownError;
	}

	//set the values:
	wcscpy_s(stringData[0].Label, L"Some Label");//label
	wcscpy_s(stringData[0].Value, L"Default value");//default value

	wcscpy_s(stringData[1].Label, L"/w wildcard defaults");//label
	wcscpy_s(stringData[1].Value, L"[PlayerName] [PlatformName] [TrimName]");//default value, containing wildcards to be filled with information from Automation

	return AuCarExpErrorCode_Success;
}

//Gets the number of user-supplied booleans that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolDataCount(unsigned int* retCount)
{
	//we will want to get 1 boolean from the user:
	*retCount = 1;

	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied boolean information
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData, wchar_t const* locale)
{
	if (boolData.GetCount() != 1)
	{
		//Automation has not given us the number of items we asked for
		//(this should never happen)
		return AuCarExpErrorCode_UnknownError;
	}

	//set the value:
	wcscpy_s(boolData[0].Label, L"Question?");//label
	boolData[0].Value = false;//default value

	return AuCarExpErrorCode_Success;
}


AuCarExpErrorCode AuCarExportDLL::BeginExport(const AuCarExpCarData* carData, AuCarExpArray<wchar_t>& retDir, unsigned int* retFlags)
{
	AuExpManager::CreateInstance();
	AuCarExpErrorCode error = AuExpManager::Instance()->Init(carData);

	//tell Automation which directory the plugin will be exporting files to (so this can be displayed in a message to the user):
	std::wstring ExportDirectory;
	AuExpManager::Instance()->GetExportDirectory(ExportDirectory);

	wcscpy_s(retDir.GetData(), retDir.GetCount(), ExportDirectory.c_str());
	

	//set the flags to none:
	*retFlags = AuCarExpExporterFlags_None;

	return error;
}

//Called after all data has been supplied to the export plugin, and all texture information has been finalised
AuCarExpErrorCode AuCarExportDLL::EndExport()
{
	//texture information is all good now, we can save the image files:
	AuExpManager::Instance()->SaveImages();

	return AuCarExpErrorCode_Success;
}

//Called after EndExport(), to give the plugin the chance to finish any threaded operations that may still be running
AuCarExpErrorCode AuCarExportDLL::IsExportInProgress(bool* retInProgress)
{
	*retInProgress = false;//set to true if there are still threads running

	return AuCarExpErrorCode_Success;
}

//Called after the export has finished, and IsExportInProgress() has reported nothing in progress.
//There will be no further function calls for this export.
//All outstanding plugin-allocated memory should be cleaned up here
AuCarExpErrorCode AuCarExportDLL::FreeAllData()
{
	//de-allocate everything:
	AuExpManager::DestroyInstance();

	return AuCarExpErrorCode_Success;
}


//Export the car body mesh
AuCarExpErrorCode AuCarExportDLL::AddBodyMesh(AuCarExpMesh* mesh)
{
	AuExpManager::Instance()->SaveMesh(mesh, L"car_body");

	return AuCarExpErrorCode_Success;
}

//Export a (single) fixture
AuCarExpErrorCode AuCarExportDLL::AddFixtureMeshes(const AuCarExpArray<AuCarExpMesh*>& meshes, bool isBreakable, const wchar_t* name, const bool isTowBar, const AuCarExpVector& towPosition)
{
	for (unsigned int i = 0; i < meshes.GetCount(); i++)
	{
		AuExpManager::Instance()->SaveMesh(meshes[i], L"fixture");
	}

	return AuCarExpErrorCode_Success;
}

//Export an engine mesh
AuCarExpErrorCode  AuCarExportDLL::AddEngineMesh(const AuCarExpMesh* mesh, const wchar_t* name)
{
	AuExpManager::Instance()->SaveMesh(mesh, L"engine");

	return AuCarExpErrorCode_Success;
}

//Export an engine mesh whose mesh data is shared with another mesh
AuCarExpErrorCode  AuCarExportDLL::AddEngineMeshInstance(const AuCarExpMeshInstance* meshInstance)
{
	return AuCarExpErrorCode_Success;
}

//Export an exhaust line
//meshes will include piping, catalytic converter, mufflers and exhaust tip fixture (if present)
AuCarExpErrorCode  AuCarExportDLL::AddExhaust(const AuCarExpArray<AuCarExpMesh*>& meshes, const AuCarExpArray<AuCarExpVector>& points)
{
	for (unsigned int i = 0; i < meshes.GetCount(); i++)
	{
		AuExpManager::Instance()->SaveMesh(meshes[i], L"exhaust");
	}

	return AuCarExpErrorCode_Success;
}

//Set the driver and bonnet camera positions
AuCarExpErrorCode  AuCarExportDLL::AddCameraPositions(const AuCarExpCameraData& driverCam, const AuCarExpCameraData& bonnetCam)
{
	return AuCarExpErrorCode_Success;
}

//Export a pair of wheels (either front or back), including the suspension meshes
AuCarExpErrorCode  AuCarExportDLL::AddWheelPair(const AuCarExpWheelData& wheelData, const bool isFront)
{
	AuExpManager::Instance()->SaveMesh(wheelData.SuspensionMesh, isFront ? L"suspension_front" : L"suspension_");
	AuExpManager::Instance()->SaveMesh(wheelData.RimMesh, isFront ? L"rim_front" : L"rim_rear");
	AuExpManager::Instance()->SaveMesh(wheelData.TyreMesh, isFront ? L"tyre_front" : L"tyre_rear");

	for (int i = 0; i < 3; i++)
	{
		AuExpManager::Instance()->SaveMesh(wheelData.BrakeMeshes[i], isFront ? L"brake_front" : L"brake_rear");
	}

	return AuCarExpErrorCode_Success;
}

//Export the chassis meshes
AuCarExpErrorCode  AuCarExportDLL::AddChassis(const AuCarExpArray<AuCarExpMesh*>& meshes)
{
	for (unsigned int i = 0; i < meshes.GetCount(); i++)
	{
		AuExpManager::Instance()->SaveMesh(meshes[i], L"chassis");
	}

	return AuCarExpErrorCode_Success;
}

//Export the stamp map
//See Readme.txt for more information
AuCarExpErrorCode AuCarExportDLL::SetStampTexture(AuCarExpTexture* stampTexture)
{
	AuExpManager::Instance()->AddImage(stampTexture);

	return AuCarExpErrorCode_Success;
}




//Get render information required to generate a preview image
AuCarExpErrorCode AuCarExportDLL::GetPreviewImageData(unsigned int* retTextureWidth, unsigned int* retTextureHeight, float* retFOV)
{
	*retTextureWidth = 500;//required image width in pixels
	*retTextureHeight = 281;//required image height in pixels
	*retFOV = 30.0f;//required field of view

	return AuCarExpErrorCode_Success;
}

//Get camera transform information required to generate a preview image
AuCarExpErrorCode AuCarExportDLL::GetPreviewTransformData(const AuCarExpVector* carMin, const AuCarExpVector* carMax, AuCarExpVector* retPosition, AuCarExpVector* retLookAt)
{
	float carLength = carMax->z - carMin->z;
	float carHeight = carMax->y - carMin->y;

	retLookAt->x = (carMin->x + carMax->x) * 0.5f;
	retLookAt->y = (carMin->y + carMax->y) * 0.5f;
	retLookAt->z = (carMin->z + carMax->z) * 0.5f;

	retLookAt->z += 0.05f * carLength;
	retLookAt->y -= 0.02f * carHeight;

	*retPosition = *retLookAt;

	retPosition->x -= 1.65f * carLength;
	retPosition->z += 0.5f * carLength * 2.25f;

	retPosition->y += 0.3f * carHeight * 1.12f;

	return AuCarExpErrorCode_Success;
}

//Export the generated preview image for the car
AuCarExpErrorCode AuCarExportDLL::AddPreviewImage(AuCarExpTexture* image)
{
	AuExpManager::Instance()->AddImage(image);

	return AuCarExpErrorCode_Success;
}




AuCarExpErrorCode AuCarExportDLL::AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data)
{
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data)
{
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& Data)
{
	return AuCarExpErrorCode_Success;
}



AuCarExpErrorCode AuCarExportDLL::GetLuaFileCount(unsigned int* fileCount)
{
	//IOHelper::WriteLog("AuCarExportDLL::GetLuaFileCount");
	*fileCount = 1;

	return AuCarExpErrorCode_Success;
}



AuCarExpErrorCode AuCarExportDLL::GetLuaFileLength(unsigned int* retLength, unsigned int FileNum)
{
	*retLength = 0;

	HRSRC   hRes;              // handle/ptr to res. info.

	HMODULE module = GetModuleHandle(PROJECT_FILENAME);

	hRes = FindResource(module, MAKEINTRESOURCE(IDR_LUA_FILE), TEXT("BINARY"));

	if (!hRes)
	{
		return AuCarExpErrorCode_UnknownError;
	}

	unsigned int size = SizeofResource(module, hRes);

	*retLength = size + 1;//size in chars (what we need) is the byte size. We add one for a null terminator

	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLuaFile(AuCarExpArray<wchar_t>& stringBuffer, unsigned int FileNum)
{
	if (!stringBuffer.GetData())
	{
		return AuCarExpErrorCode_UnknownError;
	}

	HGLOBAL hResourceLoaded;  // handle to loaded resource
	HRSRC   hRes;              // handle/ptr to res. info.

	HMODULE module = GetModuleHandle(PROJECT_FILENAME);

	hRes = FindResource(module, MAKEINTRESOURCE(IDR_LUA_FILE), TEXT("BINARY"));

	if (!hRes)
	{
		return AuCarExpErrorCode_UnknownError;
	}

	unsigned int size = SizeofResource(module, hRes);

	hResourceLoaded = LoadResource(module, hRes);
	char* data = (char*)LockResource(hResourceLoaded);

	if ((size + 1) <= stringBuffer.GetCount())
	{
		for (unsigned int i = 0; i < size; i++)
		{
			stringBuffer[i] = data[i];
		}

		stringBuffer[size] = '\0';
	}

	UnlockResource(hResourceLoaded);

	return AuCarExpErrorCode_Success;
}
