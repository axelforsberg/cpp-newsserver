/* cmdtranslate.h */

#ifndef CMDTRANSLATE_H
#define CMDTRANSLATE_H

#include<vector>
#include <string>
#include "databaseinterface.h"
#include "servercoder.h"

namespace client_server{

	class CmdTranslate{
	public:
		/* Constructor, takes a ram/hddstorage as argument */
		CmdTranslate(DatabaseInterface& ds): db(ds) {};
		
		/* Command translate function */
		std::vector<unsigned char> execute(const std::vector<unsigned char>& encodedMsg);
		
	private: 
		DatabaseInterface& db;
		ServerCoder sc;
	};
}

#endif
