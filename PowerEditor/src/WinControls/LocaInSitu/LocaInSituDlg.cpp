#include "LocaInSituDlg.h"
#include "LocaInSituTools.h"
#include "UrlEncoder.h"
#include "DoRequest.h"
#include "..\..\..\Notepad_plus.h"


#include  <iostream>
using namespace std;

#include  <winhttp.h>

#define VERBOSE

BOOL CALLBACK LocaInSituDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG :
		{
			goToCenter();
			HWND hCombo = ::GetDlgItem(_hSelf, IDC_CBB_QUALITY);
			::SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
			int i = 1;
			for (i = 1; i < 10 ; i++)
			{
				char s[10];
				sprintf(s, "%d", i);
				::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)s);
			}
			::SendMessage(hCombo, CB_SETCURSEL, (WPARAM) 4, 0);
			InitListViewColumns(::GetDlgItem(_hSelf, IDC_LIST_MODULE));
			return TRUE;
		}
		case WM_COMMAND : 
		{
			if (LOWORD(wParam)==IDC_LIST_SECTRA)
			{
				if (HIWORD(wParam) == LBN_DBLCLK)
				{
					return UpdateCurrentTransFromSectraList();
				}
				break;
			}
			switch (wParam)
			{
				case IDCANCEL : // Close
					display(false);
					return TRUE;

				case IDOK :
				{
					UpdateData(true);
					ApplyTranslation();
					display(false);
					return true;
					break;
				}

				case IDC_BTN_SUBMIT :
				{
					SYSTEMTIME Time;
					GetSystemTime(&Time);
					_LocaInSituData.SetTime(Time);
					UpdateData(true);
					//return DoRequestSubmit();
					return DoRequestSubmitSQL();
					break;
				}

				//case IDC_BTN_RETRIEVE_CURRENT :
				//{
				//	bool bResult = DoRequestRetrieveCurrent();
				//	UpdateData(false);
				//	return bResult;
				//	break;

				//}
				case IDC_BTN_RETRIEVE_ALL :
				{
					//bool bResult = DoRequestRetrieveAll();
					bool bResult = DoRequestRetrieveAllSQL();
					UpdateData(false);
					return bResult;
					break;
				}
				case IDC_BTN_ONLINE :
				{
					return DoRequestGoToSectra();
					break;
				}
				default :
				{
					return StaticDialog::run_dlgProc(message, wParam, lParam);
				}
			}
		}

		default :
			return StaticDialog::run_dlgProc(message, wParam, lParam);
	}
	return false;
}
void LocaInSituDlg::display(bool toShow) 
{
	UpdateData(false);

	Window::display(toShow);
    if (toShow)
        ::SetFocus(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED));
}

void LocaInSituDlg::SetLocaParam(LocaInSituData LocaInSituData)
{
	_LocaInSituData = LocaInSituData;
	_LocaInSituData.RetrieveXliffTranslations();
	_LocaInSituData.UpdateSourceTrans();
}

void LocaInSituDlg::ApplyTranslation()
{
   // Change the file in xml map
	Notepad_plus::getInstance()->ChangeString(_LocaInSituData.GetResType(), _LocaInSituData.GetWindowTagName(), _LocaInSituData.GetID(), _LocaInSituData.GetNewTrans());

   //Reload the User Interface
	Notepad_plus::getInstance()->ReloadStringInUI(_LocaInSituData.GetResType(), _LocaInSituData.GetHWnd(), _LocaInSituData.GetWindowTagName());
}

