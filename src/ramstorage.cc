/* ramstorage.cc */

#include "ramstorage.h"
#include "newsgroup.h"

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace client_server {


	/*
	 * List articles in a specified newsgroup
	 * Returns a vector<string> with all numbers and names on the articles
	 */
	vector<string> RAMStorage::listArticles(const size_t& nbr){
		vector<string> res;
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){
			return n.getNbr() == nbr;
		});
		
		if (it != ng.end()){
			res = it->listArticles();
			if(res.size() != 0){
				return res;
			}
			res.push_back("ERR_NO_ARTS");
		}

		res.push_back("ERR_NG_DOES_NOT_EXIST");
		return res;
	}

	/*
	 * List all newsgroup
	 * Returns a vector<string> with all numbers and names och the newsgroup
	 */
	vector<string> RAMStorage::listNewsGroups(){

		vector<string> res;

		for(NewsGroup n: ng) {
			stringstream ss;
			ss << n.getNbr();
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());

			res.push_back(n.getName());
		}

		return res;
	}

	/*
	 * Create a newsgroup
	 * Returns true if no other newsgroup exists with the same name
	 */
	bool RAMStorage::createNewsGroup(const std::string& name){
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){
			return n.getName() == name;
		});
		
		if (it != ng.end()){
			return false;
		}
	
		ng.push_back(NewsGroup(name,++counter));
		return true;
	}


	/*
	 * Delete a newsgroup
	 * Returns true if a newsgroup existed with the number
	 */
	bool RAMStorage::deleteNewsGroup(const size_t& nbr){
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){
			return n.getNbr() == nbr;
		});

		if (it != ng.end()){
			ng.erase(it);
			return true;
		}

		return false;
	}

	/*
	 * Create an article in a specified newsgroup
	 * Returns true if a newsgroup existed with the number.
	 */	
	bool RAMStorage::createArticle(const size_t& nbr, const string& title, const string& author, const string& content){
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){ 
			return n.getNbr() == nbr;
		});

		if(it != ng.end()){
			it->createArticle(title, author, content);
			return true;	
		}
		return false;
	}


	/*
	 * Delete an article in a specified newsgroup
	 * Returns a string with a success / failue message
	 */
	string RAMStorage::deleteArticle(const size_t& n_nbr, const size_t& a_nbr){
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){ 
			return n.getNbr() == n_nbr;
		});
	
		if(it != ng.end()){ 
			bool b = it->deleteArticle(a_nbr);
			return b ? "ANS_ACK" : "ERR_ART_DOES_NOT_EXIST";	
		}
		return "ERR_NG_DOES_NOT_EXIST";
	}

	/*
	 * Retrive a specified article in a speficied newsgroup
	 * Returns a vector<string> contating title, author and the content of the article
	 */
	vector<string> RAMStorage::getArticle(const size_t& n_nbr, const size_t& a_nbr){
		vector<string> result;
		
		auto it = find_if(ng.begin(),ng.end(),[&](NewsGroup& n){ 
			return n.getNbr() == n_nbr;
		});

		if(it != ng.end()){
			result = it->getArticle(a_nbr);
		} else {
			result.push_back("ERR_NG_DOES_NOT_EXIST");
		}
		return result;
	}
}






