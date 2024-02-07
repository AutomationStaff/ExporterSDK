/************************************************* *
** Copyright(c) 2019 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */


//THIS HEADER IS SHARED WITH THE AUTOMATIONGAME PROJECT - DO NOT MAKE ANY CHANGES!


#pragma once

#include <functional>


#define AuCarExpErrorCode_FatalBit (0x1 << 31)

enum AuCarExpErrorCode
{
	AuCarExpErrorCode_Success = 0,

	AuCarExpErrorCode_UnknownError = 0x1 | AuCarExpErrorCode_FatalBit,

	AuCarExpErrorCode_TargetGameNotInstalled = 0x2,
	AuCarExpErrorCode_SteamNotRunning = 0x3,
	AuCarExpErrorCode_CouldNotObtainOutputPath = 0x4,
	AuCarExpErrorCode_FailedToDeleteOrWriteToZip = 0x5 | AuCarExpErrorCode_FatalBit,

	AuCarExpErrorCode_CouldNotObtainOutputPathFatal = 0x6 | AuCarExpErrorCode_FatalBit,
};

//These flags are passed from the export dll back to Automation when export begins
enum AuCarExpExporterFlags
{
	AuCarExpExporterFlags_None					= 0,
	AuCarExpExporterFlags_PreScaleUVs			= 0x1,		//If set, mesh uvs should be scaled/offset according to the material and all material scale/offset information cleared
	AuCarExpExporterFlags_DoNotAtlasTextures	= 0x1 << 1,	//If set, no textures will be combined into an atlas, and AuCarExpMaterial::m_[type]AtlasData members will always be null
};

enum AuCarExpPixelFormat
{
	AuCarExpPixelFormat_Unknown,

	AuCarExpPixelFormat_A8R8G8B8,
	AuCarExpPixelFormat_X8R8G8B8,
	AuCarExpPixelFormat_A8B8G8R8,
	AuCarExpPixelFormat_X8B8G8R8,

	AuCarExpPixelFormat_GreyScale,

	AuCarExpPixelFormat_DXT1,
	AuCarExpPixelFormat_DXT2,
	AuCarExpPixelFormat_DXT3,
	AuCarExpPixelFormat_DXT4,
	AuCarExpPixelFormat_DXT5,
	AuCarExpPixelFormat_BC4,
	AuCarExpPixelFormat_BC5,

	//for those that prefer the newer naming convention:
	AuCarExpPixelFormat_BC1 = AuCarExpPixelFormat_DXT1,
	AuCarExpPixelFormat_BC2 = AuCarExpPixelFormat_DXT3,
	AuCarExpPixelFormat_BC3 = AuCarExpPixelFormat_DXT5,
};

enum AuCarExpPixelChannel
{
	AuCarExpPixelChannel_A,
	AuCarExpPixelChannel_R,
	AuCarExpPixelChannel_G,
	AuCarExpPixelChannel_B
};

enum AuCarExpMaterialType
{
	AuCarExpMaterialType_ClearGlass,
	AuCarExpMaterialType_Other
};

enum AuCarExpLightType
{
	AuCarExpLightType_HeadLight,
	AuCarExpLightType_TailLight,
	AuCarExpLightType_BrakeLight,
	AuCarExpLightType_IndicatorLeft,
	AuCarExpLightType_IndicatorRight,
	AuCarExpLightType_Reverse,
	AuCarExpLightType_Running,

	AuCarExpLightType_Count,
	AuCarExpLightType_None = AuCarExpLightType_Count
};

enum AuCarExpSuspensionType
{
	AuCarExpSuspensionType_DoubleWishbone,
	AuCarExpSuspensionType_MacPhersonStrut,
	AuCarExpSuspensionType_Pushrod,
	AuCarExpSuspensionType_SolidAxle,

	AuCarExpSuspensionType_Multilink,
	AuCarExpSuspensionType_SemiTrailingArm,
	AuCarExpSuspensionType_TorsionBeam,

	AuCarExpSuspensionType_Count
};

