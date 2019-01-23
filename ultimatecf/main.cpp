#include "includes.h"

ID3DXFont* g_pMainFont = nullptr;

using namespace Tools;

typedef LONG(__stdcall * Present_t)(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
Present_t oPresent = 0;

LONG WINAPI hkPresent(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	static LPDIRECT3DDEVICE9 g_pDevice = 0;

	/*static DWORD FindDevice = NULL;

	if (!FindDevice)
		FindDevice = SearchPattern("crossfire.exe", (PBYTE)"\x8B\x35\xFF\xFF\xFF\xFF\x8B\xEE\xE8\xFF\xFF\xFF\xFF\x8B\x45\x00\x8B\x08\x8B\x91\xFF\xFF\xFF\xFF\x57\x6A\xFF\x53", "xx????xxx????xxxxxxx????xx?x");

	if (FindDevice)
		pDevice = *(LPDIRECT3DDEVICE9 *)(*(DWORD *)(*(DWORD *)(FindDevice + 0x2)));
		*/

	if (g_pDevice != pDevice)
	{
		g_pDevice = pDevice;
		try 
		{
			if (g_pMainFont != 0 ) 
			{
				g_pMainFont->Release();
			}
		}
		catch (...) {}

		g_pMainFont = 0;

		D3DXCreateFontA(pDevice, 12, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pMainFont);
	}

	if (pDevice)
	{
		pRenderer->SetDevice(pDevice);
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		Menu::Update(pDevice);

		if (Interfaces::IsValid())
		{
			if (g_pGameFlow->GameStatus == GSC_INGAME)
			{
				Hacks::Weapon::Update();
				Hacks::Player::Update();
				Hacks::Hitbox::Update();

				ESP::Update();
				Aimbot::Update();
			}
		}

		pDevice->SetRenderState(D3DRS_ZENABLE, true);
	}

	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void HookD3D9(void* arg)
{
	MemoryMod(Thread_StartAddr, bytes, 6);

	srand((UINT)time(0));

	while (!GetModuleHandleA("d3d9.dll"))
		std::this_thread::sleep_for(10ms);

	auto DevicePtr = SearchPattern("d3d9.dll", (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	DWORD *VTables = { 0 };

	memcpy(&VTables, (void *)( DevicePtr + 2 ), 4);
	//auto PresentMain = SearchPattern("d3d9.dll",
	//	(BYTE *)"\x8B\xFF\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC4\x89\x84\x24\x00\x00\x00\x00\x8B\x45\x08\x53\x8B\xD9\x89\x44\x24\x2C\x8B\x45\x0C\x89\x44\x24\x04\x56\x8B\x4B\x0C",
	//	"xxxxxxxxxx????x????xxxxx????xxxxxxxxxxxxxxxxxxxxx");

	oPresent = (Present_t)VTables[17];

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&oPresent, hkPresent);
	DetourTransactionCommit();

	while (!GetModuleHandleA("CShell.dll"))
		std::this_thread::sleep_for(10ms);

	GameHelper::oIntersectSegment = (GameHelper::tIntersectSegment) *( DWORD *)( g_pLTClient + 0x7C );
	*(DWORD *)(g_pLTClient + 0x7C) = (DWORD)RemoteKill::IntersectAndPenetrateSegment;

	Interfaces::Init();
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		pRenderer = std::make_unique< CRender2D >();

		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen("CON", "w", stdout);

		printf("DLL Allocated at: %p\n", (DWORD)hDll);

		EraseHeaders(hDll);
		CreateStealthThread((threadFunc_t)&HookD3D9, nullptr);

		break;
	}
	return 1;
}