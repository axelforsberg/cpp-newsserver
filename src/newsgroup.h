/* newsgroup.h */

#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include "article.h"

#include <vector>
#include <string>

namespace client_server{

class NewsGroup{
	
	public:
		NewsGroup(const std::string& u_name, const size_t& u_news_nbr);
		
		void createArticle(const std::string& title, const std::string& author, const std::string& content);
		bool deleteArticle(const size_t u_art_nbr);
		std::vector<std::string> listArticles();
		std::vector<std::string> getArticle(const size_t u_art_nbr);
		
		std::string& getName(){ return u_name; }
		size_t& getNbr(){ return u_news_nbr; } 
		
	private:
		std::string u_name;
		size_t u_news_nbr;
		std::vector<Article> articles;
		size_t counter; 		
};
}
#endif
		
