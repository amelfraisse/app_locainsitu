#include "LocaInSituData.h"
#include "..\..\..\Notepad_plus.h"

LocaInSituData::LocaInSituData()
{
	_ResType = DialogTitle;
	_hWnd = NULL;
	_sWindowTagName.clear();
	_uItemID = -1;
	_sCurrentTrans.clear();

	_sNewTrans.clear();

	_sSourceTrans.clear();

	_sComment.clear();
	_iQuality = 5;
	//_Time;

	//TODO
	_iIdProject = 6;
	_iIdUser	= 1;
}

ResType LocaInSituData::GetResType()
{
	return _ResType;
}

HWND LocaInSituData::GetHWnd()
{
	return _hWnd;
}

string LocaInSituData::GetWindowTagName()
{
	return _sWindowTagName;
}

UINT LocaInSituData::GetID()
{
	return _uItemID;
}

string LocaInSituData::GetCurrentTrans()
{
	return _sCurrentTrans;
}

string LocaInSituData::GetNewTrans()
{
	return _sNewTrans;
}

void LocaInSituData::SetNewTrans(string sNewTrans)
{
	_sNewTrans = sNewTrans;
}

string LocaInSituData::GetSourceTrans()
{
	return _sSourceTrans;
}

vector<string>& LocaInSituData::GetXLiffTranslations()
{
	return _vectXliffTranslations;
}

vector<SectraUnit>& LocaInSituData::GetSectraTranslations()
{
	return _vectSectraTranslations;
}


string LocaInSituData::GetComment()
{
	return _sComment;
}

void LocaInSituData::SetComment(string sComment)
{
	_sComment = sComment;
}

int LocaInSituData::GetQuality()
{
	return _iQuality;
}

void LocaInSituData::SetQuality(int Quality)
{
	_iQuality = Quality;
}

SYSTEMTIME LocaInSituData::GetTime()
{
	return _Time;
}

void LocaInSituData::SetTime(SYSTEMTIME Time)
{
	_Time = Time;
}

void LocaInSituData::SetUIParams(ResType resType, HWND hWnd, string sTagName, UINT uID, string sCurrentTrans)
{
	ClearAll();

	_ResType		= resType;
	_hWnd			= hWnd;
	_sWindowTagName	= sTagName;
	_uItemID		= uID;
	_sCurrentTrans	= sCurrentTrans;
}

void LocaInSituData::ClearAll()
{
	_ResType = Undefined;
	_hWnd = NULL;
	_sWindowTagName.clear();
	_uItemID = -1;
	_sCurrentTrans.clear();

	_sNewTrans.clear();

	_sSourceTrans.clear();

	_iQuality = 5;

	//_Time = NULL;

	_sComment.clear();

	_vectXliffTranslations.clear();
	_vectSectraTranslations.clear();
}

bool LocaInSituData::UpdateSourceTrans()
{
	//If source trans is not stored in Xliff file, we store it
	string sSourceTrans;
	if (!Notepad_plus::getInstance()->GetSourceName(_ResType, _sWindowTagName, _uItemID, sSourceTrans))
	{
		if (!Notepad_plus::getInstance()->SetSourceName(_ResType, _sWindowTagName, _uItemID, _sCurrentTrans))
			return false;

		_sSourceTrans = _sCurrentTrans;
	}
	else
	{
		_sSourceTrans = sSourceTrans;
	}

	return true;
}

bool LocaInSituData::RetrieveXliffTranslations()
{
	//First Clera exising translations
	_vectXliffTranslations.clear();

	std::vector<string> ListString;
	if (!Notepad_plus::getInstance()->GetAllTranslation(_ResType, _sWindowTagName, _uItemID, ListString))
		return false;

	for(int i=0; i < ListString.size() ; i++)
	{
		_vectXliffTranslations.push_back(ListString[i]);
	}

	return true;
}

LocaInSituData& LocaInSituData::operator = (const LocaInSituData& LocaData) 
{ 
	_ResType		= LocaData._ResType;
	_hWnd			= LocaData._hWnd;
	_sWindowTagName = LocaData._sWindowTagName;
	_uItemID		= LocaData._uItemID;
	_sCurrentTrans	= LocaData._sCurrentTrans;

	_sNewTrans		= LocaData._sNewTrans;

	_sSourceTrans	= LocaData._sSourceTrans;

	_sComment		= LocaData._sComment;
	_iQuality		= LocaData._iQuality;
	_Time			= LocaData._Time;

	_vectXliffTranslations.clear();
	for (int i = LocaData._vectXliffTranslations.size()-1; i >=0 ; --i)
		_vectXliffTranslations.push_back(LocaData._vectXliffTranslations[i]);

	_vectSectraTranslations.clear();
	for (int i = LocaData._vectSectraTranslations.size()-1; i >=0 ; --i)
		_vectSectraTranslations.push_back(LocaData._vectSectraTranslations[i]);

	_iIdProject = LocaData._iIdProject;

	_iIdUser	= LocaData._iIdUser;

    return *this; 
}

int LocaInSituData::GetIdProject()
{ 
	return this->_iIdProject;
}

void LocaInSituData::SetIdProject(int iId)
{ 
	this->_iIdProject = iId;
}

int LocaInSituData::GetIdUser()
{ 
	return this->_iIdUser;
}

void LocaInSituData::SetIdUser(int iId)
{ 
	this->_iIdUser = iId;
}