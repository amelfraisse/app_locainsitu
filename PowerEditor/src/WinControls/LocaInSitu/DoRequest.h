#ifndef _DO_REQUEST_H
#define _DO_REQUEST_H

#include <string>

#include <mysql++.h>
#include <ssqls.h>

using namespace std;
class CDoRequest
{
public :
	CDoRequest(	const std::string& sUrl, 
				const std::string& sUser, 
				const std::string& sPass, 
				const std::string& sDataBase);

	bool DoQueryGet(mysqlpp::StoreQueryResult& ResultSet, const std::string& sQuery);
	bool DoQueryInsert(const std::string& sQuery);

protected :
	std::string m_sUrl;
	std::string m_sUser;
	std::string m_sPassword;
	std::string m_sDataBase;
};

#endif// _DO_REQUEST_H