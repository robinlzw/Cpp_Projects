#include "../header/MyStringFormat.h"

MyStringFormat::MyStringFormat(void)
{
}

MyStringFormat::~MyStringFormat(void)
{
}


//ascii ת Unicode 
wstring MyStringFormat::Acsi2WideByte(string& strascii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return wstring(&resultstring[0]);
}
//Unicode ת Utf8 
string MyStringFormat::Unicode2Utf8(const wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(utf8size);
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);
	if (convresult != utf8size)
	{
		throw std::exception("La falla!");
	}
	return string(&resultstring[0]);
}
//ascii ת Utf8 
string MyStringFormat::ASCII2UTF_8(string& strAsciiCode)
{
	string strRet("");
	//�Ȱ� ascii תΪ unicode 
	wstring wstr = Acsi2WideByte(strAsciiCode);
	//���� unicode תΪ utf8 
	strRet = Unicode2Utf8(wstr);
	return strRet;
}

//UTF-8תUnicode 
wstring MyStringFormat::Utf82Unicode(const string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return wstring(&resultstring[0]);
}
//unicode תΪ ascii 
string MyStringFormat::WideByte2Acsi(wstring wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}
	return string(&resultstring[0]);
}
//utf-8 ת ascii 
string MyStringFormat::UTF_82ASCII(string& strUtf8Code)
{
	string strRet("");
	//�Ȱ� utf8 תΪ unicode 
	wstring wstr = Utf82Unicode(strUtf8Code);
	//���� unicode תΪ ascii 
	strRet = WideByte2Acsi(wstr);
	return strRet;
}