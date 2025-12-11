#include "PatternDB.h"


static std::unordered_map<std::wstring, std::vector<BYTE>> Patterns{

	// "virus" 시그니처
	{L"CUSTOM_SIG", {0x76, 0x69, 0x72, 0x75, 0x73}},

	// "UPX!" 시그니처 
	{ L"UPX_SIG", { 'U', 'P', 'X', '!' } },

	// "DEADCODE" 시그니처
	{ L"FAKE_PACKER_SIG", { 0xDE, 0xAD, 0xC0, 0xDE } },

};


std::unordered_map<std::wstring, std::vector<BYTE>>& GetAllPatterns()
{
	return Patterns;
}
