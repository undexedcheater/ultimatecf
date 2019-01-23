#include "includes.h"

#define Knockback                                0x0
#define ChangeWeaponAnimRatio                    0x0	
#define ReloadAnimRatio                          0x0

#define AmmoDamage                               0x0
#define KnifeNormalAniRate                       0x0
#define KnifeNoramlAniRate2                      0x0
#define KnifeNoramlRange                         0x0
#define KnifeNoramlRange2                        0x0
#define KnifeBigshotRange                        0x0
#define KnifeBigshotRange2                       0x0
#define KnifeBigshotAngle                        0x0
#define KnifeBigshotAngle2                       0x0
#define KnifeBigshotAniRate                      0x0
#define KnifeBigshotAniRate2                     0x0  
#define  MoveSpeedPenalty							0x0
#define Range										0x0
#define FireAnimMultiplier							0x0

#define MovementWalkRate						 0x0
#define	MovementFriction							0x0
#define MovementDuckWalkRate 	                 0x0
#define C4PlantTime                              0x0
#define C4DefuseTime                             0x0

#define EdgeShotEnabled                          0x0
#define WallShotEnabled                          0x0
#define PerfectWallShotEnabled                   0x0 

#define DamageZone                               0x0
#define DamagePerSec                             0x0
#define Name                                     0x0
#define PointList                                0x0
#define MaxDepth                                 0x0
#define versioncode                              0x0
#define header                                   0x0
#define world                                    0x0
#define Point                                    0x0
#define FloorNameMsgIndex                        0x0
#define Floor                                    0x0
#define VisibleZoneIndex                         0x0
#define ZoneName                                 0x0
#define Rot                                      0x0
#define Pos                                      0x0
#define ZonemanName                              0x0
#define EatmanName                               0x0
#define Eatman                                   0x0
#define Zoneman                                  0x0

#define WallPointer                              0x0
#define Texture									 0x0
#define ModelNode                                0x0

#define FallDamage                               0x0
#define FallDamageOffset                         0x0

#define BasicPlayerInfo 0x0
namespace Hacks
{
	namespace Hitbox
	{
		void Update()
		{
			DWORD cshell = Tools::GetModuleBaseAddress("CShell.dll");
			DWORD MaxAreaHitMgr = *(DWORD*)(cshell + ModelNode);

			if (MaxAreaHitMgr == 0)
				return;

			for (int i = 0; i < 89; i++)
			{
				for (int x = 0; x < 3; x++)
				{
					//Change hitbox
					*(float*)((MaxAreaHitMgr + 0x38 + x * 4) + (i * 0x9C)) = 65.0f;
					*(float*)((MaxAreaHitMgr + 0x3C + x * 4) + (i * 0x9C)) = 28.0f;
					*(float*)((MaxAreaHitMgr + 0x40 + x * 4) + (i * 0x9C)) = 35.0f;
				}
			}
		}
	}

	namespace Player
	{
		void Update()
		{
			DWORD cshell = Tools::GetModuleBaseAddress("CShell.dll");
			DWORD BPI = *(DWORD *)(cshell + BasicPlayerInfo);

			if (BPI == 0)
				return;

			auto PlayerClient = g_pLTClientShell->GetPlayerClient();

			if (PlayerClient == 0)
				return;

			DWORD WallClass = *(DWORD*)(cshell + WallPointer);

			if (WallClass == 0)
				return;

			//NO FALL DAMAGE
			DWORD NoFallDamage = *(DWORD*)(cshell + FallDamage);

			if (NoFallDamage == 0)
				return;

			*(float*)(NoFallDamage + FallDamageOffset) = 0.0f;
			//*(float*)(DistFallDamageStartFrom + FallDamageOffset) = 0.0f;

			DWORD Bag = *(DWORD*)(cshell + 0x0);
			if (Bag == 0)
				return;

			//unlimited bag
			if (GetAsyncKeyState('B'))
				*(DWORD*)(Bag + 0xC) = 0;

			//Shoot Trought Walls
			for (int j = 0; j < 64; j++)
			{
				*(bool *)(WallClass + (j * Texture) + EdgeShotEnabled) = true;
				*(bool *)(WallClass + (j * Texture) + PerfectWallShotEnabled) = true;
				*(bool *)(WallClass + (j * Texture) + WallShotEnabled) = true;
			}

			//Walk Throught Walls
			if (GetAsyncKeyState('V'))
			{
				for (int i = 0; i < 0x1FF; ++i)
				{
					if (*(float*)(PlayerClient + 4 * i) == 56.0f)
						*(float*)(PlayerClient + 4 * i) = -56.0f;
				}
			}

			//Flast Plant
			*(float *)(BPI + C4PlantTime) = 0.0f;

			//Flast Defuse
			*(float *)(BPI + C4DefuseTime) = 1.6f;

			//Simple SpeedHack
			*(float *)(BPI + MovementWalkRate) = 1.2f;
			*(float *)(BPI + MovementDuckWalkRate) = 1.2;

			//No Bug Damage
			*(float*)(VisibleZoneIndex + cshell) = 0;
			*(float*)(DamageZone + cshell) = 0;
			*(float*)(DamagePerSec + cshell) = 0;
			//*(float*)(DamagePerMeter + coCShell) = 0;
			*(float*)(Zoneman + cshell) = 0;
			*(float*)(Eatman + cshell) = 0;
			*(float*)(EatmanName + cshell) = 0;
			*(float*)(versioncode + cshell) = 0;
			//*(float*)(PointIndex + coCShell) = 0;
			*(float*)(Floor + cshell) = 0;
			*(float*)(FloorNameMsgIndex + cshell) = 0;
			*(float*)(world + cshell) = 0;
			*(float*)(PointList + cshell) = 0;
			//*(float*)(Normal + coCShell) = 0;
			//*(float*)(Dist + coCShell) = 0;
			*(float*)(Rot + cshell) = 0;
			*(float*)(Pos + cshell) = 0;
			*(float*)(Point + cshell) = 0;
			//*(float*)(EditPoly + coCShell) = 0;
			//*(float*)(PolyList + coCShell) = 0;
			*(float*)(Name + cshell) = 0;
			*(float*)(MaxDepth + cshell) = 0;
		}
	}

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
			for (int i = 0; i <= 2963; i++)
			{
				uintptr_t pWeapon = GetWeaponByIndex(i);

				if (pWeapon == 0)
					continue;

				auto PlayerClient = g_pLTClientShell->GetPlayerClient();

				if (PlayerClient == 0)
					continue;

				//Less Recoil
				PlayerClient->SetLessRecoil(0);

				//No Spread
				PlayerClient->SetNoSpread(0);

				//Fast Reload
				*(float *)(pWeapon + ReloadAnimRatio) = 999.0F;

				//Fast Switch
				*(float *)(pWeapon + ChangeWeaponAnimRatio) = 999.0F;

				//One Hit Kill
				*(float *)(pWeapon + AmmoDamage) = 999.0F;

				//No Knockback
				*(float *)(pWeapon + Knockback) = 0.0F;

				//No Weapon Weight
				*(float *)(pWeapon + MoveSpeedPenalty) = 0.0F;

				//Weapon Range
				*(float *)(pWeapon + Range) = 0.0F;

				//Rapid Fire
				*(float *)(pWeapon + FireAnimMultiplier) = 10.0F;

			}
		}
	}
}