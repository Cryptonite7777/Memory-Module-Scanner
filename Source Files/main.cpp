#include <iostream>
#include "ModuleScanner.h"
#include "GetProcList.h"

int main()
{

	PrintProcList(); // 프로세스 리스트 출력
	DWORD pid;
	std::wcout << "PID: ";
	std::wcin >> pid;

	ModuleScanner scanner;

	if (!scanner.Attach(pid))
	{
		wprintf(L"[%lu]: Attach Failed\n", pid);
		return 0;
	}


	//while (true) {
	auto modules = scanner.EnumModules(pid);

	// 모듈 정보 나열 
	for (const auto& s : modules)
	{

		wprintf(L"\nModule: %ws\tBase: %p\tSize: %lX\tPath: %ws\n",
			s.Name.c_str(),
			s.BaseAddr,
			s.BaseSize,
			s.Path.c_str());
		wprintf(L"--------------------------------------\n");
		//std::wcout << s.BaseAddr << "\t" << std::hex <<s.BaseSize << "\t" << s.Name << "\t" << s.Path << std::endl;
	}


	// 모듈 하나씩 순회
	for (const auto& m : modules)
	{
		//std::wcout << L"\n[SCAN] " << m.Name << std::endl;
		wprintf(L"\n======================================\n");
		wprintf(L"[SCAN] %ws\n", m.Name.c_str());
		wprintf(L"--------------------------------------\n");

		// 패턴 전체 다돌면 다음 모듈로 
		if (!scanner.ScanModule(m))
		{
			wprintf(L"Failed");
			return 0;
		}

	}
	/* wprintf(L"\n--- END SCAN CYCLE ---\n");
	 Sleep(5000);*/
	 // }

	system("Pause");
}
