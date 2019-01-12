#include "includes.h"

CLTClientShell* g_pLTClientShell = nullptr;
CLTClient* g_pLTClient = nullptr;
CGameFlowMgr* g_pGameFlow = nullptr;

namespace Interfaces
{
	void Init()
	{
		g_pLTClient			= **(CLTClient ***)(Tools::SearchPattern("CShell.dll", (BYTE *)"\x8B\x0D\x00\x00\x00\x00\x8B\x01\x8B\x80\x00\x00\x00\x00\x8D\x54\x24\x24\x52\x8B\x16", "xx????xxxx????xxxxxxx") + 0x2);
		g_pLTClientShell	= **(CLTClientShell ***)(Tools::SearchPattern("CShell.dll", (BYTE *)"\x8B\x0D\x00\x00\x00\x00\x57\xE8\x00\x00\x00\x00\x57", "xx????xx????x") + 0x2);
		g_pGameFlow			= *(CGameFlowMgr**)(Tools::SearchPattern("CShell.dll", (BYTE *)"\x8B\x15\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x83\xEC\x1C", "xx????x????xxx") + 0x2);
	
		printf("g_pLTClient\t\t->\t\t[%p]\n", g_pLTClient);
		printf("g_pLTClientShell\t->\t\t[%p]\n", g_pLTClientShell);
		printf("g_pLTModel\t->\t\t[%p]\n", g_pLTClient->GetLTModel());
		printf("g_pGameFlow\t\t->\t\t[%p]\n", g_pGameFlow);
	}

	bool IsValid()
	{
		if (g_pLTClient != nullptr &&
			g_pLTClientShell != nullptr &&
			g_pGameFlow != nullptr)
			return true;
		return false;
	}
};