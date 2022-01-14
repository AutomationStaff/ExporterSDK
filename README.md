# ExporterSDK


## Automation Export DLL Example Project

This is a simple, bare-bones project to be used as a starting point for creating custom export plugins for Automation.
It simply outputs all textures to image files, and all meshes (without transforms) as .obj files.
To use an export plugin, a user needs to place the DLL in this directory: [AutomationGame install directory]/Content/ExportPlugins
An export plugin should include both 64 bit and 32 bit versions.


## Required files:

Two header files are required for an export plugin. These are also used by the AutomationGame project, so do not change them!:

  * AuCarExport.h		Contains all the data structures passed between Automation and the export plugin.
  * AuCarExportDLL.h	Contains declarations of all the DLL entry points the export plugin needs to have.

## Export order:

The following functions are called as soon as the DLL is loaded in order to display the appropriate UI to the user:

  * GetExporterName()
  * GetExporterVersion()
  * GetRequiredStringDataCount()
  * GetRequiredStringData()
  * GetRequiredBoolDataCount()
  * GetRequiredBoolData()

Once the export begins, export functions are called in this order:

  * BeginExport()

  * GetLUAFileLength()
  * GetLUAFile()
  * AddLuaFiles()
  * AddLuaFloatData()
  * AddLuaStringData()

  * SetStampTexture()

  * AddBodyMesh()
  * AddCameraPositions()

  * AddChassis()
  * AddWheelPair()	//called twice, for front and rear
  * AddFixtureMeshes()	//called per fixture (mirrored fixtures export each side seperately)

  * AddEngineMesh()	//called per engine mesh
  * AddEngineMeshInstance()	//called per mesh instance

  * AddExhaust()	//called per exhaust line (duel exhausts or headers will result in multiple exhaust lines)

  * GetPreviewImageData()
  * GetPreviewTransformData()
  * AddPreviewImage()

  * EndExport()
  * IsExportInProgress()	//called each game update after EndExport(), until it reports nothing still in progress
  * FreeAllData()


## DLL interface:

All the DLL entry points are declared in AuCarExportDLL.h. All data passed between the DLL and Automation is allocated and managed 
on the Automation side, and all meshes, materials and textures will remain resident in memory throughout the export process, until 
FreeAllData() is called.


## Texture data:

Some texture data within Automation needs to be pulled from the GPU buffers, which is a threaded operation. This means that data 
contained inside an AuCarExpTexture object may not be valid immediately. Automation will wait for all texture data to be valid 
before calling EndExport(). Export plugins should not perform operations based on texture data untill EndExport() is called.


## Stamp map:

The stamp map is the texture used in Automation for managing fixture cut-outs and layering. Each layer is stored in one of the R, 
G, B or A channels of the texture. The data within each channel is a transparency rather than an opacity map (ie. 0 is fully 
opaque and 255 is fully transparent). The stamp map aligns with the second UV coordinates of the car body and fixture meshes.


## Lua/Car Data:

See the Wiki: https://wiki.automationgame.com/index.php?title=Exporter_Plugin_SDK


## Transform and position information:

In exported data, the Y axis is vertical, with positive in the up direction. The Z axis runs down the length of the car, with positive 
forward. X is positive to the left and negative to the right (yes, I know that's backward, sorry).

  +    X    -

  +  [front]
  Z  [ car ]
  -  [rear ]
