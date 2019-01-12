#pragma once

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];

template< class type > type GetVFunction(PVOID base, SIZE_T index)
{
	ULONG_PTR* vtablefunc = *(PDWORD_PTR*)base;

	return (type)(vtablefunc[index]);
}

class CTypeDescriptor
{
public:
	char pad_0000[12]; //0x0000
	char class_name[60]; //0x000C
}; //Size: 0x0048

template<typename T>
bool ValidatePointer(T lpAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T size = VirtualQuery(reinterpret_cast<LPVOID>(lpAddress), &mbi, sizeof(MEMORY_BASIC_INFORMATION));

	if (size == 0)
		return false;

	if (mbi.Protect & PAGE_NOACCESS)
		return false;

	if (mbi.Protect & PAGE_GUARD)
		return false;

	return true;
}

#pragma pack(1)		// Packed as the size must be 512 bytes exactly
typedef struct _IDENTIFY_DEVICE_DATA {
	struct {
		USHORT Reserved1 : 1;
		USHORT Retired3 : 1;
		USHORT ResponseIncomplete : 1;
		USHORT Retired2 : 3;
		USHORT FixedDevice : 1;
		USHORT RemovableMedia : 1;
		USHORT Retired1 : 7;
		USHORT DeviceType : 1;
	} GeneralConfiguration;
	USHORT NumCylinders;
	USHORT ReservedWord2;
	USHORT NumHeads;
	USHORT Retired1[2];
	USHORT NumSectorsPerTrack;
	USHORT VendorUnique1[3];
	UCHAR  SerialNumber[20];
	USHORT Retired2[2];
	USHORT Obsolete1;
	UCHAR  FirmwareRevision[8];
	UCHAR  ModelNumber[40];
	UCHAR  MaximumBlockTransfer;
	UCHAR  VendorUnique2;
	USHORT ReservedWord48;
	struct {
		UCHAR  ReservedByte49;
		UCHAR  DmaSupported : 1;
		UCHAR  LbaSupported : 1;
		UCHAR  IordyDisable : 1;
		UCHAR  IordySupported : 1;
		UCHAR  Reserved1 : 1;
		UCHAR  StandybyTimerSupport : 1;
		UCHAR  Reserved2 : 2;
		USHORT ReservedWord50;
	} Capabilities;
	USHORT ObsoleteWords51[2];
	USHORT TranslationFieldsValid : 3;
	USHORT Reserved3 : 13;
	USHORT NumberOfCurrentCylinders;
	USHORT NumberOfCurrentHeads;
	USHORT CurrentSectorsPerTrack;
	ULONG  CurrentSectorCapacity;
	UCHAR  CurrentMultiSectorSetting;
	UCHAR  MultiSectorSettingValid : 1;
	UCHAR  ReservedByte59 : 7;
	ULONG  UserAddressableSectors;
	USHORT ObsoleteWord62;
	USHORT MultiWordDMASupport : 8;
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;
	USHORT ReservedByte64 : 8;
	USHORT MinimumMWXferCycleTime;
	USHORT RecommendedMWXferCycleTime;
	USHORT MinimumPIOCycleTime;
	USHORT MinimumPIOCycleTimeIORDY;
	USHORT ReservedWords69[6];
	USHORT QueueDepth : 5;
	USHORT ReservedWord75 : 11;
	USHORT ReservedWords76[4];
	USHORT MajorRevision;
	USHORT MinorRevision;
	struct {
		USHORT SmartCommands : 1;
		USHORT SecurityMode : 1;
		USHORT RemovableMediaFeature : 1;
		USHORT PowerManagement : 1;
		USHORT Reserved1 : 1;
		USHORT WriteCache : 1;
		USHORT LookAhead : 1;
		USHORT ReleaseInterrupt : 1;
		USHORT ServiceInterrupt : 1;
		USHORT DeviceReset : 1;
		USHORT HostProtectedArea : 1;
		USHORT Obsolete1 : 1;
		USHORT WriteBuffer : 1;
		USHORT ReadBuffer : 1;
		USHORT Nop : 1;
		USHORT Obsolete2 : 1;
		USHORT DownloadMicrocode : 1;
		USHORT DmaQueued : 1;
		USHORT Cfa : 1;
		USHORT AdvancedPm : 1;
		USHORT Msn : 1;
		USHORT PowerUpInStandby : 1;
		USHORT ManualPowerUp : 1;
		USHORT Reserved2 : 1;
		USHORT SetMax : 1;
		USHORT Acoustics : 1;
		USHORT BigLba : 1;
		USHORT DeviceConfigOverlay : 1;
		USHORT FlushCache : 1;
		USHORT FlushCacheExt : 1;
		USHORT Resrved3 : 2;
		USHORT SmartErrorLog : 1;
		USHORT SmartSelfTest : 1;
		USHORT MediaSerialNumber : 1;
		USHORT MediaCardPassThrough : 1;
		USHORT StreamingFeature : 1;
		USHORT GpLogging : 1;
		USHORT WriteFua : 1;
		USHORT WriteQueuedFua : 1;
		USHORT WWN64Bit : 1;
		USHORT URGReadStream : 1;
		USHORT URGWriteStream : 1;
		USHORT ReservedForTechReport : 2;
		USHORT IdleWithUnloadFeature : 1;
		USHORT Reserved4 : 2;
	} CommandSetSupport;
	struct {
		USHORT SmartCommands : 1;
		USHORT SecurityMode : 1;
		USHORT RemovableMediaFeature : 1;
		USHORT PowerManagement : 1;
		USHORT Reserved1 : 1;
		USHORT WriteCache : 1;
		USHORT LookAhead : 1;
		USHORT ReleaseInterrupt : 1;
		USHORT ServiceInterrupt : 1;
		USHORT DeviceReset : 1;
		USHORT HostProtectedArea : 1;
		USHORT Obsolete1 : 1;
		USHORT WriteBuffer : 1;
		USHORT ReadBuffer : 1;
		USHORT Nop : 1;
		USHORT Obsolete2 : 1;
		USHORT DownloadMicrocode : 1;
		USHORT DmaQueued : 1;
		USHORT Cfa : 1;
		USHORT AdvancedPm : 1;
		USHORT Msn : 1;
		USHORT PowerUpInStandby : 1;
		USHORT ManualPowerUp : 1;
		USHORT Reserved2 : 1;
		USHORT SetMax : 1;
		USHORT Acoustics : 1;
		USHORT BigLba : 1;
		USHORT DeviceConfigOverlay : 1;
		USHORT FlushCache : 1;
		USHORT FlushCacheExt : 1;
		USHORT Resrved3 : 2;
		USHORT SmartErrorLog : 1;
		USHORT SmartSelfTest : 1;
		USHORT MediaSerialNumber : 1;
		USHORT MediaCardPassThrough : 1;
		USHORT StreamingFeature : 1;
		USHORT GpLogging : 1;
		USHORT WriteFua : 1;
		USHORT WriteQueuedFua : 1;
		USHORT WWN64Bit : 1;
		USHORT URGReadStream : 1;
		USHORT URGWriteStream : 1;
		USHORT ReservedForTechReport : 2;
		USHORT IdleWithUnloadFeature : 1;
		USHORT Reserved4 : 2;
	} CommandSetActive;
	USHORT UltraDMASupport : 8;
	USHORT UltraDMAActive : 8;
	USHORT ReservedWord89[4];
	USHORT HardwareResetResult;
	USHORT CurrentAcousticValue : 8;
	USHORT RecommendedAcousticValue : 8;
	USHORT ReservedWord95[5];
	ULONG  Max48BitLBA[2];
	USHORT StreamingTransferTime;
	USHORT ReservedWord105;
	struct {
		USHORT LogicalSectorsPerPhysicalSector : 4;
		USHORT Reserved0 : 8;
		USHORT LogicalSectorLongerThan256Words : 1;
		USHORT MultipleLogicalSectorsPerPhysicalSector : 1;
		USHORT Reserved1 : 2;
	} PhysicalLogicalSectorSize;
	USHORT InterSeekDelay;
	USHORT WorldWideName[4];
	USHORT ReservedForWorldWideName128[4];
	USHORT ReservedForTlcTechnicalReport;
	USHORT WordsPerLogicalSector[2];
	struct {
		USHORT ReservedForDrqTechnicalReport : 1;
		USHORT WriteReadVerifySupported : 1;
		USHORT Reserved01 : 11;
		USHORT Reserved1 : 2;
	} CommandSetSupportExt;
	struct {
		USHORT ReservedForDrqTechnicalReport : 1;
		USHORT WriteReadVerifyEnabled : 1;
		USHORT Reserved01 : 11;
		USHORT Reserved1 : 2;
	} CommandSetActiveExt;
	USHORT ReservedForExpandedSupportandActive[6];
	USHORT MsnSupport : 2;
	USHORT ReservedWord1274 : 14;
	struct {
		USHORT SecuritySupported : 1;
		USHORT SecurityEnabled : 1;
		USHORT SecurityLocked : 1;
		USHORT SecurityFrozen : 1;
		USHORT SecurityCountExpired : 1;
		USHORT EnhancedSecurityEraseSupported : 1;
		USHORT Reserved0 : 2;
		USHORT SecurityLevel : 1;
		USHORT Reserved1 : 7;
	} SecurityStatus;
	USHORT ReservedWord129[31];
	struct {
		USHORT MaximumCurrentInMA2 : 12;
		USHORT CfaPowerMode1Disabled : 1;
		USHORT CfaPowerMode1Required : 1;
		USHORT Reserved0 : 1;
		USHORT Word160Supported : 1;
	} CfaPowerModel;
	USHORT ReservedForCfaWord161[8];
	struct {
		USHORT SupportsTrim : 1;
		USHORT Reserved0 : 15;
	} DataSetManagementFeature;
	USHORT ReservedForCfaWord170[6];
	USHORT CurrentMediaSerialNumber[30];
	USHORT ReservedWord206;
	USHORT ReservedWord207[2];
	struct {
		USHORT AlignmentOfLogicalWithinPhysical : 14;
		USHORT Word209Supported : 1;
		USHORT Reserved0 : 1;
	} BlockAlignment;
	USHORT WriteReadVerifySectorCountMode3Only[2];
	USHORT WriteReadVerifySectorCountMode2Only[2];
	struct {
		USHORT NVCachePowerModeEnabled : 1;
		USHORT Reserved0 : 3;
		USHORT NVCacheFeatureSetEnabled : 1;
		USHORT Reserved1 : 3;
		USHORT NVCachePowerModeVersion : 4;
		USHORT NVCacheFeatureSetVersion : 4;
	} NVCacheCapabilities;
	USHORT NVCacheSizeLSW;
	USHORT NVCacheSizeMSW;
	USHORT NominalMediaRotationRate;
	USHORT ReservedWord218;
	struct {
		UCHAR NVCacheEstimatedTimeToSpinUpInSeconds;
		UCHAR Reserved;
	} NVCacheOptions;
	USHORT ReservedWord220[35];
	USHORT Signature : 8;
	USHORT CheckSum : 8;
} IDENTIFY_DEVICE_DATA, *PIDENTIFY_DEVICE_DATA;
#pragma pack()

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct LDR_DATA_ENTRY {
	LIST_ENTRY              InMemoryOrderModuleList;
	PVOID                   BaseAddress;
	PVOID                   EntryPoint;
	ULONG                   SizeOfImage;
	UNICODE_STRING          FullDllName;
	UNICODE_STRING          BaseDllName;
	ULONG                   Flags;
	SHORT                   LoadCount;
	SHORT                   TlsIndex;
	LIST_ENTRY              HashTableEntry;
	ULONG                   TimeDateStamp;
} LDR_DATA_ENTRY, *PLDR_DATA_ENTRY;

