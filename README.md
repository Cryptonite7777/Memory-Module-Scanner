# Memory Module Scanner

A simple memory module scanner for learning PE structure and analyzing how modules are mapped in process memory.

특정 프로세스에 로드된 모듈(EXE + DLL)을 출력하고 각 모듈이 메모리에 로드된 상태에서 어떤 구조와 바이트 데이터를 가지고 있는지 직접 확인하기  
위해 제작한 학습용 토이 프로젝트입니다.

PE 구조를 공부하면서 파일 기반 분석(PEVIEW, HxD)만으로는 메모리 로드 이후의 실제 구조를 알기 어렵다는 점이 흥미로웠고  
프로세스에 로드된 모듈의 메모리 덤프를 직접 읽고 간단한 정적 패턴을 탐지하는 기능을 구현해보았습니다.

## Features

- 프로세스 목록 출력
- 특정 PID에 Attach하여 메모리 읽기
- 모듈(EXE + DLL) 출력
- VirtualQueryEx 기반 메모리 Region 순회
- ReadProcessMemory 기반 모듈 내부 메모리 덤프
- 사용자 정의 패턴(Signature) 탐지
- PatternDB.cpp에서 패턴 추가 가능

## How It Works

1. 실행하면 현재 프로세스 목록을 출력합니다.
2. 스캔할 PID를 입력합니다.
3. 해당 프로세스에 Attach되면 로드된 모듈들의 정보를 출력합니다.
4. 각 모듈의 메모리 페이지를 VirtualQueryEx로 확인합니다.
5. ReadProcessMemory로 데이터를 읽고 지정된 패턴이 존재하는지 검사합니다.
6. 패턴이 발견되면 VA 주소, Region Offset, Module Offset을 함께 출력합니다.

## Example Output 1/2

```
PID: 1234

Module: Example.exe     Base: 00007FF720670000  Size: C000      Path: C:\\Example.exe
--------------------------------------

Module: ntdll.dll       Base: 00007FF90D0B0000  Size: 1F8000    Path: C:\\Windows\\SYSTEM32\\ntdll.dll
--------------------------------------

Module: KERNEL32.DLL    Base: 00007FF90C040000  Size: C2000     Path: C:\\Windows\\System32\\KERNEL32.DLL
--------------------------------------
```

## Example Output 2/2

```
======================================
[SCAN] Example.exe
--------------------------------------
 [Region] Base: 00007FF720670000  Size: 0x1000
 [Region] Base: 00007FF720671000  Size: 0x4000
[+] Pattern Found : [UPX_SIG]
     VA            : 00007FF7206710E1
     Region Offset : 0xE1
     Module Offset : 0x10E1
--------------------------------------
[+] Pattern Found : [FAKE_PACKER_SIG]
     VA            : 00007FF720671183
     Region Offset : 0x183
     Module Offset : 0x1183
--------------------------------------
 [Region] Base: 00007FF720675000  Size: 0x3000
 [Region] Base: 00007FF720678000  Size: 0x1000
 [Region] Base: 00007FF720679000  Size: 0x3000

======================================
[SCAN] ntdll.dll
--------------------------------------
 [Region] Base: 00007FF90D0B0000  Size: 0x1000
 [Region] Base: 00007FF90D0B1000  Size: 0x11C000
 [Region] Base: 00007FF90D1CD000  Size: 0x49000
 [Region] Base: 00007FF90D216000  Size: 0x1000
 [Region] Base: 00007FF90D219000  Size: 0x9000
 [Region] Base: 00007FF90D222000  Size: 0x86000

======================================
[SCAN] KERNEL32.DLL
--------------------------------------
 [Region] Base: 00007FF90C040000  Size: 0x1000
 [Region] Base: 00007FF90C041000  Size: 0x81000
 [Region] Base: 00007FF90C0C2000  Size: 0x35000
 [Region] Base: 00007FF90C0F7000  Size: 0x2000
 [Region] Base: 00007FF90C0F9000  Size: 0x9000

======================================
```

## Project Structure

```
Source Files/
 ├─ main.cpp
 ├─ GetProcList.cpp
 ├─ ModuleScanner.cpp
 ├─ PatternDB.cpp

Header Files/
 ├─ GetProcList.h
 ├─ ModuleScanner.h
 ├─ PatternDB.h
```

## Build

- Visual Studio 2022
- Windows 11
- C++17
