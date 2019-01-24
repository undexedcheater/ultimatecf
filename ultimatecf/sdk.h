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

#pragma pack(1)

typedef struct _IDENTIFY_DATA {
	USHORT GeneralConfiguration;       /* 00 */
	USHORT NumCylinders;               /* 02 */
	USHORT Reserved1;                  /* 04 */
	USHORT NumHeads;                   /* 06 */
	USHORT UnformattedBytesPerTrack;   /* 08 */
	USHORT UnformattedBytesPerSector;  /* 10 */
	USHORT NumSectorsPerTrack;         /* 12 */
	USHORT VendorUnique1[3];           /* 14 */
	UCHAR  SerialNumber[20];           /* 20 */
	USHORT BufferType;                 /* 40 */
	USHORT BufferSectorSize;           /* 42 */
	USHORT NumberOfEccBytes;           /* 44 */
	UCHAR  FirmwareRevision[8];        /* 46 */
	UCHAR  ModelNumber[40];            /* 54 */
	UCHAR  MaximumBlockTransfer;       /* 94 */
	UCHAR  VendorUnique2;              /* 95 */
	USHORT DoubleWordIo;               /* 96 */
	USHORT Capabilities;               /* 98 */
	USHORT Reserved2;                  /* 100 */
	UCHAR  VendorUnique3;              /* 102 */
	UCHAR  PioCycleTimingMode;         /* 103 */
	UCHAR  VendorUnique4;              /* 104 */
	UCHAR  DmaCycleTimingMode;         /* 105 */
	USHORT TranslationFieldsValid : 3;   /* 106 */
	USHORT Reserved3 : 13;               /*  -  */
	USHORT NumberOfCurrentCylinders;   /* 108 */
	USHORT NumberOfCurrentHeads;       /* 110 */
	USHORT CurrentSectorsPerTrack;     /* 112 */
	ULONG  CurrentSectorCapacity;      /* 114 */
	USHORT CurrentMultiSectorSetting;  /* 118 */
	ULONG  UserAddressableSectors;     /* 120 */
	USHORT SingleWordDMASupport : 8;     /* 124 */
	USHORT SingleWordDMAActive : 8;      /*  -  */
	USHORT MultiWordDMASupport : 8;      /* 126 */
	USHORT MultiWordDMAActive : 8;       /*  -  */
	USHORT AdvancedPIOModes : 8;         /* 128 */
	USHORT Reserved4 : 8;                /*  -  */
	USHORT MinimumMWXferCycleTime;     /* 130 */
	USHORT RecommendedMWXferCycleTime; /* 132 */
	USHORT MinimumPIOCycleTime;        /* 134 */
	USHORT MinimumPIOCycleTimeIORDY;   /* 136 */
	USHORT Reserved5[11];              /* 138 */
	USHORT MajorRevision;              /* 160 */
	USHORT MinorRevision;              /* 162 */
	USHORT Reserved6;                  /* 164 */
	USHORT CommandSetSupport;          /* 166 */
	USHORT Reserved6a[2];              /* 168 */
	USHORT CommandSetActive;           /* 172 */
	USHORT Reserved6b;                 /* 174 */
	USHORT UltraDMASupport : 8;          /* 176 */
	USHORT UltraDMAActive : 8;           /*  -  */
	USHORT Reserved7[11];              /* 178 */
	ULONG  Max48BitLBA[2];             /* 200 */
	USHORT Reserved7a[22];             /* 208 */
	USHORT LastLun : 3;                  /* 252 */
	USHORT Reserved8 : 13;               /*  -  */
	USHORT MediaStatusNotification : 2;  /* 254 */
	USHORT Reserved9 : 6;                /*  -  */
	USHORT DeviceWriteProtect : 1;       /*  -  */
	USHORT Reserved10 : 7;               /*  -  */
	USHORT Reserved11[128];            /* 256 */
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

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

//typedef bool(*IntersectSegment) (const IntersectQuery &iQuery, IntersectInfo *qInfo);
typedef bool(*ObjectFilterFn) (int* hObj, void* pUserData);

struct IntersectInfo
{
	D3DXVECTOR3 vImpactPos;		//0x0000
	char unknown12[16];
	int *hObjImpact;			//0x001C
	DWORD nSomething1;			//0x0020
	DWORD nSomething2;			//0x0024
	DWORD nSomething3;			//0x0028
};

struct IntersectQuery
{
	D3DXVECTOR3 Start;					//0x0000
	D3DXVECTOR3 End;					//0x000C
	char unknown24[12];
	UINT Flags;							//0x0024
	ObjectFilterFn FilterFn;			//0x0028
	void *FilterIntersectParam;			//0x002C
	ObjectFilterFn FilterIntersectFn;	//0x0030
	void *FilterParam;					//0x0034
	void *PolyFilterParam;				//0x0038
};

class CObjectFX
{
public:
	PAD(160); //0x0004
	int* Object; //0x00A4
	PAD(8); //0x00A8
	float fPitch; //0x00B0
	float fYaw; //0x00B4
	float fRoll; //0x00B8
	PAD(44); //0x00BC
	D3DXVECTOR3 Position; //0x00E8
	PAD(272); //0x00F4
	int8_t IsDead; //0x0204
	PAD(15); //0x0205
	int8_t SpawnShield; //0x0214
	PAD(763); //0x0215
	__int32 numHitBox; //0x0510
};

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
	CObjectFX* pObjectFX; //0x001C
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

__forceinline CPlayerInfo* GetPlayerByIndex(uint8_t i)
{
	CPlayerInfo* player = nullptr;

	if (i > 0 && i <= 16)
	{
		player = (CPlayerInfo *)((DWORD)g_pLTClientShell + (0xD78 * i) + 0x1E8);
	}
	return player;
}

__forceinline CPlayerInfo* GetLocalPlayer( )
{
	using GetLocalPlayer_t = uint8_t(__thiscall *)(DWORD);

	static DWORD dwGetLocalPlayerFnc = 
		Tools::SearchPattern("CShell.dll", (BYTE *)"\x56\x8B\xF1\x0F\xB6\x86\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x04", "xxxxxx????xx????xxxxxxx");

	return GetPlayerByIndex( reinterpret_cast<GetLocalPlayer_t>(dwGetLocalPlayerFnc)((DWORD)g_pLTClientShell) );
}

//CPlayerClient Offset -> 8B 49 78 85 C9 74 3E 
#define LessRecoil                               0x618
#define LessRecoilZM                             0x528
#define NoSpread                                 0x5F8
#define NoSpreadZM                               0x5F4 // 0x4F4
#define Yaw                                      0x4E4
#define Pitch                                    0x4EC

class CPlayerClient
{
public:
	float GetYaw()
	{
		return *(float *)((uintptr_t)this + Yaw);
	}

	float GetPitch()
	{
		return *(float *)((uintptr_t)this + Pitch);
	}

	void SetPlayerAngles(float x, float y)
	{
		*(float *)((uintptr_t)this + Yaw) = x;
		*(float *)((uintptr_t)this + Pitch) = y;
	}

	void SetLessRecoil(int val)
	{
		*(DWORD *)((uintptr_t)this + LessRecoil) = val;
	}

	void SetNoSpread(int val)
	{
		*(DWORD *)((uintptr_t)this + NoSpread) = val;
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

#define AIOBJECT_OFF		0x0
#define DROPPEDFX_OFF		0x0

class CAIObject
{
public:
	char pad_0000[164];
	int* Object; // 0x00A4
};

class CDroppedWeaponFX
{
public:
	char pad_0000[8]; //0x0000
	__int32 ItemType; //0x0008
	int* cObject0; //0x000C
	int* cObject1; //0x0010
};

//CLTClientShell Sig: 8B 0D ? ? ? ? 57 E8 ? ? ? ? 57 
class CLTClientShell
{
public:
	CPlayerClient* GetPlayerClient()
	{
		return (CPlayerClient*)((DWORD)this + 0x78);
	}

	CAIObject* GetAIObject()
	{
		return **(CAIObject***)((DWORD)this + AIOBJECT_OFF);
	}

	CDroppedWeaponFX* GetDroppedFX()
	{
		return *(CDroppedWeaponFX**)((DWORD)this + DROPPEDFX_OFF);
	}
};

class CLTBase
{
public:
	void RunConsoleCmd(const char* pszCommand)
	{
		using RunConsoleCmd_t = void(__cdecl *)(const char*);
		DWORD ptr = ((DWORD)this + 0x21C);
		return reinterpret_cast<RunConsoleCmd_t>(ptr)(pszCommand);
	}

	void SetObjectOutLine(int* Object, bool Status, uint8_t r, uint8_t g, uint8_t b)
	{
		using oSetObjectOutLine = void(__cdecl* )( int*, bool, uint8_t, uint8_t, uint8_t);
		DWORD ptr = ((DWORD)this + 0x108);
		return reinterpret_cast<oSetObjectOutLine>(ptr)( Object, Status, r, g, b );
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

	uint32_t GetObjectPos(int* hObj, D3DXVECTOR3 *pPos)
	{
		typedef uint32_t (__thiscall* oGetObjectPos)(void *, int*, D3DXVECTOR3* );
		return GetVFunction<oGetObjectPos>(this, 999 /*find yourself because i was too lazy to do it, hint: 0x0098 - 0x009C*/)(this, hObj, pPos);
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
