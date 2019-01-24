#include "includes.h"

using namespace GameHelper;

namespace ESP
{
	__forceinline void Bones(int* Obj, DWORD color)
	{
		DrawBone(Obj, 6, 0, color);
		DrawBone(Obj, 5, 6, color);
		DrawBone(Obj, 4, 5, color);
		DrawBone(Obj, 3, 4, color);
		DrawBone(Obj, 2, 3, color);
		DrawBone(Obj, 1, 2, color);
		DrawBone(Obj, 21, 1, color);
		DrawBone(Obj, 22, 21, color);
		DrawBone(Obj, 23, 22, color);
		DrawBone(Obj, 24, 23, color);
		DrawBone(Obj, 25, 1, color);
		DrawBone(Obj, 26, 25, color);
		DrawBone(Obj, 27, 26, color);
		DrawBone(Obj, 28, 27, color);
		DrawBone(Obj, 14, 5, color);
		DrawBone(Obj, 15, 14, color);
		DrawBone(Obj, 16, 15, color);
		DrawBone(Obj, 17, 16, color);
		DrawBone(Obj, 18, 17, color);
		DrawBone(Obj, 19, 17, color);
		DrawBone(Obj, 20, 17, color);
		DrawBone(Obj, 7, 5, color);
		DrawBone(Obj, 8, 7, color);
		DrawBone(Obj, 9, 8, color);
		DrawBone(Obj, 10, 9, color);
		DrawBone(Obj, 11, 10, color);
		DrawBone(Obj, 12, 10, color);
		DrawBone(Obj, 13, 10, color);
	}

	__forceinline void Box2D(D3DXVECTOR3 Head, D3DXVECTOR3 Foot, DWORD dwColor)
	{
		D3DXVECTOR3 Box = Head - Foot;

		if (Box.y < 0)
			Box.y *= -1;

		int BoxWidth = (int)Box.y / 2;
		int DrawX = (int)Head.x - (BoxWidth / 2);
		int DrawY = (int)Head.y;

		pRenderer->BorderedBox(DrawX + 1, DrawY, BoxWidth, (int)Box.y, 1, Black);
		pRenderer->BorderedBox(DrawX, DrawY, BoxWidth, (int)Box.y, 1, dwColor);
	}

	__forceinline void Box3D(int* Obj, DWORD color)
	{
		D3DXVECTOR3 Pos0, Pos1, Pos2, Pos3, Pos4, Pos5, Pos6, Pos7, Pos8;

		std::function< D3DXVECTOR3( D3DXVECTOR3, D3DXVECTOR3 ) > GetMidPoint = []( D3DXVECTOR3 V1, D3DXVECTOR3 V2 )
		{
			D3DXVECTOR3 Mid;
			Mid.x = (V1.x + V2.x) / 2;
			Mid.y = (V1.y + V2.y) / 2;
			Mid.z = (V1.z + V2.z) / 2;
			return Mid;
		};

		D3DXVECTOR3 vBoneHead, vBoneSpine;

		if (!GetBonePosition(Obj, 6, &vBoneHead, true))
			return;

		if (!GetBonePosition(Obj, 3, &vBoneSpine, true))
			return;

		Pos0 = GetMidPoint(vBoneHead, vBoneSpine);
		Pos1 = Pos0 + D3DXVECTOR3(-50, 160, -50);
		Pos2 = Pos0 + D3DXVECTOR3(-50, -160, -50);
		Pos3 = Pos0 + D3DXVECTOR3(50, -160, -50);
		Pos4 = Pos0 + D3DXVECTOR3(50, 160, -50);
		Pos5 = Pos0 + D3DXVECTOR3(-50, 160, 50);
		Pos6 = Pos0 + D3DXVECTOR3(-50, -160, 50);
		Pos7 = Pos0 + D3DXVECTOR3(50, -160, 50);
		Pos8 = Pos0 + D3DXVECTOR3(50, 160, 50);

		if (WorldToScreen( &Pos1, &Pos1 ) && 
			WorldToScreen( &Pos2, &Pos2) && 
			WorldToScreen( &Pos3, &Pos3) &&
			WorldToScreen( &Pos4, &Pos4) &&
			WorldToScreen( &Pos5, &Pos5) &&
			WorldToScreen( &Pos6, &Pos6) &&
			WorldToScreen( &Pos7, &Pos7) &&
			WorldToScreen( &Pos8, &Pos8))
		{
			pRenderer->Line( Pos1.x, Pos1.y, Pos2.x, Pos2.y, color);
			pRenderer->Line( Pos2.x, Pos2.y, Pos3.x, Pos3.y, color);
			pRenderer->Line( Pos3.x, Pos3.y, Pos4.x, Pos4.y, color);
			pRenderer->Line( Pos4.x, Pos4.y, Pos1.x, Pos1.y, color);
			pRenderer->Line( Pos5.x, Pos5.y, Pos6.x, Pos6.y, color);
			pRenderer->Line( Pos6.x, Pos6.y, Pos7.x, Pos7.y, color);
			pRenderer->Line( Pos7.x, Pos7.y, Pos8.x, Pos8.y, color);
			pRenderer->Line( Pos8.x, Pos8.y, Pos5.x, Pos5.y, color);
			pRenderer->Line( Pos1.x, Pos1.y, Pos5.x, Pos5.y, color);
			pRenderer->Line( Pos2.x, Pos2.y, Pos6.x, Pos6.y, color);
			pRenderer->Line( Pos3.x, Pos3.y, Pos7.x, Pos7.y, color);
			pRenderer->Line( Pos4.x, Pos4.y, Pos8.x, Pos8.y, color);
		}
	}

