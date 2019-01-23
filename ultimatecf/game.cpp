#include "includes.h"

namespace GameHelper
{
	tIntersectSegment oIntersectSegment = 0;

	//Set outline around the player
	void SetObjectGlow(int* Obj, bool Enabled, uint8_t r, uint8_t g, uint8_t b)
	{
		g_pLTClient->SetObjectOutLine(Obj, Enabled, r, g, b);
	}

	__declspec(noinline) bool WorldToScreen(D3DXVECTOR3 *vWorldLocation, D3DXVECTOR3 *vScreenCoord)
	{
		if (!vWorldLocation || !vScreenCoord)
			return false;

		D3DXMATRIX vecProj;
		D3DXMATRIX vecView;
		D3DXMATRIX vecWorld;

		D3DVIEWPORT9 viewPort;
		HRESULT hVP = Device->GetViewport(&viewPort);
		HRESULT hTV = Device->GetTransform(D3DTS_VIEW, &vecView);
		HRESULT hTP = Device->GetTransform(D3DTS_PROJECTION, &vecProj);
		HRESULT hTW = Device->GetTransform(D3DTS_WORLD, &vecWorld);

		if (FAILED(hVP) || FAILED(hTV) || FAILED(hTP) || FAILED(hTW))
			return false;

		D3DXVec3Project(
			vScreenCoord, vWorldLocation,
			&viewPort, &vecProj, &vecView, &vecWorld);

		return (vScreenCoord->z < 1.f);
	}

	__declspec(noinline) bool GetBonePosition(int* hObject, const char *szBoneName, D3DXVECTOR3 *out, bool bWorldSpace)
	{
		if (!szBoneName)
			return false;

		if (!hObject)
			return false;

		if (!out)
			return false;

		uint32_t hBoneNode;

		if (g_pLTClient->GetLTModel()->GetNode(hObject, szBoneName, hBoneNode))
		{
			LTransform transform;

			if (g_pLTClient->GetLTModel()->GetNodeTransform(hObject, hBoneNode, transform, bWorldSpace))
			{
				out->x = transform.m_vPos.x;
				out->y = transform.m_vPos.y;
				out->z = transform.m_vPos.z;

				return true;
			}
		}

		return false;
	}

	__declspec(noinline) bool GetBonePosition(int* hObject, uint32_t iBoneID, D3DXVECTOR3 *out, bool bWorldSpace)
	{
		if (!hObject)
			return false;

		if (!out)
			return false;

		LTransform transform;

		if (g_pLTClient->GetLTModel()->GetNodeTransform(hObject, iBoneID, transform, bWorldSpace))
		{
			out->x = transform.m_vPos.x;
			out->y = transform.m_vPos.y;
			out->z = transform.m_vPos.z;

			return true;
		}

		return false;
	}

	__declspec(noinline) void DrawBone(int *Obj, UINT iStart, UINT iEnd, D3DCOLOR Color)
	{
		D3DXVECTOR3 StartPos,		EndPos;
		D3DXVECTOR3 vOutStartPos,	vOutEndPos;

		GetBonePosition(Obj, iStart, &StartPos, true);
		GetBonePosition(Obj, iEnd, &EndPos, true);

		if (WorldToScreen(&StartPos, &vOutStartPos) &&
			WorldToScreen(&EndPos, &vOutEndPos))
		{
			pRenderer->Line(vOutStartPos.x, vOutStartPos.y, vOutEndPos.x, vOutEndPos.y, Color);
		}
	}

	__declspec(noinline) bool IsVisible(D3DXVECTOR3 Start, D3DXVECTOR3 End)
	{
		static IntersectQuery iQuery;
		static IntersectInfo iInfo;

		RtlSecureZeroMemory(&iQuery, sizeof(iQuery));

		iQuery.Start	= Start;
		iQuery.End		= End;

		return !oIntersectSegment(iQuery, &iInfo);
	}
}