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

bool CDoRequest::DoQueryGet(mysqlpp::StoreQueryResult& ResultSet, const std::string& sQuery)
{
	//try
	{
		mysqlpp::Connection conn(false);

		if (!conn.connect(this->m_sDataBase.c_str(), this->m_sUrl.c_str(), this->m_sUser.c_str(), this->m_sPassword.c_str())) 
			return false;

		mysqlpp::Query query = conn.query(sQuery.c_str());
		ResultSet = query.store();

		return true;
	}
 //   catch (const mysqlpp::BadQuery& er)
	//{
 //       // Handle any query errors
 //       cerr << "Query error: " << er.what() << endl;
 //       return false;
 //   }
 //   catch (const mysqlpp::BadConversion& er)
	//{  
 //       // Handle bad conversions
 //       cerr << "Conversion error: " << er.what() << endl <<
 //               "\tretrieved data size: " << er.retrieved <<
 //               ", actual size: " << er.actual_size << endl;
 //       return false;
 //   }
    //catch (const mysqlpp::Exception& er) {
    //    // Catch-all for any other MySQL++ exceptions
    //    cerr << "Error: " << er.what() << endl;
    //    return false;
    //}
}


bool CDoRequest::DoQueryInsert(const std::string& sQuery)
{
	mysqlpp::Connection conn(false);

	if (!conn.connect(this->m_sDataBase.c_str(), this->m_sUrl.c_str(), this->m_sUser.c_str(), this->m_sPassword.c_str())) 
		return false;

	mysqlpp::Query query = conn.query(sQuery.c_str());
	mysqlpp::SimpleResult Res;
	Res = query.execute();

	return true;
}
