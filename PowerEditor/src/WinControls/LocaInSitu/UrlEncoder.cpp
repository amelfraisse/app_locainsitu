#include "UrlEncoder.h"

#include <math.h>

CURLEncode::CURLEncode(string  sStringToConvert)
{
	_sStringToConvert = sStringToConvert;
	convert();
}

bool CURLEncode::isUnsafe(char compareChar)
{
	// UNSAFE String
	string csUnsafeString= "\"<>%\\^[]`+$,@:;/!#?=&";

	bool bcharfound = false;
	char tmpsafeChar;
	int m_strLen = 0;
	
	m_strLen = csUnsafeString.size();
	for(int i = 0; i < m_strLen ;i++)
	{
		tmpsafeChar = csUnsafeString[i]; 
		if(tmpsafeChar == compareChar)
		{ 
			bcharfound = true;
			break;
		} 
	}
	int char_ascii_value = 0;
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
	{
		return false;
	}
	// found no unsafe chars, return false		
	else
	{
		return true;
	}
	
	return true;
}

string CURLEncode::decToHex(char num, int radix)
{
	// HEX Values array
	char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	int temp=0;	
	string csTmp;
	int num_char;
	num_char = (int) num;
	
	// ISO-8859-1 
	// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
	// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
	if (num_char < 0)
		num_char = 256 + num_char;

	while (num_char >= radix)
    {
		temp = num_char % radix;
		double dTemp = num_char / radix;
		num_char = (int)floor(dTemp);
		csTmp = hexVals[temp];
    }
	
	csTmp += hexVals[num_char];

	if(csTmp.size() < 2)
	{
		csTmp += '0';
	}

	string strdecToHex;
	// Reverse the String
	for (int i = csTmp.size() - 1; i >= 0; i--)
	{
		strdecToHex += csTmp[i];
	}
	
	return strdecToHex;
}

string CURLEncode::convert(char val)
{
	string csRet;
	csRet += "%";
	csRet += decToHex(val, 16);
	return  csRet;
}

void CURLEncode::convert()
{
	_sConvertedString = "";
	for (int i = 0; i < _sStringToConvert.size(); i++)
	{
		if (isUnsafe(_sStringToConvert[i]))
			_sConvertedString += convert(_sStringToConvert[i]);
		else
			_sConvertedString += _sStringToConvert[i];
	}
}

string CURLEncode::GetConvertedString()
{
	return _sConvertedString;
}
