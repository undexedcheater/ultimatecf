#include "includes.h"

using namespace GameHelper;

namespace Aimbot
{
	CPlayerInfo* GetBestPlayerByFOV(float FOV)
	{
		auto PlayerClient = g_pLTClientShell->GetPlayerClient();

		if (PlayerClient == 0)
			return;

		CPlayerInfo* pLocal = GetLocalPlayer();

		if (!pLocal)
			return;

		if (pLocal->iObject == 0)
			return;

		if (pLocal->iHealth <= 0)
			return;

		CPlayerInfo* pBestPlayer = nullptr;

		for (int i = 0; i <= 16; i++)
		{
			CPlayerInfo* pTarget = GetPlayerByIndex(i);

			if (!pTarget)
				continue;

			if (pTarget->iObject == 0)
				continue;

			if (pTarget->iHealth <= 0)
				continue;

			if (pTarget->iClientID == pLocal->iClientID)
				continue;

			D3DXVECTOR3 vHeadPos = { 0.0F, 0.0F, 0.0F };

			if (!GetBonePosition(pTarget->iObject, "M-bone Head", &vHeadPos, true))
				continue;

			D3DXVECTOR3 vLocalCamera = { 0.0F, 0.0F, 0.0F };

			if (!GetBonePosition(pLocal->iObject, "M-bone Head", &vLocalCamera, true))
				continue;

			D3DXVECTOR2 vNewAgles = { 0.0F, 0.0F };

			if (Math::GetFieldOfView(FOV, vLocalCamera, vHeadPos, vNewAgles))
			{
				pBestPlayer = pTarget;
			}
		}
		return pBestPlayer;
	}

	CPlayerInfo* GetBestPlayerByDistance()
	{
		auto PlayerClient = g_pLTClientShell->GetPlayerClient();

		if (PlayerClient == 0)
			return;

		CPlayerInfo* pLocal = GetLocalPlayer();

		if (!pLocal)
			return;

		if (pLocal->iObject == 0)
			return;

		if (pLocal->iHealth <= 0)
			return;

		CPlayerInfo* pBestPlayer = nullptr;
		float flLowestDist = INFINITE;

		for (int i = 0; i <= 16; i++)
		{
			CPlayerInfo* pTarget = GetPlayerByIndex(i);

			if (!pTarget)
				continue;

			if (pTarget->iObject == 0)
				continue;

			if (pTarget->iHealth <= 0)
				continue;

			if (pTarget->iClientID == pLocal->iClientID)
				continue;

			D3DXVECTOR3 vHeadPos = { 0.0F, 0.0F, 0.0F };

			if (!GetBonePosition(pTarget->iObject, "M-bone Head", &vHeadPos, true))
				continue;

			D3DXVECTOR3 vLocalCamera = { 0.0F, 0.0F, 0.0F };

			if (!GetBonePosition(pLocal->iObject, "M-bone Head", &vLocalCamera, true))
				continue;

			std::function< float(D3DXVECTOR3, D3DXVECTOR3) > CalcDistance = [](D3DXVECTOR3 p1, D3DXVECTOR3 p2)
			{
				float diffY = p1.y - p2.y;
				float diffX = p1.x - p2.x;
				return std::sqrt((diffY * diffY) + (diffX * diffX));
			};

			float flCurDistance = CalcDistance(vLocalCamera, vHeadPos);

			if (flCurDistance < flLowestDist)
			{
				flLowestDist = flCurDistance;
				pBestPlayer = pTarget;
			}
		}
		return pBestPlayer;
	}

	void Update()
	{
		CPlayerInfo *pLocal = GetLocalPlayer();

		if (!pLocal)
			return;

		CPlayerInfo* pTarget = GetBestPlayerByDistance();

		if (!pTarget)
			return;

		D3DXVECTOR3 vHeadPos = { 0.0F, 0.0F, 0.0F };

		if (!GetBonePosition(pTarget->iObject, "M-bone Head", &vHeadPos, true))
			return;

		D3DXVECTOR3 vLocalCamera = { 0.0F, 0.0F, 0.0F };

		if (!GetBonePosition(pLocal->iObject, "M-bone Head", &vLocalCamera, true))
			return;

		D3DXVECTOR2 vAimAngles = { 0.0F, 0.0F };

		Math::GetAngleToTarget(vHeadPos, vLocalCamera, vAimAngles);

		//aimbot
		g_pLTClientShell->GetPlayerClient()->SetPlayerAngles(vAimAngles.x, vAimAngles.y);
	}
}

namespace RemoteKill
{
	bool MyFilterObjects(int* hObj, void* pUser)
	{
		for (int i = 0; i <= 16; i++)
		{
			CPlayerInfo* pPlayer = GetPlayerByIndex(i);

			if (!pPlayer)
				continue;

			if (pPlayer->iObject == 0)
				continue;

			if (hObj == pPlayer->iObject || hObj == pPlayer->pObjectFX->Object)
				return true;
		}
		return false;
	}

	bool IntersectAndPenetrateSegment(IntersectQuery* iQuery, IntersectInfo* qInfo)
	{
		bool Result = oIntersectSegment(*iQuery, qInfo);

		CPlayerInfo* pLocal = GetLocalPlayer();

		if (!pLocal)
			return Result;

		if (pLocal->iObject == 0)
			return Result;

		for (int i = 0; i <= 16; i++)
		{
			CPlayerInfo* pPlayer = GetPlayerByIndex(i);

			if (pLocal->iObject == pPlayer->iObject)
				continue;

			if (pPlayer->iHealth <= 0)
				continue;

			D3DXVECTOR3 HeadTransform = { 0.0F, 0.0F, 0.0F };

			if (!GetBonePosition(pPlayer->iObject, "M-bone Head", &HeadTransform, true))
				continue;

			iQuery->Start		= HeadTransform + D3DXVECTOR3(0.0f, 50.0f, 0.0f);
			iQuery->End			= HeadTransform - D3DXVECTOR3(0.0f, 50.0f, 0.0f);
			iQuery->FilterFn	= MyFilterObjects;

			if (Result)
			{
				qInfo->hObjImpact = pPlayer->pObjectFX->Object;
				qInfo->vImpactPos = HeadTransform;
			}
			return Result;
		}

		return Result;
	}
}