bool LocaInSituDlg::DoRequest(LPCWSTR sURLName,
							  LPCWSTR sRequestVerb,
							  LPCWSTR sRequestObjName,
							  LPCWSTR sRequestVersion,
							  LPCWSTR sRequestRefer,
							  vector<wstring>& sRequestAnswer)
{
			
	
					DWORD dwSize = 0;
					DWORD dwDownloaded = 0;
					//LPSTR pszOutBuffer;
					bool  bResults = FALSE;
					HINTERNET hSession = NULL, 
							  hConnect = NULL,
							  hRequest = NULL;

					// Use WinHttpOpen to obtain a session handle.
					hSession = WinHttpOpen( L"Request/1.0",  
										  WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
										  WINHTTP_NO_PROXY_NAME, 
										  WINHTTP_NO_PROXY_BYPASS, 0 );

											// Specify an HTTP server.
											if( !hSession )
												return false;

											hConnect = WinHttpConnect( hSession, sURLName,
																	   80, 0 );

											if( !hConnect )
											{
												UINT uError = GetLastError();
												TCHAR  str[128];
												::wsprintf(str, "WinHttpConnect : GetLastError() returned %lu", uError);
										#ifdef VERBOSE
												::MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
										#endif

												WinHttpCloseHandle( hSession );
												return false;
											}

		//hRequest = WinHttpOpenRequest( hConnect, NULL, 
	 //                                  NULL, 
	 //                                  NULL, WINHTTP_NO_REFERER, 
	 //                                  WINHTTP_DEFAULT_ACCEPT_TYPES,
	 //                                  0);

			// Create an HTTP request handle.
		hRequest = WinHttpOpenRequest( hConnect, sRequestVerb, sRequestObjName,
									   sRequestVersion, WINHTTP_NO_REFERER, 
									   WINHTTP_DEFAULT_ACCEPT_TYPES, 
									   NULL/*WINHTTP_FLAG_SECURE*/ );


		//pas de login sur le serveur nii

		//bResults = WinHttpSetCredentials(hRequest, WINHTTP_AUTH_TARGET_SERVER, WINHTTP_AUTH_SCHEME_BASIC, L"sectra", L"UnescoL", NULL);
	 


		//if (!bResults)
		//{
		//	UINT uError = GetLastError();
		//	WinHttpCloseHandle( hConnect );
		//	WinHttpCloseHandle( hSession );
		//	return false;
		//}*/

		
		
		//// Create an HTTP request handle.
		//hRequest = WinHttpOpenRequest( hConnect, sRequestVerb, sRequestObjName,
		//							   sRequestVersion, sRequestRefer, 
		//							   WINHTTP_DEFAULT_ACCEPT_TYPES, 
		//							   WINHTTP_FLAG_SECURE );

		
		if( !hRequest )
		{
			UINT uError = GetLastError();
			TCHAR  str[128];
			::wsprintf(str, "WinHttpOpenRequest : GetLastError() returned %lu", uError);
	#ifdef VERBOSE
			::MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
	#endif
			 
			WinHttpCloseHandle( hConnect );
			WinHttpCloseHandle( hSession );
			return false;
		}

	// Send a request.
	bResults = (bool) WinHttpSendRequest( hRequest,
								   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
								   WINHTTP_NO_REQUEST_DATA, 0, 
								   0, 0 );


	// End the request.
	if( bResults )
	{
		bResults = (bool) WinHttpReceiveResponse( hRequest, NULL );
	}
	else
	{
		UINT uError = GetLastError();
		TCHAR  str[128];
		::wsprintf(str, "WinHttpSendRequest : GetLastError() returned %lu", uError);
#ifdef VERBOSE
		::MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
#endif
	}

	// Keep checking for data until there is nothing left.
	if( bResults )
	{
		do 
		{
		  // Check for available data.
		  dwSize = 0;
		  if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) )
		  {
			UINT uError = GetLastError();
			TCHAR  str[128];
			::wsprintf(str, "WinHttpQueryDataAvailable : GetLastError() returned %lu", uError);
#ifdef VERBOSE
			::MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
#endif
		  }

		  // Allocate space for the buffer.
		  LPSTR pszOutBuffer = new char[dwSize+1];
		  if( !pszOutBuffer )
		  {
			printf( "Out of memory\n" );
			dwSize=0;
			bResults = false;
		  }
		  else
		  {
			// Read the data.
			ZeroMemory( pszOutBuffer, dwSize+1 );

			if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
								  dwSize, &dwDownloaded ) )
			{
				bResults = false;

				UINT uError = GetLastError();
				TCHAR  str[128];
				::wsprintf(str, "WinHttpReadData : GetLastError() returned %lu", uError);
#ifdef VERBOSE
				::MessageBox(NULL, str, "Error", MB_OK | MB_ICONERROR);
#endif
			}
			else
			{
				if (dwSize > 0)
				{
					wstring sUnicodeString = LocaInSituTools::s2ws(pszOutBuffer, true);
					sRequestAnswer.push_back(sUnicodeString);

					////TEMP
					//pszOutBuffer[dwDownloaded] = '\0';  
					//wchar_t * wTmp = NULL;  
					//DWORD dwTmpSize = mbstowcs(wTmp, pszOutBuffer, MB_CUR_MAX);  
					//wTmp = new wchar_t[dwTmpSize];  
					//mbstowcs(wTmp, pszOutBuffer, dwTmpSize);  
					////pRetval->_strBody.append(lpszData);  
					//delete[] wTmp;  

					//std::string sStr = pszOutBuffer;
					//const char* bytes = sStr.data();
					//wstring UnicodeStr = new wstring( bytes , "Cp1252" );

					//LPWSTR sUnicode = new wchar_t[dwSize+1];
					//for (int i = 0; i < dwSize; i++)
					//{
					//	sUnicode[i] = pszOutBuffer[i] & 0xff;
					//}
					//TEMP
				}
			}

			// Free the memory allocated to the buffer.
			delete [] pszOutBuffer;
		  }
		} 
		while( dwSize > 0 );
	}

	// Close any open handles.
	WinHttpCloseHandle( hRequest );
	WinHttpCloseHandle( hConnect );
	WinHttpCloseHandle( hSession );
	
	return bResults;
}

