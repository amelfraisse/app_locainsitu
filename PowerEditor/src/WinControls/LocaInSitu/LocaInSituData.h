#ifndef _LOCA_IN_SITU_DATA_H
#define _LOCA_IN_SITU_DATA_H

#include "windows.h"
#include <string>
#include <vector>

enum ResType {
	Undefined,
	DialogTitle,
	DialogControl,
	MainMenuItem,
	MainMenuSubItem
};

using namespace std;

typedef struct {
	wstring sTrans;
	int		iQualityTrans;
	wstring sComment;
	wstring sDate;
	wstring sFirstName;
	wstring sSecondName;
	int		iUserQuality;
	int		iUserQualityCalculated;
	//Add here additional information
} SectraUnit;

class LocaInSituData
{
public:
	LocaInSituData();
	LocaInSituData& operator=(const LocaInSituData& LocaData);
	//Accessors
	ResType GetResType();
	HWND GetHWnd();
	string GetWindowTagName();
	UINT GetID();
	string GetCurrentTrans();
	string GetNewTrans();
	void SetNewTrans(string sNewTrans);
	string GetSourceTrans();
	vector<string>& GetXLiffTranslations();
	vector<SectraUnit>& GetSectraTranslations();

	void ClearAll();
	void SetUIParams(ResType reType, HWND hWnd, string sTagName, UINT uID, string sCurrentTrans);
	bool UpdateSourceTrans();

	bool RetrieveXliffTranslations();
	int GetIdProject();
	void SetIdProject(int iId);
	int GetIdUser();
	void SetIdUser(int iId);	
	string GetComment();
	void SetComment(string sComment);
	int GetQuality();
	void SetQuality(int iQuality);
	SYSTEMTIME GetTime();
	void SetTime(SYSTEMTIME Time);

protected:
	ResType _ResType;
	HWND _hWnd;
	string _sWindowTagName;
	UINT _uItemID;
	string _sCurrentTrans;

	string _sNewTrans;
	int    _iQuality;
	string _sComment;
	SYSTEMTIME _Time; 

	string _sSourceTrans;
	vector<string> _vectXliffTranslations;

	vector<SectraUnit> _vectSectraTranslations;

	int _iIdProject;
	int _iIdUser;
};

#endif// _LOCA_IN_SITU_DATA_H