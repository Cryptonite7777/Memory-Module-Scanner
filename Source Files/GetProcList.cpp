#include "GetProcList.h"
#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>


// [PID]: Name 출력
void PrintProcList()
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		wprintf(L"INVALID_HANDLE_VALUE\n");
		return;
	}

	PROCESSENTRY32W me = { 0 };
	me.dwSize = sizeof(me);

	if (Process32FirstW(hSnap, &me)) {
		do {
			wprintf(L"[%lu]: %ws\n", me.th32ProcessID, me.szExeFile);

		} while (Process32NextW(hSnap, &me));
	}

	if (hSnap) {
		CloseHandle(hSnap);
		hSnap = nullptr;
	}
}