class CPlayerInfo
{
public:
	PAD(8); //0x0000
	union
	{
		void* PtrObject; //0x0008
		int* iObject; //0x0008
	};
	int8_t iClientID; //0x000C
	int8_t iTeam; //0x000D
	char szName[12]; //0x000E
	PAD(2); //0x001A
	void* PtrCharFX; //0x001C
	int32_t iPlayerSlotTeam; //0x0020
	PAD(4); //0x0024
	bool bHasC4; //0x0028
	int32_t iState; //0x0029
	int32_t iRank; //0x002D
	PAD(8); //0x0031
	int16_t iHealth; //0x0039
	int16_t iKills; //0x003B
	int32_t iDeaths; //0x003D
	int32_t iHeadshots; //0x0041
	int32_t iTeamID; //0x0045
	int32_t iPing; //0x0049
}; //Size: 0x004D

__forceinline CPlayerInfo* GetPlayerByIndex(uintptr_t client_shell, uint8_t i)
{
	CPlayerInfo* player = nullptr;

	if (i > 0 && i <= 16)
	{
		player = (CPlayerInfo *)(client_shell + (0xD78 * i) + 0x1E8);
	}
	return player;
}

__forceinline CPlayerInfo* GetLocalPlayer(uintptr_t client_shell)
{
	using GetLocalPlayer_t = uint8_t(__thiscall *)(DWORD);

	static DWORD dwGetLocalPlayerFnc = 0;
		//Tools::SearchPattern("CShell.dll", (BYTE *)"\x56\x8B\xF1\x0F\xB6\x86\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x04", "xxxxxx????xx????xxxxxxx");

	return GetPlayerByIndex(client_shell, reinterpret_cast<GetLocalPlayer_t>(dwGetLocalPlayerFnc)(client_shell));
}