enum AuCarExpBrakeMeshType
{
	AuCarExpBrakeMeshType_Hub,
	AuCarExpBrakeMeshType_Disc,
	AuCarExpBrakeMeshType_Caliper,

	AuCarExpBrakeMeshType_Count
};

enum AuCarExpBodyShape
{
	AuCarExpBodyShape_Sedan,
	AuCarExpBodyShape_Wagon,
	AuCarExpBodyShape_Hatchback,
	AuCarExpBodyShape_SUV,
	AuCarExpBodyShape_Truck,
	AuCarExpBodyShape_Van,

	AuCarExpBodyShape_Count
};


//Very simple array wrapper
//Does not check for overruns or manage memory - these things should be managed externally
template<class T> class AuCarExpArray
{
public:

	T* GetData() const { return m_Data; }
	unsigned int GetCount() const { return m_Count; }

	T& operator [](int index) const { return m_Data[index]; }

	AuCarExpArray<T>(T* data, unsigned int count) : m_Data(data), m_Count(count) {}

private:
	T* m_Data = nullptr;
	unsigned int m_Count = 0;
};




struct AuCarExpVector
{
public:

	/*!
	* \brief
	* Union to provide access alternatives to members.
	*/
	union
	{
		/*!
		* \brief
		* The individual values accessable as seperate floats.
		*/
		struct
		{
			/*!
			* \brief
			* The x value.
			*/
			float x;

			/*!
			* \brief
			* The y value.
			*/
			float y;

			/*!
			* \brief
			* The z value.
			*/
			float z;
		};

		/*!
		* \brief
		* The x, y and z values accessable as an array.
		*/
		float value[3];
	};

	inline const float & operator[](int index) const { return value[index]; }
	inline float & operator[](int index) { return value[index]; }

	bool operator==(const AuCarExpVector& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

#define AUCAREXPVECTOR_OPERATOR_SIMP(OP) inline AuCarExpVector operator OP (const AuCarExpVector& other) const { return AuCarExpVector(this->x OP other.x, this->y OP other.y, this->z OP other.z); }
#define AUCAREXPVECTOR_OPERATOR_COMPOUND(OP) inline AuCarExpVector& operator OP (const AuCarExpVector& other) { this->x OP other.x; this->y OP other.y; this->z OP other.z; return *this; }

#define AUCAREXPVECTOR_OPERATOR_FLOAT(OP) inline AuCarExpVector operator OP (float other) const { return AuCarExpVector(this->x OP other, this->y OP other, this->z OP other); }
#define AUCAREXPVECTOR_OPERATOR_FLOAT_COMPOUND(OP) inline AuCarExpVector& operator OP (float other) { this->x OP other; this->y OP other; this->z OP other; return *this; }

	AUCAREXPVECTOR_OPERATOR_SIMP(+);
	AUCAREXPVECTOR_OPERATOR_SIMP(-);
	AUCAREXPVECTOR_OPERATOR_SIMP(*);
	AUCAREXPVECTOR_OPERATOR_SIMP(/ );

	AUCAREXPVECTOR_OPERATOR_COMPOUND(+= );
	AUCAREXPVECTOR_OPERATOR_COMPOUND(-= );
	AUCAREXPVECTOR_OPERATOR_COMPOUND(*= );
	AUCAREXPVECTOR_OPERATOR_COMPOUND(/= );

	AUCAREXPVECTOR_OPERATOR_FLOAT(+);
	AUCAREXPVECTOR_OPERATOR_FLOAT(-);
	AUCAREXPVECTOR_OPERATOR_FLOAT(*);
	AUCAREXPVECTOR_OPERATOR_FLOAT(/ );

	AUCAREXPVECTOR_OPERATOR_FLOAT_COMPOUND(+= );
	AUCAREXPVECTOR_OPERATOR_FLOAT_COMPOUND(-= );
	AUCAREXPVECTOR_OPERATOR_FLOAT_COMPOUND(*= );
	AUCAREXPVECTOR_OPERATOR_FLOAT_COMPOUND(/= );

	inline float DotProduct(const AuCarExpVector& other) const { return this->x * other.x + this->y * other.y + this->z * other.z; }

	inline AuCarExpVector CrossProduct(const AuCarExpVector& other) const
	{
		float _x = this->y * other.z - other.y * this->z;
		float _y = this->z * other.x - other.z * this->x;
		float _z = this->x * other.y - other.x * this->y;
		return AuCarExpVector(_x, _y, _z);
	}

	inline float NodeToVertexDistance(const AuCarExpVector& vertex, float zOffset) const
	{
		return sqrtf((vertex.x * 0.01f + zOffset - this->x) * (vertex.x * 0.01f + zOffset - this->x) +
						(vertex.y * 0.01f + zOffset - this->y) * (vertex.y * 0.01f + zOffset - this->y) +
							(vertex.z * 0.01f + zOffset - this->z) * (vertex.z * 0.01f + zOffset - this->z));
	}

	inline AuCarExpVector operator^(const AuCarExpVector& other) const { return this->CrossProduct(other); }

	AuCarExpVector() : x(0.0f), y(0.0f), z(0.0f) {}
	AuCarExpVector(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}
};


struct AuCarExpVector2
{
public:

