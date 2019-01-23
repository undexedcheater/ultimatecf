#pragma once

typedef UINT( __stdcall *threadFunc_t )( void* );

extern void* Thread_StartAddr;
extern uint8_t bytes[6];

namespace Tools
{
	bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, std::string szMask);
	DWORD GetModuleBaseAddress(std::string szModuleName);
	DWORD GetModuleSizeAndBaseAddr(std::string szModuleName, DWORD *szModuleSize);
	DWORD SearchPattern(std::string szModuleName, BYTE* bMask, std::string szMask);
	bool CanReadRTTI(DWORD address, std::string &rtti);
	void __cdecl AddToLogFile(const char *fmt, ...);
	void MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen);
	void MakeCALL(BYTE* paddress, DWORD yourfunction, DWORD dwlen);
	void MemoryMod(void* dst, void* src, size_t size);
	void EraseHeaders(HMODULE hModule);
	HANDLE CreateStealthThread(threadFunc_t pThreadFunc, void *pArgument );
}