	void Update()
	{
		if (g_pLTClientShell->GetPlayerClient() == 0)
			return;

		auto pLocal = GetLocalPlayer((DWORD)g_pLTClientShell);

		for (int i = 0; i <= 16; i++)
		{
			auto pEnemy = GetPlayerByIndex((DWORD)g_pLTClientShell, i);

			if (pEnemy == 0)
				continue;

			if (pEnemy->iHealth <= 0)
				continue;

			if (pEnemy->iClientID == pLocal->iClientID)
				continue;

			D3DXVECTOR3 vHead		= { 0.0F, 0.0F, 0.0F };
			D3DXVECTOR3 vBody		= { 0.0F, 0.0F, 0.0F };
			D3DXVECTOR3 vFoot		= { 0.0F, 0.0F, 0.0F };

			D3DVIEWPORT9 pViewport = { 0 };

			Device->GetViewport(&pViewport);

			DWORD ScreenCenterX = pViewport.Width / 2;
			DWORD ScreenCenterY = pViewport.Height;

			if (!GetBonePosition(pEnemy->iObject, "M-bone Head", &vHead, true))
				continue;

			if (!GetBonePosition(pEnemy->iObject, "M-bone pelvis", &vBody, true))
				continue;

			if (!GetBonePosition(pEnemy->iObject, "M-bone R Foot", &vFoot, true))
				continue;

			if (!WorldToScreen(&vHead, &vHead) && !WorldToScreen(&vBody, &vBody) && !WorldToScreen(&vFoot, &vFoot))
				continue;

			//ESP Distance
			std::function< float( D3DXVECTOR3, D3DXVECTOR3 ) > CalcDistance = [](D3DXVECTOR3 p1, D3DXVECTOR3 p2)
			{
				float diffY = p1.y - p2.y;
				float diffX = p1.x - p2.x;
				return std::sqrt((diffY * diffY) + (diffX * diffX));
			};

			pRenderer->String(vFoot.x, vFoot.y + 10.0F, White, g_pMainFont, true, "Distance: %0.2F", CalcDistance(pLocal->pObjectFX->Position, pEnemy->pObjectFX->Position));

			//ESP Lines
			pRenderer->Line(ScreenCenterX, ScreenCenterY, vBody.x, vBody.y, White);

			//ESP names
			auto Center = pRenderer->StringWidth(g_pMainFont, pEnemy->szName);
			pRenderer->String((vHead.x - Center) / 2.0F, vHead.y, White, g_pMainFont, true, pEnemy->szName);

			//ESP health
			pRenderer->String(vHead.x, vHead.y - 10.0F, White, g_pMainFont, true, "Health: %d", pEnemy->iHealth);

			//ESP C4
			if (pEnemy->bHasC4)
				pRenderer->String(vHead.x, vHead.y - 5.0F, White, g_pMainFont, true, "C4");

			//2D Boxes
			Box2D(vHead, vFoot, White);

			//3D Boxes
			Box3D(pEnemy->iObject, White);

			//Skeleton
			Bones(pEnemy->iObject, White);

			auto pWeaponFX = g_pLTClientShell->GetDroppedFX();
			if (pWeaponFX == 0)
				continue;

			D3DXVECTOR3 vWeapon = { 0.0F, 0.0F, 0.0F };

			//incase LT_OK...
			g_pLTClient->GetObjectPos(pWeaponFX->cObject0, &vWeapon);

			if (!WorldToScreen(&vWeapon, &vWeapon))
				continue;

			pRenderer->String( vWeapon.x, vWeapon.y, White, g_pMainFont, true, "Weapon" );

			auto pNPC = g_pLTClientShell->GetAIObject();
			if (pNPC == 0)
				continue;

			D3DXVECTOR3 vNPCPos = { 0.0F, 0.0F, 0.0F };

			//incase LT_OK...
			g_pLTClient->GetObjectPos(pNPC->Object, &vNPCPos);

			if (!WorldToScreen(&vNPCPos, &vNPCPos))
				continue;

			pRenderer->String(vNPCPos.x, vNPCPos.y, White, g_pMainFont, true, "NPC");
		}
	}
}