	/*!
	* \brief
	* Union to provide access alternatives to members.
	*/
	union
	{
		/*!
		* \brief
		* The individual values accessable as seperate floats.
		*/
		struct
		{
			/*!
			* \brief
			* The x value.
			*/
			float x;

			/*!
			* \brief
			* The y value.
			*/
			float y;
		};

		/*!
		* \brief
		* The x and y values accessable as an array.
		*/
		float value[2];
	};

	const float & operator[](int index) const { return value[index]; }
	float & operator[](int index) { return value[index]; }

	bool operator==(const AuCarExpVector2& other) const
	{
		return x == other.x && y == other.y;
	}

	AuCarExpVector2() : x(0.0f), y(0.0f) {}
	AuCarExpVector2(float xVal, float yVal) : x(xVal), y(yVal) {}
};

struct AuCarExpVertex
{
public:

	AuCarExpVector Position;
	AuCarExpVector2 UVcoords0;
	AuCarExpVector2 UVcoords1;
	AuCarExpVector Normal;
	AuCarExpVector Binormal;
	AuCarExpVector Tangent;
	unsigned int Colour = 0xFFFFFFFF;

	bool operator==(const AuCarExpVertex& other) const
	{
		return Position == other.Position && UVcoords0 == other.UVcoords0 && UVcoords1 == other.UVcoords1
											&& Normal == other.Normal && Binormal == other.Binormal && Tangent == other.Tangent
											&& Colour == other.Colour;
	}
};

const unsigned int AuCarExpMaxAdditionalMipCount = 16;

class AuCarExpTexture
{
public:
	inline AuCarExpPixelFormat GetFormat() const { return m_PixelFormat; }
	inline const TCHAR* GetName() const { return m_Name; }

	//Width and height in pixels:
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }

	inline void* GetPixelBuffer() const { return m_PixelBuffer; }

	//Buffer size and pitch in bytes:
	inline unsigned int GetBufferSize() const { return m_BufferSize; }
	inline int GetPitch() const { return m_Pitch; }


	inline bool GetDoNotUseBlockCompression() const { return m_DoNotUseBlockCompression; }
	inline bool IsUniqueTexture() const { return m_IsUniqueTexture; }
	inline bool IsSRGB() const { return m_IsSRGB; }

	inline unsigned int GetAdditionalMipCount() const { return m_AdditionalMipCount; }
	const AuCarExpTexture* GetAdditionalMipMap(unsigned int index) const { return m_AdditionalMips[index]; }
	AuCarExpTexture* const * GetAdditionalMipMapArray() const { return &m_AdditionalMips[0]; }

protected:

	void* m_PixelBuffer = nullptr;
	unsigned int m_BufferSize = 0;

	unsigned int m_Width = 0;
	unsigned int m_Height = 0;

	AuCarExpPixelFormat m_PixelFormat = AuCarExpPixelFormat_Unknown;
	int m_Pitch = 0;

