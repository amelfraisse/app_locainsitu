#ifndef _LOCA_IN_SITU_H
#define _LOCA_IN_SITU_H

#include "Resource.h"
#include "StaticDialog.h"
#include "LocaInSituData.h"
#include <string>
#include <vector>

using namespace std;

//typedef struct {
//   HWND hWnd;
//   string sWindowTagName;
//   UINT uItemID;
//   string sSourceFile;
//   string sExistingFile;
//   string sNewFile;
//} LocaInSituStruct;

class LocaInSituDlg : public StaticDialog
{
public :
   LocaInSituDlg() : StaticDialog() {};

	void init(HINSTANCE hInst, HWND hPere) {
		Window::init(hInst, hPere);
	};

	virtual void create(int dialogID, bool isRTL = false) {
		StaticDialog::create(dialogID, isRTL);
	};

	void doDialog(bool isRTL = false) {
		if (!isCreated())
			create(IDD_LOCA_IN_SITU, isRTL);
		display();
		::SetFocus(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED));
	};

    virtual void display(bool toShow = true);

    void SetLocaParam(LocaInSituData LocaInSituData);

protected :
   virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void ApplyTranslation();
   void InitTranslations();
   bool DoRequest(	LPCWSTR sURLName,
					LPCWSTR sRequestVerb,
					LPCWSTR sRequestObjName,
					LPCWSTR sRequestVersion,
					LPCWSTR sRequestRefer,
					vector<wstring>& sRequestAnswer);

   bool DoRequestSubmit();
   bool DoRequestRetrieveAll();
   bool DoRequestGoToSectra();
   bool UpdateCurrentTransFromSectraList();

   bool DoRequestRetrieveAllSQL();
   bool DoRequestSubmitSQL();

   void ExtractTranslations(const wstring& sTrans);

   bool UpdateData(bool bUiToKernel);

   void InitListViewColumns(HWND hWndListView);

   LocaInSituData _LocaInSituData;
};
#endif// _LOCA_IN_SITU_H
