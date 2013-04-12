/* article.cc */

/* Self explanatory class that acts as a data container for an article */

#include "article.h"

#include <string>

using namespace std;

namespace client_server{

	Article::Article(const std::string& title, const std::string& author, const std::string& content, const size_t& u_art_nbr) {
		this->title = title;
		this->author = author;
		this->content = content;
		this->u_art_nbr = u_art_nbr;
	}

	string& Article::getAuthor(){
		return author;
	}
	
	string& Article::getTitle(){
		return title;
	}
	
	string& Article::getContent(){
		return content;
	}
	
	size_t Article::getArtNbr(){
		return u_art_nbr;
	}
}
