/* databaseinterface.h */

/*
 * The interface used by the diffrent storage solutions
 * Current implementations: 
 * RAMSTORAGE 	HDDSTORAGE
 */

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include<string>
#include<vector>

namespace client_server{

class DatabaseInterface{

public:	
	
	virtual std::vector<std::string> listArticles(const size_t& nbr) = 0;
	
	virtual std::vector<std::string> listNewsGroups() = 0;
	
	virtual bool createNewsGroup(const std::string& name)=0;
	
	virtual bool deleteNewsGroup(const size_t& nbr)=0;
	
	virtual bool createArticle(const size_t& nbr,const std::string& author, const std::string& title, const std::string& content)=0;
	
	virtual std::string deleteArticle(const size_t& n_nbr, const size_t& a_nbr)=0;
	
	virtual std::vector<std::string> getArticle(const size_t& n_nbr, const size_t& a_nbr)=0;
	
};
}
#endif
