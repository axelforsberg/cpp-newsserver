/* servercoder.h */

#ifndef SERVERCODER_H
#define SERVERCODER_H

#include<vector>
#include <string>

namespace client_server{

class ServerCoder {
	public:
		/* Encodes the servers response to the client on the low level protocol */
		std::vector<unsigned char> serverEncode(const std::vector<std::string>& msg);
		
		/* Decodes the clients request to the server from the low level protocol */
		std::vector<std::string> serverDecode(const std::vector<unsigned char>& msg);
		
	private:
		/* Help fuction to translate between a unsigned int and 4 unsigned chars. */
		std::vector<unsigned char> intToChar(const unsigned int& i);
		
		/* Help fuction to translate between 4 unsigned chars and a unsigned int. */
		unsigned int bytesToInt(const unsigned char& c1, const unsigned char& c2, const unsigned char& c3, const unsigned char& c4);
		
	};
}
#endif