    TCHAR m_Name[64] = TEXT("");

	bool m_DoNotUseBlockCompression = false;
	bool m_IsUniqueTexture = false;
	bool m_IsSRGB = false;

	unsigned int m_AdditionalMipCount = 0;
	AuCarExpTexture* m_AdditionalMips[AuCarExpMaxAdditionalMipCount];

	unsigned int m_InternalUseOnly = 0;
};

struct AuCarExpTextureAtlasData
{
	AuCarExpTexture* Texture = nullptr;
	AuCarExpVector2 UVOffset;
	AuCarExpVector2 UVScale = AuCarExpVector2(1.0f, 1.0f);
};

struct AuCarExpTextureData
{
	float Multiplier = 1.0f;

	AuCarExpPixelChannel MaskChannel = AuCarExpPixelChannel_A;
	AuCarExpTexture* Texture = nullptr;
	AuCarExpTextureAtlasData* AtlasData = nullptr;

	inline AuCarExpTexture* GetTextureFinal() const { return AtlasData ? AtlasData->Texture : Texture; }
};

enum AuCarExpMaterialFlags
{
	AuCarExpMaterialFlags_None					= 0,
	AuCarExpMaterialFlags_IsPaint				= 0x1,
	AuCarExpMaterialFlags_AlphaBlendEnabled		= 0x1 << 1,
	AuCarExpMaterialFlags_AlphaTestEnabled		= 0x1 << 2,
	AuCarExpMaterialFlags_StampMapHasPriority	= 0x1 << 3,
	AuCarExpMaterialFlags_TwoSided				= 0x1 << 4,
};

class AuCarExpMaterial
{
public:

	inline const TCHAR* GetName() const { return m_Name; }

	inline AuCarExpMaterialType GetMaterialType() const { return m_MaterialType; }

	inline unsigned int GetTint() const { return m_Tint; }
	inline unsigned int GetSecondaryTint() const { return m_SecondaryTint; }
	inline unsigned int GetEmmissiveTint() const { return m_EmmissiveTint; }

	inline bool GetAlphaBlendEnabled() const { return (m_Flags & AuCarExpMaterialFlags_AlphaBlendEnabled) != 0; }
	inline bool GetAlphaTestEnabled() const { return (m_Flags & AuCarExpMaterialFlags_AlphaTestEnabled) != 0; }
	inline bool IsStamped() const { return m_StampMapIndex != -1; }

	//should the stamp map be used for alpha even when another alpha source is present?
	//(for systems that don't support both)
	inline bool StampMapHasPriority() const { return (m_Flags & AuCarExpMaterialFlags_StampMapHasPriority) != 0; }

	inline bool IsPaint() const { return (m_Flags & AuCarExpMaterialFlags_IsPaint) != 0; }
	inline bool IsMainBodyMaterial() const { return m_BodyPaintIndex != -1; }

	inline float GetAlphaCutoff() const { return m_AlphaCutoff; }
	inline float GetDiffuseTextureToColourLerp() const { return m_DiffuseTextureToColourLerp; }

	inline int GetStampMapIndex() const { return m_StampMapIndex; }
	inline int GetBodyPaintIndex() const { return m_BodyPaintIndex; }

	inline const AuCarExpTextureData& GetDiffuseMapData() const { return m_DiffuseMapData; }
	inline const AuCarExpTextureData& GetSecondaryDiffuseMapData() const { return m_SecondaryDiffuseMapData; }
	inline const AuCarExpTextureData& GetNormalMapData() const { return m_NormalMapData; }
	inline const AuCarExpTextureData& GetSpecularMapData() const { return m_SpecularMapData; }
	inline const AuCarExpTextureData& GetMetallicMapData() const { return m_MetallicMapData; }
	inline const AuCarExpTextureData& GetRoughnessMapData() const { return m_RoughnessMapData; }
	inline const AuCarExpTextureData& GetOpacityMapData() const { return m_OpacityMapData; }
	inline const AuCarExpTextureData& GetEmmissiveMapData() const { return m_EmmissiveMapData; }
	inline const AuCarExpTextureData& GetClearCoatMapData() const { return m_ClearCoatMapData; }
	inline const AuCarExpTextureData& GetClearCoatBottomNormalMapData() const { return m_ClearCoatBottomNormalMapData; }
	inline const AuCarExpTextureData& GetAmbientOcclusionMapData() const { return m_AmbientOcclusionMapData; }
	inline const AuCarExpTextureData& GetDetailMapData() const { return m_DetailMapData; }
	inline const AuCarExpTextureData& GetDetailNormalMapData() const { return m_DetailNormalMapData; }