bool LocaInSituDlg::DoRequestSubmit()
{
	//serveur Japan
	//LPCWSTR sURLName        = L"sectra-w.ex.nii.ac.jp";
	//serveur Grenoble
	LPCWSTR sURLName        = L"eolss.imag.fr";
	LPCWSTR  sRequestVerb    = L"GET";

	//LPCWSTR  sRequestObjName = L"xwiki/bin/view/Corpus/Translation?xpage=plain&content=I%20love%20you&sl=en&tl=fr&MT=iMAG";
	//wprintf(sRequestObjName, L"I%20love%20you");

	//Requete UPDATE sur le serveur du Japan
	//wstring sRequestObjName = L"xwiki/bin/view/TM/UpdateTM?projName=DSR_CAPTION&source=en&tran=fr&sourcetext=";
	
	//Requete UPDATE sur le serveur de Grenoble sur le corpus NOTEPAD
	wstring sRequestObjName = L"xwiki/bin/view/TM/UpdateTM?projName=NOTEPAD&source=en&tran=fr&sourcetext=";

	string sSourceName = _LocaInSituData.GetWindowTagName() + "@" + _LocaInSituData.GetSourceTrans();
	//CURLEncode UrlEncoder(sSourceName);
	//sSourceName = UrlEncoder.GetConvertedString();

	string sTargetName = _LocaInSituData.GetNewTrans();
	//CURLEncode UrlEncoder2(sTargetName);
	//sTargetName = UrlEncoder2.GetConvertedString();
	
	sRequestObjName += LocaInSituTools::s2ws(sSourceName);
	sRequestObjName += L"&targettext=";
	sRequestObjName += LocaInSituTools::s2ws(sTargetName);

	LPCWSTR  sRequestVersion = L"HTTP/1.1";
	LPCWSTR  sRequestRefer = WINHTTP_NO_REFERER;
	vector<wstring> sRequestAnswer;

	if (DoRequest(sURLName, sRequestVerb, sRequestObjName.c_str(), sRequestVersion, sRequestRefer, sRequestAnswer) == FALSE)
		return false;

	for(int i= 0 ; i < sRequestAnswer.size(); i++)
	{
		sRequestAnswer[i];
	}

	DoRequestRetrieveAll();

	UpdateData(false);
	return true;
}

