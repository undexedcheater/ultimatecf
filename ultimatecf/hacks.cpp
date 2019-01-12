#include "includes.h"

namespace Hacks
{
	namespace Weapon
	{
		//GetWpnByIndx: 66 8B 4C 24 ? 66 83 F9 FF 74 35 66 81 F9 ? ? 74 2E 33 C0 66 85 C9 7C 29 56 8B 35 ? ? ? ? 0F BF D1 83 C6 FF 
		__forceinline uintptr_t GetWeaponByIndex(uint16_t i)
		{
			using GetWpnById_t = uintptr_t(__cdecl*)(uint16_t);

			static DWORD fnc_GetWpById = Tools::SearchPattern("CShell.dll", (BYTE *)"\x66\x8B\x4C\x24\x00\x66\x83\xF9\xFF\x74\x35\x66\x81\xF9\x00\x00\x74\x2E\x33\xC0\x66\x85\xC9\x7C\x29\x56", "xxxx?xxxxxxxxx??xxxxxxxxxx");

			return reinterpret_cast<GetWpnById_t>(fnc_GetWpById)(i);
		}

		void Update()
		{
			for (int i = 0; i <= 3071; i++)
			{
				uintptr_t pWeapon = GetWeaponByIndex(i);

				if (pWeapon != 0)
				{
					*(float *)(pWeapon + 0xE34) = 999.0F;
				}
			}
		}
	}
}