	inline float GetClearCoatRoughness() const { return m_ClearCoatRoughness; }

	inline AuCarExpVector2 GetDetailMapUVScale() const { return m_DetailMapUVScale; }


	inline AuCarExpLightType GetLightType() const { return m_LightType; }
	inline unsigned int GetLightColour() const { return m_LightColour; }

	inline bool IsTwoSided() const { return (m_Flags & AuCarExpMaterialFlags_TwoSided) != 0; }

protected:

	TCHAR m_Name[256] = TEXT("");

	AuCarExpMaterialType m_MaterialType = AuCarExpMaterialType_Other;

	unsigned int m_Flags = 0;

	float m_AlphaCutoff = 1.0f;

	//Will be -1 for an unstamped material:
	int m_StampMapIndex = -1;
	int m_BodyPaintIndex = -1;

	AuCarExpLightType m_LightType = AuCarExpLightType_None;
	unsigned int m_LightColour = 0xFFFFFFFF;

	unsigned int m_Tint = 0xFFFFFFFF;
	unsigned int m_SecondaryTint = 0xFFFFFFFF;
	float m_DiffuseTextureToColourLerp = 0.0f;

	unsigned int m_EmmissiveTint = 0x00000000;

	AuCarExpTextureData m_DiffuseMapData;
	AuCarExpTextureData m_SecondaryDiffuseMapData;

	AuCarExpTextureData m_NormalMapData;
	AuCarExpTextureData m_SpecularMapData;
	AuCarExpTextureData m_MetallicMapData;
	AuCarExpTextureData m_RoughnessMapData;
	AuCarExpTextureData m_OpacityMapData;
	AuCarExpTextureData m_EmmissiveMapData;
	AuCarExpTextureData m_AmbientOcclusionMapData;

	//clear coat
	float m_ClearCoatRoughness = 0.0f;
	AuCarExpTextureData m_ClearCoatMapData;
	AuCarExpTextureData m_ClearCoatBottomNormalMapData;

	//detail maps:
	AuCarExpTextureData m_DetailMapData;
	AuCarExpTextureData m_DetailNormalMapData;
	AuCarExpVector2 m_DetailMapUVScale;
};

const int MAX_INDEX_BUFFER_COUNT = 32;

class AuCarExpMesh
{
public:

	inline const float* GetTransform() const { return m_Transform; }
	inline AuCarExpVector GetRotationEulerDegrees() const { return m_RotationEulerDegrees; }
	inline AuCarExpVector GetScale() const { return m_Scale; }

	inline AuCarExpVertex* GetVertexBuffer() const { return m_VertexBuffer; }
	inline unsigned int GetVertexCount() const { return m_VertexCount; }

	inline unsigned int GetIndexBufferCount() const { return m_IndexBufferCount; }

	inline int* GetIndexBuffer(int bufferIndex) const { return m_IndexBuffer[bufferIndex].Buffer; }
	inline unsigned int GetIndexCount(int bufferIndex) const { return m_IndexBuffer[bufferIndex].IndexCount; }
	inline AuCarExpMaterial* GetMaterial(int bufferIndex) const { return m_IndexBuffer[bufferIndex].Material; }

	inline AuCarExpVector GetMin() const { return m_Min; }
	inline AuCarExpVector GetMax() const { return m_Max; }