bool LocaInSituDlg::DoRequestRetrieveAll()
{
	////serveur du Japan
	//LPCWSTR sURLName        = L"sectra-w.ex.nii.ac.jp";
	//serveur de grenoble
	LPCWSTR sURLName        = L"eolss.imag.fr";
	LPCWSTR  sRequestVerb    = L"GET";

	//LPCWSTR  sRequestObjName = L"xwiki/bin/view/Corpus/Translation?xpage=plain&content=I%20love%20you&sl=en&tl=fr&MT=iMAG";
	//wprintf(sRequestObjName, L"I%20love%20you");

	// Requete de recherche Exact sur le corpus DSR_CAPTION
	//wstring  sRequestObjName = L"xwiki/bin/view/TM/ExactSearch?xpage=plain&projName=DSR_CAPTION&source=en&tran=fr&sourcetext=";
	
	// Requete de recherche Exact sur le corpus NOTEPAD
	wstring  sRequestObjName = L"xwiki/bin/view/TM/ExactSearch?xpage=plain&projName=NOTEPAD&source=en&tran=fr&sourcetext=";


	string sSourceName = _LocaInSituData.GetWindowTagName() + "@" + _LocaInSituData.GetSourceTrans();
	//CURLEncode UrlEncoder(sSourceName);
	//sSourceName = UrlEncoder.GetConvertedString();

	sRequestObjName += LocaInSituTools::s2ws(sSourceName);

	LPCWSTR  sRequestVersion = L"HTTP/1.1";
	LPCWSTR  sRequestRefer = WINHTTP_NO_REFERER;
	vector<wstring> sRequestAnswer;

	if (DoRequest(sURLName, sRequestVerb, sRequestObjName.c_str(), sRequestVersion, sRequestRefer, sRequestAnswer) == FALSE)
		return false;

	_LocaInSituData.GetSectraTranslations().clear();
	for(int i= 0 ; i < sRequestAnswer.size(); i++)
	{
		ExtractTranslations(sRequestAnswer[i]);
	}

	UpdateData(false);
	return true;
}

