#pragma once

namespace Math
{
	void VectorAngles(const D3DXVECTOR3 forward, D3DXVECTOR2& angles);
	float AngleNormalize(float x);
	void GetAngleToTarget(D3DXVECTOR3 target, D3DXVECTOR3 camera, D3DXVECTOR2& angles);
	bool GetFieldOfView(float angle, D3DXVECTOR3 camera, D3DXVECTOR3 target, D3DXVECTOR2& newAngles);
}