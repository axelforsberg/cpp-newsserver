/* clientcoder.h */

#ifndef CLIENTCODER_H
#define CLIENTCODER_H

#include<vector>
#include <string>

namespace client_server{

	class ClientCoder {
	public:
		/* Encodes the clients request to the server on the low level protocol */
		std::vector<unsigned char> clientEncode(const std::vector<std::string>& msg);

		/* Decodes the servers resonse to the client on the low level protocol */
		std::vector<std::string> clientDecode(const std::vector<unsigned char>& msg);
		
	private:
		/* Help fuction to translate between a unsigned int and 4 unsigned chars. */
		std::vector<unsigned char> intToChar(const unsigned int& i);
		
		/* Help fuction to translate between 4 unsigned chars and a unsigned int. */
		unsigned int bytesToInt(const unsigned char& c1, const unsigned char& c2, const unsigned char& c3, const unsigned char& c4);	
	
	};
}
#endif
