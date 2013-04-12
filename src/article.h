/* article.h */

/* Self explanatory class that acts as a data container for an article */

#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

namespace client_server{

class Article{
	
	public:
		Article(const std::string& title, const std::string& author, const std::string& content, const size_t& u_art_nbr);
		std::string& getAuthor();
		std::string& getTitle();
		std::string& getContent();
		size_t getArtNbr();
	private:
		std::string author;
		std::string title;
		std::string content;
		size_t u_art_nbr; 		
};
}
		
#endif
