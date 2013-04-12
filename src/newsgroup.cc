/* newsgroup.h */

#include "newsgroup.h"
#include "article.h"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

namespace client_server {

	/*
	 * Constructor for a newsgroup
	 */
	NewsGroup::NewsGroup(const string& u_name, const size_t& u_news_nbr){
		this->u_name = u_name;
		this->u_news_nbr = u_news_nbr;
		counter = 0;
	}

	/*
	 * Create an article
	 */
	void NewsGroup::createArticle(const string& title, const string& author, const string& content){
		articles.push_back(Article(title, author, content, ++counter));
	}
	
	/*
	 * Delete an article
	 * Returns true if an article existed with the specified number
	 */
	bool NewsGroup::deleteArticle(const size_t u_art_nbr){
		auto it = find_if(articles.begin(),articles.end(),[&](Article& a){
			return a.getArtNbr() == u_art_nbr;
		});
		
		if(it != articles.end()){
			articles.erase(it);
			return true;
		}
		return false;
	}
	
	/*
	 * List articles
	 * Returns a vector<string> with number and names on the articles
	 */
	vector<string> NewsGroup::listArticles() {
		vector<string> result;
		for(Article a: articles){
			stringstream ss;
			ss << a.getArtNbr();
			result.push_back(ss.str());
			ss.clear();
			ss.str(string());

			result.push_back(a.getTitle());
		}	
		return result;
	}
	
	/*
	 * Retrive an article
	 * Returns a vector<string> with the content of the speficied article
	 */
	vector<string> NewsGroup::getArticle(const size_t u_art_nbr){	
		vector<string> result;
		auto it = find_if(articles.begin(),articles.end(),[&](Article& a){
			return a.getArtNbr() == u_art_nbr;
		});
		
		if(it != articles.end()){
			result.push_back(it->getTitle());
			result.push_back(it->getAuthor());
			result.push_back(it->getContent());
		} else {
			result.push_back("ERR_ART_DOES_NOT_EXIST");
		}
		return result;
	}		
}
	
	
	