bool LocaInSituDlg::DoRequestGoToSectra()
{
	//LPCSTR sURLName = "http://eolss.imag.fr/xwiki/bin/view/Corpus/PosteditTM?projName=NOTEPAD&source=en&tran=fr";
	LPCSTR sURLName = "http://eolss.imag.fr/xwiki/bin/view/Corpus/PosteditTM?currentProj=NOTEPAD&source=en&tran=fr";

	::ShellExecute(_hSelf, "open", sURLName, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool LocaInSituDlg::UpdateData(bool bUiToKernel)
{
	if (bUiToKernel == true)
	{
		char sNewString[512];

		// IDC_EDIT_SR_TRANSLATED
		::GetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED), sNewString, 512);
		_LocaInSituData.SetNewTrans(string(sNewString));

		//IDC_CBB_QUALITY
		::GetWindowText(::GetDlgItem(_hSelf, IDC_CBB_QUALITY), sNewString, 512);
		_LocaInSituData.SetQuality(atoi(sNewString));

		//IDC_CBB_QUALITY
		::GetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_COMMENT), sNewString, 512);
		_LocaInSituData.SetComment(sNewString);
	}
	else
	{
		// IDC_EDIT_STR_TO_TRANSLATE
		::SetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_STR_TO_TRANSLATE), _LocaInSituData.GetCurrentTrans().c_str());
		// IDC_EDIT_SR_TRANSLATED
		HWND hCombo = ::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED);
		if (hCombo)
		{
		  ::SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
		  vector<string> ListString;
		  ListString = _LocaInSituData.GetXLiffTranslations();

		  for(int i=ListString.size()-1; i >=0 ; --i)
		  {
			 char sTrans[512];
			 strcpy(sTrans, ListString[i].c_str());
			 ::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)sTrans);
		  }
		  ::SendMessage(hCombo, CB_SETCURSEL, (WPARAM) 0, 0);
		}
		::SetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED), _LocaInSituData.GetNewTrans().c_str());

		// IDC_LIST_SECTRA
		//HWND hList = ::GetDlgItem(_hSelf, IDC_LIST_SECTRA);
		//if (hList)
		//{
		//  ::SendMessage(hList, LB_RESETCONTENT, 0, 0);
		//  vector<SectraUnit> ListSectraUnit;
		//  ListSectraUnit = _LocaInSituData.GetSectraTranslations();

		//  for(int i=ListSectraUnit.size()-1; i >=0 ; --i)
		//  {
		//	 char sTrans[512];
		//	 strcpy(sTrans, LocaInSituTools::ws2s(ListSectraUnit[i].sTrans).c_str());
		//	 ::SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sTrans);
		//  }
		//  ::SendMessage(hList, LB_SETCURSEL, (WPARAM) 0, 0);
		//}
		// IDC_LIST_MODULE
		::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE), LVM_DELETEALLITEMS, 0, 0);
		//Initialize LVITEM members that are common to all items.
		LVITEM lvI;
		lvI.mask = LVIF_TEXT; 
		lvI.cchTextMax = 256; // Max size of test
		lvI.state = 0; 
		lvI.stateMask = 0; 

		// Initialize LVITEM members that are different for each item. 
		vector<SectraUnit> ListSectraUnit;
		ListSectraUnit = _LocaInSituData.GetSectraTranslations();
		int iSize = ListSectraUnit.size();
		for (int index = 0; index < iSize; index++)
		{
			lvI.iItem = index;
			lvI.iSubItem = 0;
			char buffer[512];
			strcpy(buffer, LocaInSituTools::ws2s(ListSectraUnit[index].sTrans).c_str()); 
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_INSERTITEM,0,(LPARAM)&lvI);

			lvI.iItem = index;
			lvI.iSubItem = 1;
			sprintf(buffer, "%d", ListSectraUnit[index].iQualityTrans);
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 2;
			strcpy(buffer, LocaInSituTools::ws2s(ListSectraUnit[index].sComment).c_str()); 
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 3;
			strcpy(buffer, LocaInSituTools::ws2s(ListSectraUnit[index].sDate).c_str()); 
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 4;
			strcpy(buffer, LocaInSituTools::ws2s(ListSectraUnit[index].sFirstName).c_str()); 
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 5;
			strcpy(buffer, LocaInSituTools::ws2s(ListSectraUnit[index].sSecondName).c_str()); 
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 6;
			sprintf(buffer, "%d", ListSectraUnit[index].iUserQuality);
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);

			lvI.iSubItem = 7;
			sprintf(buffer, "%d", ListSectraUnit[index].iUserQualityCalculated);
			lvI.pszText = buffer;
			::SendMessage(::GetDlgItem(_hSelf, IDC_LIST_MODULE),LVM_SETITEM,0,(LPARAM)&lvI);
		}
		
	}

	return true;
}