//CPlayerClient Offset -> 8B 49 78 85 C9 74 3E 
class CPlayerClient
{
public:
	float *GetYaw()
	{
		return *(float **)((uintptr_t)this + 0x4E4);
	}

	float *GetPitch()
	{
		return *(float **)((uintptr_t)this + 0x4EC);
	}
};

struct LTVector
{
	float x, y, z;
};

struct LTRotation
{
	float Quad[4];
};

struct LTransform
{
	LTVector	m_vPos;
	LTRotation	m_rRot;
	float		m_fScale;
};

class CLTModel
{
public:
	uint32_t GetNodeTransform(int* hObj, uint32_t hNode, LTransform &transform, bool bWorldSpace)
	{
		typedef uint32_t(__thiscall* oGetNodeTransform)(void*, int*, uint32_t, LTransform &, bool);
		return GetVFunction<oGetNodeTransform>(this, 15)(this, hObj, hNode, transform, bWorldSpace);
	}

	uint32_t GetNode(int* hObj, const char *pNodeName, uint32_t &hNode)
	{
		typedef uint32_t(__thiscall* oGetNode)(void*, int*, const char *, uint32_t &);
		return GetVFunction<oGetNode>(this, 13)(this, hObj, pNodeName, hNode);
	}
};

//CLTClientShell Sig: 8B 0D ? ? ? ? 57 E8 ? ? ? ? 57 
class CLTClientShell
{
public:
	CPlayerClient* GetPlayerClient()
	{
		return (CPlayerClient*)((DWORD)this + 0x78);
	}
};

