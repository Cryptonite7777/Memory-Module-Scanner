#include "ModuleScanner.h"
#include "PatternDB.h"
#include <iostream>
#include <tlhelp32.h>



ModuleScanner::~ModuleScanner()
{
	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = nullptr;
	}
}

// 프로세스 핸들 얻기 - VirtualQueryEx
bool ModuleScanner::Attach(DWORD pid)
{
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, pid);

	if (hProcess != NULL) { return true; }
	return false;
}



// 프로세스 내 모듈 정보 얻기 - PID 기준
std::vector<ModuleInfo> ModuleScanner::EnumModules(DWORD pid)
{
	std::vector<ModuleInfo> out;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

	MODULEENTRY32W me = { 0 };
	me.dwSize = sizeof(me);

	if (Module32FirstW(hSnap, &me)) {
		do {

			ModuleInfo Temp;
			Temp.BaseAddr = me.modBaseAddr;
			Temp.BaseSize = me.modBaseSize;
			Temp.Name = me.szModule;
			Temp.Path = me.szExePath;
			out.push_back(Temp);

		} while (Module32NextW(hSnap, &me));
	}

	if (hSnap) {
		CloseHandle(hSnap);
		hSnap = nullptr;
	}

	return out;
}


// 전체 모듈 지정 + Region 순회 
bool ModuleScanner::ScanModule(const ModuleInfo& mod)
{
	// 범위 지정 ( 프로세스내 모듈만 )
	BYTE* Start_addr = mod.BaseAddr;
	BYTE* End_addr = mod.BaseAddr + mod.BaseSize;


	while (Start_addr < End_addr) {
		MEMORY_BASIC_INFORMATION mbi = { 0 };

		if (VirtualQueryEx(hProcess, Start_addr, &mbi, sizeof(mbi)) == 0)
		{
			return false;
		}

		if ((mbi.State == MEM_COMMIT) &&
			(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)))
		{
			ScanModulePages((BYTE*)mbi.BaseAddress, mbi.RegionSize, mod.BaseAddr);
		}

		Start_addr = (BYTE*)mbi.BaseAddress + mbi.RegionSize;
	}

	return true;
}


// 모듈 내부 단위 패턴 검사
void ModuleScanner::ScanModulePages(BYTE* base, SIZE_T size, const BYTE* Module_base)
{
	if (!hProcess)
	{
		std::cout << "Attach되지 않음.\n";
		return;
	}


	std::vector<BYTE> buffer(size);
	SIZE_T BytesRead = 0;

	if (ReadProcessMemory(hProcess, base, buffer.data(), size, &BytesRead)) {

		wprintf(L" [Region] Base: %p  Size: 0x%zX\n", base, BytesRead);

		auto& Patterns = GetAllPatterns();  // 전체 패턴 DB

		for (auto& it : Patterns)
		{
			const auto& key = it.first;       // 패턴 이름
			const auto& sig = it.second;      // 패턴 바이트

			for (SIZE_T i = 0; i + sig.size() <= BytesRead; i++)
			{
				bool found = true;

				for (SIZE_T j = 0; j < sig.size(); j++)
				{
					if (buffer[i + j] != sig[j])
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					BYTE* foundAddr = base + i;
					SIZE_T moduleoffset = (SIZE_T)foundAddr - (SIZE_T)Module_base;

					wprintf(L"[+] Pattern Found : [%ws]\n", key.c_str());
					wprintf(L"     VA            : %p\n", foundAddr);
					wprintf(L"     Region Offset : 0x%zX\n", i);
					wprintf(L"     Module Offset : 0x%zX\n", moduleoffset);
					wprintf(L"--------------------------------------\n");
				}
			}
		}
	}

}