void LocaInSituDlg::ExtractTranslations(const wstring& sTrans)
{
	wstring delimiters = L"<br>";
	vector<wstring> tokens;
    	
	// skip delimiters at beginning.
	wstring::size_type lastPos = sTrans.find(delimiters, 0);
	if (lastPos == 0)
		lastPos = delimiters.size();
	else
		lastPos = 0;

	// find first "non-delimiter".
	wstring::size_type pos = sTrans.find(delimiters, lastPos);
	if (pos == wstring::npos)
		pos = sTrans.size();

	while (wstring::npos != pos || wstring::npos != lastPos)
	{
		// found a token
		wstring sExtracted = sTrans.substr(lastPos, pos - lastPos);
		LocaInSituTools::TrimLeft(sExtracted, L" ");
		LocaInSituTools::TrimLeft(sExtracted, new wchar_t(13));
		LocaInSituTools::TrimLeft(sExtracted, new wchar_t(10));
		LocaInSituTools::TrimLeft(sExtracted, L" ");

		LocaInSituTools::TrimRight(sExtracted, L" ");
		LocaInSituTools::TrimRight(sExtracted, new wchar_t(13));
		LocaInSituTools::TrimRight(sExtracted, new wchar_t(10));
		LocaInSituTools::TrimRight(sExtracted, L" ");


		if (!sExtracted.empty())
		{
			SectraUnit SectUnit;
			SectUnit.sTrans = sExtracted;
			_LocaInSituData.GetSectraTranslations().push_back(SectUnit);
		}

		// skip delimiters.  Note the "not_of"
		lastPos = sTrans.find(delimiters, pos);
		if (lastPos != wstring::npos)
			lastPos += delimiters.size();

		if (lastPos >= sTrans.size())
			lastPos = wstring::npos;

		// find next "non-delimiter"
		pos = sTrans.find(delimiters, lastPos);
	}

}

bool LocaInSituDlg::UpdateCurrentTransFromSectraList()
{
	int index = ::SendDlgItemMessage(_hSelf, IDC_LIST_SECTRA, LB_GETCURSEL, 0, 0);
	if (index != LB_ERR)
	{
		LPSTR sText;
		::SendDlgItemMessage(_hSelf, IDC_LIST_SECTRA, LB_GETTEXT, index, (LPARAM)sText);
		//Search if the string is already in the data
		bool bFound = false;
		int iIndex = 0;
		for (int iIndex = 0; iIndex < _LocaInSituData.GetXLiffTranslations().size(); iIndex++)
		{
			if (strcmp(_LocaInSituData.GetXLiffTranslations()[iIndex].c_str(), sText) == 0)
			{
				bFound = true;
				break;
			}
		}
		if (bFound == true)
		{
			iIndex = _LocaInSituData.GetXLiffTranslations().size() - iIndex - 1;
			::SendMessage(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED), CB_SETCURSEL, (WPARAM) iIndex, 0);
			return true;
		}
		else
		{
			::SetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_SR_TRANSLATED), sText);
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool LocaInSituDlg::DoRequestSubmitSQL()
{
	string sQuery;
	string sIdString, sIdUser, sIdProject, sQuality, sDate, sTransType, sTransAncestor;

	char buffer1[10], buffer2[10], buffer3[10] ,buffer4[10], buffer5[10], buffer6[10] ,buffer7[10];
	sprintf(buffer1, "%d", _LocaInSituData.GetID());
	sIdString = _LocaInSituData.GetWindowTagName() + string("@") + buffer1;
	sprintf(buffer2, "%d", _LocaInSituData.GetIdUser());
	sIdUser = buffer2;
	sprintf(buffer3, "%d", _LocaInSituData.GetIdProject());
	sIdProject = buffer3;
	sprintf(buffer4, "%d", _LocaInSituData.GetQuality());
	sQuality = buffer4;
	sprintf(buffer5, "%d-%d-%d %d:%d:%d", _LocaInSituData.GetTime().wYear, _LocaInSituData.GetTime().wMonth, _LocaInSituData.GetTime().wDay, _LocaInSituData.GetTime().wHour, _LocaInSituData.GetTime().wMinute, _LocaInSituData.GetTime().wSecond);
	sDate = buffer5;
	sprintf(buffer6, "%d", 1);
	sTransType = buffer6;
	sprintf(buffer7, "%d", 0);
	sTransAncestor = buffer7;

	sQuery = string("INSERT INTO TRANSLATIONS (Id_String, Id_User, Id_Project, Translation, Quality, Comment, Date, Id_TransActionType, Id_TransActionAncestor) VALUES ") + string("( '") + sIdString + string("', '") + sIdUser + string("', '") + sIdProject + string("', '") + _LocaInSituData.GetNewTrans() + string("', '") + sQuality + string("', '") + _LocaInSituData.GetComment() + string("', '") + sDate + string("', '") + sTransType + string("', '") + sTransAncestor + string("');");

	CDoRequest* pDoReq = new CDoRequest("127.0.0.1", "root", "", "notepad");
	return pDoReq->DoQueryInsert(sQuery);
}

