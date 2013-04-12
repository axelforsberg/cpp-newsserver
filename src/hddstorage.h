/* hddstorage.h */

#ifndef HDDSTORAGE_H
#define HDDSTORAGE_H

#include "databaseinterface.h"

#include <string>
#include <vector>
#include <iostream>

namespace client_server{

	class HDDStorage : public DatabaseInterface {

		public:
			HDDStorage();

			std::vector<std::string> listArticles(const size_t& nbr);

			std::vector<std::string> listNewsGroups();

			bool createNewsGroup(const std::string& name);
			
			bool deleteNewsGroup(const size_t& nbr);
			
			bool createArticle(const size_t& nbr, const std::string& title, const std::string& author, const std::string& content);
			
			std::string deleteArticle(const size_t& n_nbr, const size_t&  a_nbr);
			
			std::vector<std::string> getArticle(const size_t& n_nbr, const size_t&  a_nbr);
			
		private:
		
			void createFile(std::string ngName, const std::string& title, const std::string& author, const std::string& content);
			
			size_t getCount(std::string filepath);
			
			std::string delFile(std::string ngName, size_t nbr);
		
			void delAllFiles(std::string ngName);
			
			std::vector<std::string> listAllFiles(std::string ngName);
			
			std::vector<std::string> getFile(std::string ngName, const size_t& nbr);
			
			std::string ROOT_DIR = "./filesystem/";
			
			std::string NG_COUNTER_LOG = "./filesystem/counterlog";
			
			size_t counter = 0;
	};
}
#endif
