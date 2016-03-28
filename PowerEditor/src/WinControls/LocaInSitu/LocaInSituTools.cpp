#include "LocaInSituTools.h"
#include "URLEncoder.h"


std::wstring LocaInSituTools::s2ws(const std::string& s, boolean bUtf8)
{
	int len;
	int slength = (int)s.length() + 1;

	DWORD dCP = CP_ACP;
	if (bUtf8 == true)
		dCP = CP_UTF8;

	len = MultiByteToWideChar(dCP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(dCP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	return r;
}

std::string LocaInSituTools::ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0); 
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;

	return r;
}

void LocaInSituTools::TrimLeft(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_first_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(0,pos);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}

void LocaInSituTools::TrimLeft(std::wstring& str, const wchar_t* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_first_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(0,pos);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}

void LocaInSituTools::TrimRight(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_last_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(pos+1);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}

void LocaInSituTools::TrimRight(std::wstring& str, const wchar_t* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_last_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(pos+1);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}

std::string EncodeUrl(const std::string& sStr)
{
	CURLEncode UrlEncoder(sStr);
	return UrlEncoder.GetConvertedString();
}