bool LocaInSituDlg::DoRequestRetrieveAllSQL()
{
	CDoRequest* pDoReq = new CDoRequest("127.0.0.1", "root", "", "notepad");
	mysqlpp::StoreQueryResult ResultSet;

	char buffer1[10];
	sprintf(buffer1, "%d", _LocaInSituData.GetID());
	string sIdString = _LocaInSituData.GetWindowTagName() + "@" + buffer1;
	string sIdProject;
	char buffer2[10];
	sprintf(buffer2, "%d", _LocaInSituData.GetIdProject());
	string sQuery = string("SELECT * FROM TRANSLATIONS, USER, PROJECT, USERSKILL WHERE TRANSLATIONS.Id_String = '") + sIdString + string("' AND TRANSLATIONS.Id_Project = '") + buffer2 + string("'  AND PROJECT.Id_Project = '") +  buffer2 + string("'  AND TRANSLATIONS.Id_User = USER.Id_User AND USERSKILL.tech = PROJECT.Tech_dom AND USERSKILL.language = PROJECT.Target_language AND USERSKILL.user = TRANSLATIONS.Id_User;"); 

	if (!pDoReq->DoQueryGet(ResultSet, sQuery))
		return false;

	_LocaInSituData.GetSectraTranslations().clear();

	for (size_t i = 0; i < ResultSet.num_rows(); ++i) 
	{
		SectraUnit SectUnit;
		SectUnit.sTrans = LocaInSituTools::s2ws(ResultSet[i]["Translation"].c_str());
		SectUnit.iQualityTrans = ResultSet[i]["Quality"];
		SectUnit.sComment = LocaInSituTools::s2ws(ResultSet[i]["Comment"].c_str());
		SectUnit.sDate = LocaInSituTools::s2ws(ResultSet[i]["Date"].c_str());
		SectUnit.sFirstName = LocaInSituTools::s2ws(ResultSet[i]["FirstName"].c_str());
		SectUnit.sSecondName = LocaInSituTools::s2ws(ResultSet[i]["SecondName"].c_str());
		SectUnit.iUserQuality = ResultSet[i]["level"];
		SectUnit.iUserQualityCalculated = ResultSet[i]["calc_level"];
		_LocaInSituData.GetSectraTranslations().push_back(SectUnit);
	}

	return true;
}

void LocaInSituDlg::InitListViewColumns(HWND hWndListView) 
{ 
	string sColNames[8];
	sColNames[0] = "Translation";
	sColNames[1] = "Quality";
	sColNames[2] = "Comment";
	sColNames[3] = "Date";
	sColNames[4] = "First Name";
	sColNames[5] = "Second Name";
	sColNames[6] = "User Quality";
	sColNames[7] = "User Calc Quality";

	int iColSize[8];
	iColSize[0] = 200;
	iColSize[1] = 80;
	iColSize[2] = 80;
	iColSize[3] = 80;
	iColSize[4] = 80;
	iColSize[5] = 80;
	iColSize[6] = 80;
	iColSize[7] = 80;

    char szText[256];     // temporary buffer 
    LVCOLUMN lvc; 
    int iCol; 

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text, and subitem members
    // of the structure are valid. 
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 
	  
    // Add the columns
    for (iCol = 0; iCol < 8; iCol++) 
    { 
        lvc.iSubItem = iCol;
        lvc.pszText = (LPSTR)sColNames[iCol].c_str();	
        lvc.cx = iColSize[iCol];     // width of column in pixels

        lvc.fmt = LVCFMT_RIGHT; // right-aligned column		                         

		::SendMessage(hWndListView, LVM_INSERTCOLUMN,iCol,(LPARAM)&lvc);
    } 
} 