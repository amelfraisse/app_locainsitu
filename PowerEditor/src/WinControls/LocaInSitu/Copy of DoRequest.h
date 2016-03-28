#ifndef _DO_REQUEST_H
#define _DO_REQUEST_H

#include <string>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class CDoRequest
{
public :
	CDoRequest(	const std::string& sUrl, 
				const std::string& sUser, 
				const std::string& sPass, 
				const std::string& sDataBase);

	sql::ResultSet* DoQueryGet(const std::string& sQuery);

protected :
	std::string m_sUrl;
	std::string m_sUser;
	std::string m_sPassword;
	std::string m_sDataBase;
};

#endif// _DO_REQUEST_H