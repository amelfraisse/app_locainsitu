#include "DoRequest.h"
 
CDoRequest::CDoRequest(const std::string& sUrl, 
					 const std::string& sUser, 
					 const std::string& sPass, 
					 const std::string& sDataBase)
{
	this->m_sUrl		= sUrl;
	this->m_sUser		= sUser;
	this->m_sPassword	= sPass;
	this->m_sDataBase	= sDataBase;
}

sql::ResultSet* CDoRequest::DoQueryGet(const std::string& sQuery)
{
	try
	{
		sql::Driver* pDriver = get_driver_instance();
		if (pDriver == NULL)
			return NULL;
		sql::Connection* pConnection = pDriver->connect(this->m_sUrl, this->m_sUser, this->m_sPassword);
		if (pConnection == NULL)
			return NULL;

		pConnection->setSchema(this->m_sDataBase);
		sql::Statement* pStmt = pConnection->createStatement();
		if (pStmt == NULL)
		{
			delete pConnection;
			return NULL;
		}

		//stmt->execute("CALL get_pop(\"Uganda\", @pop)");
		sql::ResultSet* Res = pStmt->executeQuery(sQuery);

		delete pStmt;
		delete pConnection;

		return Res;
	}
	catch (sql::SQLException &e) 
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		std::string sMessage;
		sMessage = std::string(e.what()) + " - " /*+ std::string(e.getErrorCode())*/ + " - " + std::string(e.getSQLState());
	}
}