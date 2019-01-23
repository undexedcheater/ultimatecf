#pragma once

//unl chat 80 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? ?? ?? ?? B8
namespace GameHelper
{
	IDirect3DDevice9* Device;

	typedef bool(WINAPIV * tIntersectSegment)(IntersectQuery& iQuery, IntersectInfo* qInfo);
	extern tIntersectSegment oIntersectSegment;

	//Set outline around the player
	void SetObjectGlow(int* Obj, bool Enabled, uint8_t r, uint8_t g, uint8_t b);
	bool WorldToScreen(D3DXVECTOR3 *vWorldLocation, D3DXVECTOR3 *vScreenCoord);
	bool GetBonePosition(int* hObject, const char *szBoneName, D3DXVECTOR3 *out, bool bWorldSpace);
	bool GetBonePosition(int* hObject, uint32_t iBoneID, D3DXVECTOR3 *out, bool bWorldSpace);
	void DrawBone(int *Obj, UINT iStart, UINT iEnd, D3DCOLOR Color);
}