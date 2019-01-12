#include "includes.h"

CD3DFont* g_pMainFont = nullptr;

using namespace Tools;

typedef int(__thiscall * Present_t)(void *, const struct tagRECT *, const struct tagRECT *, HWND, const struct _RGNDATA *, unsigned int);
Present_t oPresent = 0;

void __fastcall hkPresent(void *_this, void* edx, const struct tagRECT *a2, const struct tagRECT *a3, HWND a4, const struct _RGNDATA *a5, unsigned int a6)
{
	static LPDIRECT3DDEVICE9 pDevice;
	static DWORD FindDevice = NULL;

	if (!FindDevice)
		FindDevice = SearchPattern("crossfire.exe", (PBYTE)"\x8B\x35\xFF\xFF\xFF\xFF\x8B\xEE\xE8\xFF\xFF\xFF\xFF\x8B\x45\x00\x8B\x08\x8B\x91\xFF\xFF\xFF\xFF\x57\x6A\xFF\x53", "xx????xxx????xxxxxxx????xx?x");

	if (FindDevice)
		pDevice = *(LPDIRECT3DDEVICE9 *)(*(DWORD *)(*(DWORD *)(FindDevice + 0x2)));

	if (pDevice)
	{
		pRenderer->SetDevice(pDevice);
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		if (g_pMainFont == nullptr)
		{
			g_pMainFont = new CD3DFont("Verdana", 12, D3DFONT_BOLD);
			g_pMainFont->InitDeviceObjects(pDevice);
			g_pMainFont->RestoreDeviceObjects();
		}

		pRenderer->String(50.0f, 50.0f, D3DCOLOR_ARGB(255, 150, 40, 0), g_pMainFont, true, "www.github.com");
		pRenderer->String(50.0f, 70.0f, D3DCOLOR_ARGB(255, 150, 40, 0), g_pMainFont, false, "www.github.com");

		if (Interfaces::IsValid())
		{
			if (g_pGameFlow->GameStatus == GSC_INGAME)
			{
				Hacks::Weapon::Update();

				g_pLTClient->RunConsoleCmd("TextureModels 0");
				g_pLTClient->RunConsoleCmd("DrawFlat 1");
			}
		}

		pDevice->SetRenderState(D3DRS_ZENABLE, true);
	}

	oPresent(_this, a2, a3, a4, a5, a6);
}

void HookD3D9()
{
	while (!GetModuleBaseAddress("ClientFx.fxd"))
		std::this_thread::sleep_for(10ms);

	Interfaces::Init();

	while (!GetModuleBaseAddress("d3d9.dll"))
		std::this_thread::sleep_for(10ms);

	auto PresentMain = SearchPattern("d3d9.dll",
		(BYTE *)"\x8B\xFF\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC4\x89\x84\x24\x00\x00\x00\x00\x8B\x45\x08\x53\x8B\xD9\x89\x44\x24\x2C\x8B\x45\x0C\x89\x44\x24\x04\x56\x8B\x4B\x0C",
		"xxxxxxxxxx????x????xxxxx????xxxxxxxxxxxxxxxxxxxxx");

	oPresent = (Present_t)PresentMain;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&oPresent, hkPresent);
	DetourTransactionCommit();
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

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&HookD3D9, 0, 0, 0);
		break;
	}
	return 1;
}