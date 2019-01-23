#include "includes.h"

#define M_PI   3.14159265358979323846264338327950288

namespace Math
{
	__forceinline float AngleNormalize(float x)
	{
		x = std::fmod(x + 180, 360);
		if (x < 0)
			x += 360;
		return x - 180;
	}

	inline void VectorAngles(const D3DXVECTOR3 forward, D3DXVECTOR2& angles) 
	{
		float tmp;

		if (forward[2] == 0 && forward[0] == 0)
		{
			angles[1] = 0;
			angles[0] = -90;
		}
		else
		{
			angles[1] = AngleNormalize((std::atan2f(forward[2], -forward[0]) * 180 / M_PI) - 90);

			tmp = std::sqrtf(forward[0] * forward[0] + forward[2] * forward[2]);
			angles[0] = AngleNormalize(std::atan2f(-forward[1], tmp) * 180 / M_PI);
		}
	}

	inline void GetAngleToTarget(D3DXVECTOR3 target, D3DXVECTOR3 camera, D3DXVECTOR2& angles) 
	{
		D3DXVECTOR3 delta = target - camera;
		VectorAngles(delta, angles);
	}

	inline bool GetFieldOfView(float angle, D3DXVECTOR3 camera, D3DXVECTOR3 target, D3DXVECTOR2& newAngles)
	{
		auto pmanager = g_pLTClientShell->GetPlayerClient();

		if (!pmanager)
			return false;

		angle /= 2;

		VectorAngles(target - camera, newAngles);

		auto curAngles = D3DXVECTOR2(AngleNormalize(D3DXToDegree(pmanager->GetPitch())), AngleNormalize(D3DXToDegree(pmanager->GetYaw())));

		return (newAngles[1] >= (curAngles[1] - angle) &&
			newAngles[1] <= (curAngles[1] + angle) &&
			newAngles[0] >= (curAngles[0] - angle) &&
			newAngles[0] <= (curAngles[0] + angle));
	}
}