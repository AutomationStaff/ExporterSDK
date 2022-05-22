/************************************************* *
** Copyright(c) 2019 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */


//THIS HEADER IS SHARED WITH THE AUTOMATIONGAME PROJECT - DO NOT MAKE ANY CHANGES!


#pragma once


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
	AuCarExpMaterialType_Grille,
	AuCarExpMaterialType_ClearGlass,
	AuCarExpMaterialType_NumberPlate,

	AuCarExpMaterialType_Other
};

enum AuCarExpLightType
{
	AuCarExpLightType_HeadLight,
	AuCarExpLightType_TailLight,
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
};

const unsigned int AuCarExpMaxAdditionalMipCount = 16;

class AuCarExpTexture
{
public:
	inline AuCarExpPixelFormat GetFormat() const { return m_PixelFormat; }
	inline const wchar_t* GetName() const { return m_Name; }

	//Width and height in pixels:
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }

	inline void* GetPixelBuffer() const { return m_PixelBuffer; }

	//Buffer size and pitch in bytes:
	inline unsigned int GetBufferSize() const { return m_BufferSize; }
	inline int GetPitch() const { return m_Pitch; }


	inline bool GetDoNotUseBlockCompression() const { return m_DoNotUseBlockCompression; }
	inline bool IsUniqueTexture() const { return m_IsUniqueTexture; }

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

	wchar_t m_Name[64] = L"";

	bool m_DoNotUseBlockCompression = false;
	bool m_IsUniqueTexture = false;

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

	inline AuCarExpMaterialType GetMaterialType() const { return m_MaterialType; }

	inline unsigned int GetTint() const { return m_Tint; }
	inline unsigned int GetSecondaryTint() const { return m_SecondaryTint; }

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

	//Paint parameters, use only if IsPaint() is true:
	inline float GetPearlStrength() const { return m_PearlStrength; }
	inline float GetFlakeStrength() const { return m_FlakeStrength; }


	inline AuCarExpLightType GetLightType() const { return m_LightType; }
	inline unsigned int GetLightColour() const { return m_LightColour; }

	inline bool IsTwoSided() const { return (m_Flags & AuCarExpMaterialFlags_TwoSided) != 0; }

protected:

	AuCarExpMaterialType m_MaterialType = AuCarExpMaterialType_Other;

	unsigned int m_Flags = 0;

	float m_AlphaCutoff = 1.0f;

	//Will be -1 for an unstamped material:
	int m_StampMapIndex = -1;
	int m_BodyPaintIndex = -1;

	//Paint parameters, use only if m_Flags contains AuCarExpMaterialFlags_IsPaint:
	float m_PearlStrength = 0.0f;
	float m_FlakeStrength = 0.0f;

	AuCarExpLightType m_LightType = AuCarExpLightType_None;
	unsigned int m_LightColour = 0xFFFFFFFF;

	unsigned int m_Tint = 0xFFFFFFFF;
	unsigned int m_SecondaryTint = 0xFFFFFFFF;
	float m_DiffuseTextureToColourLerp = 0.0f;

	AuCarExpTextureData m_DiffuseMapData;
	AuCarExpTextureData m_SecondaryDiffuseMapData;

	AuCarExpTextureData m_NormalMapData;
	AuCarExpTextureData m_SpecularMapData;
	AuCarExpTextureData m_MetallicMapData;
	AuCarExpTextureData m_RoughnessMapData;
	AuCarExpTextureData m_OpacityMapData;
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

struct AuCarExpUIStringData
{
public:

	static const unsigned int LABEL_STRING_LENGTH = 63;
	static const unsigned int VALUE_STRING_LENGTH = 511;

	wchar_t Label[LABEL_STRING_LENGTH + 1] = L"";
	wchar_t Value[VALUE_STRING_LENGTH + 1] = L"";
};

struct AuCarExpUIBoolData
{
public:

	static const unsigned int LABEL_STRING_LENGTH = 63;

	wchar_t Label[LABEL_STRING_LENGTH + 1] = L"";
	bool Value = false;
};

class AuCarExpCarData
{
public:

	inline const wchar_t* GetCarName() const { return m_CarName; }

	inline float GetHeightOffset() const { return m_HeightOffset; }

	inline AuCarExpBodyShape GetBodyShape() const { return m_BodyShape; }

	inline const AuCarExpUIStringData* GetStringData(unsigned int index) const { return &m_StringData[index]; }
	inline unsigned int GetStringDataCount() const { return m_StringDataCount; }

	inline const AuCarExpUIBoolData* GetBoolData(unsigned int index) const { return &m_BoolData[index]; }
	inline unsigned int GetBoolDataCount() const { return m_BoolDataCount; }

protected:

	wchar_t m_CarName[512] = L"";

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
	AuCarExpSoundSample(const wchar_t* Name, float* Samples, int SampleCount) :
		m_SamplesBuffer(Samples),
		m_SampleCount(SampleCount)
	{
		wcscpy_s(m_Name, Name);
	}

	inline const wchar_t* GetName() const { return m_Name; }
	inline float* GetSamples() const { return m_SamplesBuffer; }
	inline unsigned int GetSampleCount() const { return m_SampleCount; }

	//Gets Sample rate, should be 48000hz by default
	inline unsigned int GetSampleRate() const { return m_SampleRate; }

public:
	float* m_SamplesBuffer = nullptr;
	unsigned int  m_SampleCount = 0;
	unsigned int  m_SampleRate = 44100;

	wchar_t m_Name[64] = L"";
};

struct AuCarExpWheelData
{
public:
	float TyreDiameter;
	float TyreWidth;
	float RimDiameter;
	AuCarExpMesh* TyreMesh;
	AuCarExpMesh* RimMesh;
	AuCarExpMesh* BrakeMeshes[AuCarExpBrakeMeshType_Count];

	AuCarExpMesh* SuspensionMesh = nullptr;
	bool MirrorSuspensionMesh = false;
	AuCarExpSuspensionType SuspensionType = AuCarExpSuspensionType_Count;

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

struct AuCarExpLuaFloatData
{
public:
	wchar_t ValueName[64] = L"";
	float Value = 0.0f;
};

struct AuCarExpLuaStringData
{
public:
	wchar_t ValueName[64] = L"";
	wchar_t* Buffer = nullptr;
	int BufferSize = 0;
};


struct AuCarLuaDataFile
{
public:
	wchar_t FileName[64] = L"";
	char* Buffer = nullptr;
	int BufferSize = 0;
};