	inline AuCarExpVector2 GetUVOffset(int bufferIndex) const { return m_IndexBuffer[bufferIndex].UVOffset; }
	inline AuCarExpVector2 GetUVScale(int bufferIndex) const { return m_IndexBuffer[bufferIndex].UVScale; }

protected:

	struct IndexBuffer
	{
	public:
		int* Buffer = nullptr;
		unsigned int IndexCount = 0;
		AuCarExpMaterial* Material = nullptr;
		AuCarExpVector2 UVOffset;
		AuCarExpVector2 UVScale = AuCarExpVector2(1.0f, 1.0f);
	};

	float m_Transform[16] =	{1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f};

	AuCarExpVector m_RotationEulerDegrees;
	AuCarExpVector m_Scale = AuCarExpVector(1.0f, 1.0f, 1.0f);

	AuCarExpVector m_Min;
	AuCarExpVector m_Max;

	unsigned int m_VertexCount = 0;
	AuCarExpVertex* m_VertexBuffer = nullptr;

	unsigned int m_IndexBufferCount = 0;
	IndexBuffer m_IndexBuffer[MAX_INDEX_BUFFER_COUNT];
};

struct AuCarExpMeshInstance
{
public:

	AuCarExpMesh* BaseMesh = nullptr;

	float Transform[16] =	{1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f};

	AuCarExpVector RotationEulerDegrees;
	AuCarExpVector Scale = AuCarExpVector(1.0f, 1.0f, 1.0f);
};


template<>
struct std::hash<AuCarExpVector>
{
	std::size_t operator()(const AuCarExpVector& vector) const
	{
		size_t outHash = 0;
//#if PLATFORM_WINDOWS || defined(_WINDOWS)
		outHash = std::hash<float>()(vector.x);
		outHash ^= std::hash<float>()(vector.y) << 1;
		outHash >>= 1;
		outHash ^= std::hash<float>()(vector.z) << 1;
//#endif
		return outHash;
	}
};

template<>
struct std::hash<AuCarExpVector2>
{
	std::size_t operator()(const AuCarExpVector2& vector) const
	{
		size_t outHash = 0;
//#if PLATFORM_WINDOWS	 || defined(_WINDOWS)	
		outHash = std::hash<float>()(vector.x);
		outHash ^= std::hash<float>()(vector.y) << 1;
//#endif
		return outHash;
	}
};

template<>
struct std::hash<AuCarExpVertex>
{
	std::size_t operator()(const AuCarExpVertex& vertex) const
	{
		size_t outHash;
		outHash = std::hash<AuCarExpVector>()(vertex.Position);
		outHash ^= std::hash<AuCarExpVector2>()(vertex.UVcoords0) << 1;
		outHash >>= 1;
		outHash ^= std::hash<AuCarExpVector2>()(vertex.UVcoords1) << 1;
		outHash >>= 1;
		outHash ^= std::hash<AuCarExpVector>()(vertex.Normal) << 1;
		outHash >>= 1;
		outHash ^= std::hash<AuCarExpVector>()(vertex.Binormal) << 1;
		outHash >>= 1;
		outHash ^= std::hash<AuCarExpVector>()(vertex.Tangent) << 1;
		outHash >>= 1;
		outHash ^= std::hash<unsigned int>()(vertex.Colour) << 1;

		return outHash;
	}
};

struct AuCarExpUIStringData
{
public:

	static const unsigned int LABEL_STRING_LENGTH = 63;
	static const unsigned int TOOLTIP_STRING_LENGTH = 511;
	static const unsigned int VALUE_STRING_LENGTH = 511;

	TCHAR Label[LABEL_STRING_LENGTH + 1] = TEXT("");
	TCHAR ToolTip[TOOLTIP_STRING_LENGTH + 1] = TEXT("");
	TCHAR Value[VALUE_STRING_LENGTH + 1] = TEXT("");
};

struct AuCarExpUIBoolData
{
public:

	static const unsigned int LABEL_STRING_LENGTH = 63;
	static const unsigned int TOOLTIP_STRING_LENGTH = 511;