class CLTBase
{
public:
	void RunConsoleCmd(const char* pszCommand)
	{
		using RunConsoleCmd_t = void(__cdecl *)(const char*);
		DWORD ptr = *( DWORD *)((DWORD)this + 0x21C);
		return reinterpret_cast<RunConsoleCmd_t>(ptr)(pszCommand);
	}
};

//CLTClient Sig: 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? 8D 54 24 24 52 8B 16 
class CLTClient : public CLTBase
{
public:

	CLTModel* GetLTModel()
	{
		typedef CLTModel*(__thiscall* oGetLTModel)(void *);
		return GetVFunction<oGetLTModel>(this, 4)(this);
	}
};

//ModelNodeMgr Sig: 8B 15 ? ? ? ? D9 5C 17 2C 

enum /*class*/ eGameState
{
	GSC_LOGIN = 1,
	GSC_SERVER = 17,
	GSC_CHANNEL = 3,
	GSC_ROOM = 8,
	GSC_MEGALOTTO = 22,
	GSC_STORAGE = 4,
	GSC_ITEMSHOP = 18,
	GSC_MILEAGE = 33,
	GSC_CLAN = 28,
	GSC_TOURNAMENT = 30,
	GSC_INROOM = 13,
	GSC_LOADING85 = 0,
	GSC_INGAME = 11,
	GSC_AFTERINGAMERECORDLIST = 14
};

//CGameFlowMgr Sig: 8B 15 ? ? ? ? A1 ? ? ? ? 83 EC 1C 
class CGameFlowMgr
{
public:
	eGameState GameStatus;
};

extern CLTClientShell* g_pLTClientShell;
extern CLTClient* g_pLTClient;
extern CGameFlowMgr* g_pGameFlow;
