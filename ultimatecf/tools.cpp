#include "includes.h"

#include <fstream>

#define LOG_FILE		"C:\\Users\\Public\\CrossFire_Log.log"

void* Thread_StartAddr = nullptr;
uint8_t bytes[6] = { 0 };

namespace Tools
{
	bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, std::string szMask)
	{
		int maskLen = szMask.length();

		for (DWORD i = 0; i < dwLen - maskLen; i++)
			if (Compare((BYTE*)(dwAddress + i), bMask, szMask.c_str()))
				return (DWORD)(dwAddress + i);
		return 0;
	}

	DWORD GetModuleBaseAddress(std::string szModuleName)
	{
		/*PLDR_DATA_ENTRY pFirstEntry = nullptr;
		DWORD dwModuleBase = 0;

		__asm
		{
			mov eax, fs:[0x30]
			mov eax, [eax + 0x0C]
			mov eax, [eax + 0x1C]
			mov pFirstEntry, eax;
		}

		while (pFirstEntry->BaseAddress)
		{
			size_t iStrLen = pFirstEntry->BaseDllName.Length + 1;
			auto szOutStr = new char[iStrLen];

			size_t iCharsOut = 0;
			wcstombs_s(&iCharsOut, szOutStr, iStrLen, pFirstEntry->BaseDllName.Buffer, iStrLen - 1);

			if (!szModuleName.compare(szOutStr))
			{
				delete[] szOutStr;
				dwModuleBase = reinterpret_cast<DWORD>(pFirstEntry->BaseAddress);
				break;
			}

			delete[] szOutStr;

			pFirstEntry = (PLDR_DATA_ENTRY)pFirstEntry->InMemoryOrderModuleList.Flink;
		}
		return dwModuleBase;*/
		return reinterpret_cast<DWORD>(GetModuleHandleA(szModuleName.c_str()));
	}

	DWORD GetModuleSizeAndBaseAddr(std::string szModuleName, DWORD *szModuleSize)
	{
		DWORD module = GetModuleBaseAddress(szModuleName.c_str());

		auto pImageDos = (IMAGE_DOS_HEADER*)module;
		auto pImageNt = (IMAGE_NT_HEADERS32*)(module + pImageDos->e_lfanew);

		*szModuleSize = pImageNt->OptionalHeader.SizeOfImage;
		return module;
	}

	DWORD SearchPattern(std::string szModuleName, BYTE* bMask, std::string szMask)
	{
		DWORD ModuleSize = 0;
		auto ModuleBase = GetModuleSizeAndBaseAddr(szModuleName, &ModuleSize);

		return FindPattern(ModuleBase, ModuleSize, bMask, szMask);
	}

	bool CanReadRTTI(DWORD address, std::string &rtti)
	{
		std::string stringOut = " ";

		DWORD dwObjectLocatorPtr = *(DWORD *)(address - sizeof(DWORD));

		if (dwObjectLocatorPtr == 0)
			return false;

		DWORD dwClassHierarchyDescriptor = *(DWORD *)(dwObjectLocatorPtr + 0x10);

		if (dwClassHierarchyDescriptor == 0)
			return false;

		auto iBassClassCnt = *(int *)(dwClassHierarchyDescriptor + 8);

		if (iBassClassCnt > 0 && iBassClassCnt <= 25)
		{
			DWORD BaseClassArray = *(DWORD *)(dwClassHierarchyDescriptor + 0xC);

			if (BaseClassArray == 0)
				return false;

			for (auto i = 0; i < iBassClassCnt; ++i)
			{
				DWORD dwBaseClassDescriptorPtr = *(DWORD *)(BaseClassArray + (4 * i));

				if (dwBaseClassDescriptorPtr != 0)
				{
					CTypeDescriptor* typeDescriptorPtr = (CTypeDescriptor *)*(DWORD *)dwBaseClassDescriptorPtr;

					if (typeDescriptorPtr != nullptr)
					{
						std::string r_name = typeDescriptorPtr->class_name;

						//the first RTTI ends with @@
						std::size_t found = r_name.find_last_of("@@");
						r_name = r_name.substr(0, found - 1);

						//clean the '@@' and append it to our string
						stringOut.append(r_name);
						stringOut.append(" : ");

						continue;
					}
				}

				break;
			}

			//remove the last 3 chars from the string cuz it's ' : '
			if (!stringOut.empty())
				stringOut = stringOut.substr(0, stringOut.size() - 3);
		}

		rtti = stringOut;
		return true;
	}

	void __cdecl AddToLogFile(const char *fmt, ...)
	{
		std::ofstream ofile;
		ofile.open(LOG_FILE, std::ios::app);

		va_list va_alist;
		char logbuf[256] = { 0 };

		va_start(va_alist, fmt);
		vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);

		ofile << logbuf << std::endl;

		ofile.close();
	}

	void MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
	{
		DWORD dwOldProtect, dwRelAddr;
		VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;
		*pAddress = 0xE9;
		*((DWORD *)(pAddress + 0x1)) = dwRelAddr;
		for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;
		VirtualProtect(pAddress, dwLen, dwOldProtect, &dwOldProtect);
		return;
	}

	void MakeCALL(BYTE* paddress, DWORD yourfunction, DWORD dwlen)
	{
		DWORD dwOldProtect, dwBkup, dwRelAddr;
		VirtualProtect(paddress, dwlen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		dwRelAddr = (DWORD)(yourfunction - (DWORD)paddress) - 5;
		*paddress = 0xE8;
		*((DWORD*)(paddress + 0x1)) = dwRelAddr;
		for (DWORD x = 0x5; x < dwlen; x++) *(paddress + x) = 0x90;
		VirtualProtect(paddress, dwlen, dwOldProtect, &dwBkup);
		return;
	}

	void MemoryMod(void* dst, void* src, size_t size)
	{
		DWORD dwOld = 0;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOld);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, dwOld, &dwOld);
	}

	void EraseHeaders(HMODULE hModule)
	{
		DWORD dwModule = reinterpret_cast<DWORD>(hModule);
		
		PIMAGE_DOS_HEADER		pImageDOS		= (IMAGE_DOS_HEADER*)dwModule;
		PIMAGE_NT_HEADERS32		pImageNTHeader	= (IMAGE_NT_HEADERS32*)( dwModule + pImageDOS->e_lfanew );

		DWORD dwOld = 0;
		auto header_size = pImageNTHeader->OptionalHeader.SizeOfHeaders;

		VirtualProtect(reinterpret_cast<void *>(dwModule), header_size, PAGE_EXECUTE_READWRITE, &dwOld);
		RtlSecureZeroMemory(reinterpret_cast<void *>(dwModule), header_size);
		VirtualProtect(reinterpret_cast<void *>(dwModule), header_size, PAGE_NOACCESS, &dwOld);
	}

	HANDLE CreateStealthThread(threadFunc_t pThreadFunc, void *pArgument )
	{
		BYTE *pK32 = ( BYTE *)GetModuleHandleA( "ntdll.dll" );

		auto rva2va = [&](DWORD dwVA) { return (void*)((uintptr_t)pK32 + dwVA); };

		// Find propper location to place our shellcode
		void *pWriteTarget = nullptr;

		auto pMz = (IMAGE_DOS_HEADER*)pK32;
		auto pNt = (IMAGE_NT_HEADERS32*)rva2va(pMz->e_lfanew);
		auto pCurSection = (IMAGE_SECTION_HEADER*)((uintptr_t)pNt + sizeof(IMAGE_NT_HEADERS32));

		for (int i = 0; i < pNt->FileHeader.NumberOfSections; ++i)
		{
			if (memcmp(".text", pCurSection->Name, 5) == 0)
			{
				pWriteTarget = (void*)((uintptr_t)rva2va(pCurSection->VirtualAddress) + pCurSection->Misc.VirtualSize - 6);
				break;
			}
			++pCurSection;
		}

		if (!pWriteTarget)
			return NULL;

		printf("WriteLoc: %p\n", pWriteTarget);

		// Prepare and write shellcode to K32
		uint8_t shellcode[]			= "\x68\x00\x00\x00\x00\xC2";

		DWORD dwOld = 0;
		*( threadFunc_t *)( shellcode + 1 ) = pThreadFunc;
		VirtualProtect(pWriteTarget, 6, PAGE_EXECUTE_READWRITE, &dwOld);

		//make backup first
		Thread_StartAddr = pWriteTarget;
		for (int i = 0; i < 6; ++i)
			bytes[i] = *(uint8_t *)pWriteTarget + i;

		//do magic
		memcpy(pWriteTarget, shellcode, 6);
		
		VirtualProtect(pWriteTarget, 6, dwOld, &dwOld);

		DWORD tid = 0;

		//createthread normally
		auto hThread = CreateThread( nullptr, 0, (LPTHREAD_START_ROUTINE)pWriteTarget, pArgument, NULL, &tid);
		printf("Thread ID: %d\n", tid);

		return hThread;
	}
}