/* ramstorage.h */

#ifndef RAMSTORAGE_H
#define RAMSTORAGE_H

#include "databaseinterface.h"
#include "newsgroup.h"

#include <string>
#include <vector>

namespace client_server{

	class RAMStorage : public DatabaseInterface {

		public:
			RAMStorage(){
				counter = 0;
			};

			std::vector<std::string> listArticles(const size_t& nbr);

			std::vector<std::string> listNewsGroups();

			bool createNewsGroup(const std::string& name);
			
			bool deleteNewsGroup(const size_t& nbr);
			
			bool createArticle(const size_t& nbr, const std::string& title, const std::string& author, const std::string& content);
			
			std::string deleteArticle(const size_t& n_nbr, const size_t&  a_nbr);
			
			std::vector<std::string> getArticle(const size_t& n_nbr, const size_t&  a_nbr);
			
		private:
			std::vector<NewsGroup> ng;
			size_t counter;
	};
}
#endif
