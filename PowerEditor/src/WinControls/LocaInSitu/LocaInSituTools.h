#ifndef _LOCA_IN_SITU_TOOLS_H
#define _LOCA_IN_SITU_TOOLS_H

#include "windows.h"
#include <string>

namespace LocaInSituTools
{
	std::wstring s2ws(const std::string&  s, boolean bUtf8 = false);
	std::string  ws2s(const std::wstring& s);

	void TrimLeft(std::string& str, const char* chars2remove);
	void TrimLeft(std::wstring& str, const wchar_t* chars2remove);
	void TrimRight(std::string& str, const char* chars2remove);
	void TrimRight(std::wstring& str, const wchar_t* chars2remove);
	//std::string EncodeUrl(const std::string& sStr);
}

#endif// _LOCA_IN_SITU_TOOLS_H