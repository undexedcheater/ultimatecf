#pragma once

namespace Tools
{
	bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, std::string szMask);
	DWORD GetModuleBaseAddress(std::string szModuleName);
	DWORD GetModuleSizeAndBaseAddr(std::string szModuleName, DWORD *szModuleSize);
	DWORD SearchPattern(std::string szModuleName, BYTE* bMask, std::string szMask);
	bool CanReadRTTI(DWORD address, std::string &rtti);
}