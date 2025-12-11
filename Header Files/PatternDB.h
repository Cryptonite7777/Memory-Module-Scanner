#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <unordered_map>

// 전체 패턴 반환용
std::unordered_map<std::wstring, std::vector<BYTE>>& GetAllPatterns();

