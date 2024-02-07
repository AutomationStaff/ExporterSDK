/************************************************* *
** Copyright(c) 2019 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */


//THIS HEADER IS SHARED WITH THE AUTOMATIONGAME PROJECT - DO NOT MAKE ANY CHANGES!


#pragma once

#include "AuCarExport.h"


//This value is incremented when data structures or interface function definitions change
//Automation will not load export DLLs whose interface version does not match the latest version
#define AUCAREXPORT_DLL_INTERFACE_VERSION 29


#define AUCAREXPORT_MAX_DATA_ITEM_COUNT 16


#ifndef AUCAREXPORT_GAMESIDE

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

#endif


	namespace AuCarExportDLL
	{


#ifdef AUCAREXPORT_GAMESIDE //should always be undefined in an exporter dll

		//Defnitions to be used within Automation
		//Exporter projects do not need to worry about these:
        
	#if PLATFORM_WINDOWS
		#define AUCAREXPORTDLL_FUNCTION(Name, ...)	 typedef AuCarExpErrorCode(__cdecl *Proc ## Name)(__VA_ARGS__);\
															Proc ## Name GetProc ## Name(HMODULE dll) { return (Proc ## Name)GetProcAddress(dll, #Name); }
				bool AuCarExpDLLInterfaceMatches(HMODULE dll) { FARPROC proc = GetProcAddress(dll, "AuCarExpDLLInterface"); return proc ? proc() == AUCAREXPORT_DLL_INTERFACE_VERSION : false;}
	#else
		
	#define AUCAREXPORTDLL_FUNCTION(Name, ...)     typedef AuCarExpErrorCode(__cdecl *Proc ## Name)(__VA_ARGS__);\
						Proc ## Name GetProc ## Name(HMODULE dll) { return nullptr; }
		
		
		bool AuCarExpDLLInterfaceMatches(HMODULE dll) {return false; }
		#endif
#else

        //Definitions to be used by exporter dll projects:
	#define AUCAREXPORTDLL_FUNCTION(Name, ...) __declspec(dllexport) AuCarExpErrorCode __cdecl Name(__VA_ARGS__);
        __declspec(dllexport) long long __cdecl AuCarExpDLLInterface() { return AUCAREXPORT_DLL_INTERFACE_VERSION; }

#endif

		//Functions called outside of export (in order to display information to the user):

		//Gets the plugin name, to be displayed in the drop-down list of available plugins
		AUCAREXPORTDLL_FUNCTION(GetExporterName, AuCarExpArray<wchar_t>& name, wchar_t const* locale);

		//Gets the plugin version number
		//This is for display in the UI only (to help when users report bugs, etc.). Automation does nothing else with this information.
		AUCAREXPORTDLL_FUNCTION(GetExporterVersion, unsigned int* versionNumber);

		//Gets the default export directory
		AUCAREXPORTDLL_FUNCTION(GetExportDirectory, AuCarExpArray<wchar_t>& retDir);

		//Plugin-requested data. Export plugins can request user-supplied information, to be displayed in Automation as either text input fields or checkboxes.

		//Gets the number of user-supplied strings that the plugin will be requesting
		AUCAREXPORTDLL_FUNCTION(GetRequiredStringDataCount, unsigned int* retCount);

		//Gets the user-supplied string information
		AUCAREXPORTDLL_FUNCTION(GetRequiredStringData, AuCarExpArray<AuCarExpUIStringData>& stringData, wchar_t const* locale);

		//Gets the number of user-supplied booleans that the plugin will be requesting
		AUCAREXPORTDLL_FUNCTION(GetRequiredBoolDataCount, unsigned int* retCount);

		//Gets the user-supplied boolean information
		AUCAREXPORTDLL_FUNCTION(GetRequiredBoolData, AuCarExpArray<AuCarExpUIBoolData>& boolData, wchar_t const* locale);


		//Functions called when actually exporting:

		AUCAREXPORTDLL_FUNCTION(BeginExport, const AuCarExpCarData* carData, AuCarExpArray<wchar_t>& retDir, unsigned int* retFlags);

		//Called after all data has been supplied to the export plugin, and all texture information has been finalised
		AUCAREXPORTDLL_FUNCTION(EndExport);

		//Called after EndExport(), to give the plugin the chance to finish any threaded operations that may still be running
		AUCAREXPORTDLL_FUNCTION(IsExportInProgress, bool* retInProgress);

		//Called after the export has finished, and IsExportInProgress() has reported nothing in progress.
		//There will be no further function calls for this export.
		//All outstanding plugin-allocated memory should be cleaned up here
		AUCAREXPORTDLL_FUNCTION(FreeAllData);


		//Car components:

		//Export the car body mesh
		AUCAREXPORTDLL_FUNCTION(AddBodyMesh, AuCarExpMesh* mesh);

		//Export a (single) fixture
		AUCAREXPORTDLL_FUNCTION(AddFixtureMeshes, const AuCarExpArray<AuCarExpMesh*>& meshes, const bool isBreakable, const wchar_t* name, const bool isTowBar, const AuCarExpVector& towPosition);

		//Export an engine mesh
		AUCAREXPORTDLL_FUNCTION(AddEngineMesh, const AuCarExpMesh* mesh, const wchar_t* name);

		//Export an engine mesh whose mesh data is shared with another mesh
		AUCAREXPORTDLL_FUNCTION(AddEngineMeshInstance, const AuCarExpMeshInstance* meshInstance);

		//Export an exhaust line
		//meshes will include piping, catalytic converter, mufflers and exhaust tip fixture (if present)
		AUCAREXPORTDLL_FUNCTION(AddExhaust, const AuCarExpArray<AuCarExpMesh*>& meshes, const AuCarExpArray<AuCarExpVector>& points);

		//Set the driver and bonnet camera positions
		AUCAREXPORTDLL_FUNCTION(AddCameraPositions, const AuCarExpCameraData& driverCam, const AuCarExpCameraData& bonnetCam);

		//Export a pair of wheels (either front or back), including the suspension meshes
		AUCAREXPORTDLL_FUNCTION(AddWheelPair, const AuCarExpWheelData& wheelData, const bool isFront);

		//Export the chassis meshes
		AUCAREXPORTDLL_FUNCTION(AddChassis, const AuCarExpArray<AuCarExpMesh*>& meshes);

		//Export the stamp map
		//See Readme.txt for more information
		AUCAREXPORTDLL_FUNCTION(SetStampTexture, AuCarExpTexture* stampTexture);


		//Car preview image:

		//Get render information required to generate a preview image
		AUCAREXPORTDLL_FUNCTION(GetPreviewImageData, unsigned int* retTextureWidth, unsigned int* retTextureHeight, float* retFOV);

		//Get camera transform information required to generate a preview image
		AUCAREXPORTDLL_FUNCTION(GetPreviewTransformData, const AuCarExpVector* carMin, const AuCarExpVector* carMax, AuCarExpVector* retPosition, AuCarExpVector* retLookAt);

		//Export the generated preview image for the car
		AUCAREXPORTDLL_FUNCTION(AddPreviewImage, AuCarExpTexture* image);


		//LUA data:

		AUCAREXPORTDLL_FUNCTION(AddLuaFloatData, const AuCarExpArray<AuCarExpLuaFloatData>& Data);

		AUCAREXPORTDLL_FUNCTION(AddLuaStringData, const AuCarExpArray<AuCarExpLuaStringData>& Data);

		AUCAREXPORTDLL_FUNCTION(AddLuaFiles, const AuCarExpArray<AuCarLuaDataFile>& Data);

		//
		AUCAREXPORTDLL_FUNCTION(GetLuaFileCount, unsigned int* fileCount);

		AUCAREXPORTDLL_FUNCTION(GetLuaFileLength, unsigned int* retLength, unsigned int FileNum);
		AUCAREXPORTDLL_FUNCTION(GetLuaFile, AuCarExpArray<wchar_t>& stringBuffer, unsigned int FileNum);
	}



#ifndef AUCAREXPORT_GAMESIDE

#ifdef __cplusplus
}
#endif

#endif
