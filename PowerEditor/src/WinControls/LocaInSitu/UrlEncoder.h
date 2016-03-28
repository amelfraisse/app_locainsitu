#ifndef _URL_ENCODER_H
#define _URL_ENCODER_H

#include <string>
using namespace std;

class CURLEncode
{
private:
	string _sStringToConvert;
	string _sConvertedString;

	bool isUnsafe(char compareChar);
	string decToHex(char num, int radix);
	string convert(char val);
	void convert();

public:
	CURLEncode(string  sStringToConvert);
	virtual ~CURLEncode() { };
	string GetConvertedString();
};

#endif// _URL_ENCODER_H