	TCHAR Label[LABEL_STRING_LENGTH + 1] = TEXT("");
	TCHAR ToolTip[TOOLTIP_STRING_LENGTH + 1] = TEXT("");
	bool Value = false;
};

class AuCarExpCarData
{
public:

	inline const TCHAR* GetCarName() const { return m_CarName; }

	inline float GetHeightOffset() const { return m_HeightOffset; }

	inline AuCarExpBodyShape GetBodyShape() const { return m_BodyShape; }

	inline const AuCarExpUIStringData* GetStringData(unsigned int index) const { return &m_StringData[index]; }
	inline unsigned int GetStringDataCount() const { return m_StringDataCount; }

	inline const AuCarExpUIBoolData* GetBoolData(unsigned int index) const { return &m_BoolData[index]; }
	inline unsigned int GetBoolDataCount() const { return m_BoolDataCount; }

protected:

	TCHAR m_CarName[512] = TEXT("");

	float m_HeightOffset = 0.0f;

	AuCarExpBodyShape m_BodyShape = AuCarExpBodyShape_Count;

	AuCarExpUIStringData* m_StringData = nullptr;
	unsigned int m_StringDataCount = 0;

	AuCarExpUIBoolData* m_BoolData = nullptr;
	unsigned int m_BoolDataCount = 0;
};

//Class that contains PCM Samples
class AuCarExpSoundSample
{
public:
	AuCarExpSoundSample(const TCHAR* Name, float* Samples, int SampleCount) :
		m_SamplesBuffer(Samples),
		m_SampleCount(SampleCount)
	{
#if PLATFORM_WINDOWS || defined(_WINDOWS)
		wcscpy_s(m_Name, Name);
	#endif
	}

	inline const TCHAR* GetName() const { return m_Name; }
	inline float* GetSamples() const { return m_SamplesBuffer; }
	inline unsigned int GetSampleCount() const { return m_SampleCount; }

	//Gets Sample rate, should be 48000hz by default
	inline unsigned int GetSampleRate() const { return m_SampleRate; }

public:
	float* m_SamplesBuffer = nullptr;
	unsigned int  m_SampleCount = 0;
	unsigned int  m_SampleRate = 44100;

    TCHAR m_Name[64] = TEXT("");
};

struct AuCarExpWheelData
{
public:
	float TyreDiameter = 0.0f;
	float TyreWidth = 0.0f;
	float RimDiameter = 0.0f;
	AuCarExpMesh* TyreMesh = nullptr;
	AuCarExpMesh* RimMesh = nullptr;
	AuCarExpMesh* BrakeMeshes[AuCarExpBrakeMeshType_Count];

	AuCarExpMesh* SuspensionMesh = nullptr;
	bool MirrorSuspensionMesh = false;
	AuCarExpSuspensionType SuspensionType = AuCarExpSuspensionType_Count;

	bool LeftRimIsMirrored = false;
	bool RightRimIsMirrored = false;

	AuCarExpWheelData(float tyreDiameter, float tyreWidth, float rimDiameter, AuCarExpMesh* tyreMesh, AuCarExpMesh* rimMesh) :
    TyreDiameter(tyreDiameter),
		TyreWidth(tyreWidth),
		RimDiameter(rimDiameter),
		TyreMesh(tyreMesh),
		RimMesh(rimMesh)
	{
		for (int i = 0; i < AuCarExpBrakeMeshType_Count; i++)
		{
			BrakeMeshes[i] = nullptr;
		}
	}
};

struct AuCarExpCameraData
{
public:
	AuCarExpVector Position;
	float FoV = 60.0f;
};

struct AuCarExpLuaFloatData
{
public:
	TCHAR ValueName[64] = TEXT("");
	float Value = 0.0f;
};

struct AuCarExpLuaStringData
{
public:
    TCHAR ValueName[64] = TEXT("");
    TCHAR* Buffer = nullptr;
	int BufferSize = 0;
};


struct AuCarLuaDataFile
{
public:
    TCHAR FileName[64] = TEXT("");
	char* Buffer = nullptr;
	int BufferSize = 0;
};
