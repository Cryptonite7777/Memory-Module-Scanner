#pragma once
#include <Windows.h>
#include <vector>
#include <string>

// 모듈 정보 구조체
struct ModuleInfo
{
	BYTE* BaseAddr;         // 모듈 시작 주소
	DWORD BaseSize;         // 모듈 크기
	std::wstring Name;      // 모듈 이름
	std::wstring Path;      // 모듈 경로
};


class ModuleScanner
{
public:
	~ModuleScanner();

	// 프로세스에 Attach
	bool Attach(DWORD pid);

	// 모듈 열거
	std::vector<ModuleInfo> EnumModules(DWORD pid);

	// 특정 모듈 스캔 (패턴 검색)
	bool ScanModule(const ModuleInfo& mod);

private:
	HANDLE hProcess = nullptr;   // Attach한 프로세스 핸들

	// 모듈 내부 페이지 스캔 - 내부 함수
	void ScanModulePages(BYTE* base, SIZE_T size, const BYTE* Module_base);
};
