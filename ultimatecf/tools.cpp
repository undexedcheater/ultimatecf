#include "includes.h"

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
}