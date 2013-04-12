/* clientcoder.cc */

#include "clientcoder.h"
#include "protocol.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace protocol;

namespace client_server{

	/*
	 * Help fuction to translate between a unsigned int and 4 unsigned chars.
	 */
	vector<unsigned char> ClientCoder::intToChar(const unsigned int& i){
		vector<unsigned char> bytes;
		
		bytes.push_back(i >> 24);
		bytes.push_back(i >> 16);
		bytes.push_back(i >>  8);
		bytes.push_back(i      );

		return bytes;
	}
	
	/*
	 * Help fuction to translate between 4 unsigned chars and a unsigned int.
	 */
	unsigned int ClientCoder::bytesToInt(const unsigned char& c1,const unsigned char& c2,const unsigned char& c3,const unsigned char& c4){
		unsigned int i = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
		return i;
	}
	
	/*
	 * Encodes the clients request to the server on the low level protocol
	 */
	vector<unsigned char> ClientCoder::clientEncode(const vector<string>& msg){
		
		string command= msg[0];
		vector<unsigned char> res;
		 
		if(command=="list_ng"){
			res.push_back(Protocol::COM_LIST_NG);
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="create_ng"){
			res.push_back(Protocol::COM_CREATE_NG);
			
			res.push_back(Protocol::PAR_STRING);
			vector<unsigned char> bytes = intToChar(msg[1].length());
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			for(unsigned char c: msg[1]){
				res.push_back(c);
			}
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="delete_ng"){
			res.push_back(Protocol::COM_DELETE_NG);
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="list_art"){
			res.push_back(Protocol::COM_LIST_ART);
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="create_art"){
			res.push_back(Protocol::COM_CREATE_ART);
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			res.push_back(Protocol::PAR_STRING);
			vector<unsigned char> bytes1 = intToChar(msg[2].length());
			res.insert(res.end(),bytes1.begin(),bytes1.end());
			
			for(char c: msg[2]){
				res.push_back(c);
			}
			
			res.push_back(Protocol::PAR_STRING);
			vector<unsigned char> bytes2 = intToChar(msg[3].length());
			res.insert(res.end(),bytes2.begin(),bytes2.end());
			
			for(unsigned char c: msg[3]){
				res.push_back(c);
			}
			
			res.push_back(Protocol::PAR_STRING);
			vector<unsigned char> bytes3 = intToChar(msg[4].length());
			res.insert(res.end(),bytes3.begin(),bytes3.end());
		
			for(unsigned char c: msg[4]){
				res.push_back(c);
			}
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="delete_art"){
			res.push_back(Protocol::COM_DELETE_ART);
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());

			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes1 = intToChar(atoi(msg[2].c_str()));
			res.insert(res.end(),bytes1.begin(),bytes1.end());
			
			res.push_back(Protocol::COM_END);
		}
		
		else if(command=="get_art"){
			res.push_back(Protocol::COM_GET_ART);
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes1 = intToChar(atoi(msg[2].c_str()));
			res.insert(res.end(),bytes1.begin(),bytes1.end());
			
			res.push_back(Protocol::COM_END);
		}
		
		return res;
		
	}
	
	/*
	 * Decodes the servers resonse to the client on the low level protocol
	 */
	vector<string> ClientCoder::clientDecode(const vector<unsigned char>& msg){
		vector<string> res;
		stringstream ss;

		if(msg[0] == Protocol::ANS_LIST_NG){
			//msg[1] -> PAR_NUM
			
			//Antalet grupper
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << "Nbr of groups: " << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[6] -> PAR_STRING
			
			size_t in=6;

			for(size_t i=0;i<n;++i){
				unsigned int n1 = bytesToInt(msg[in+1],msg[in+2],msg[in+3],msg[in+4]);
				ss << n1 << " ";

				in+=5;
				
				unsigned int n2 = bytesToInt(msg[in+1],msg[in+2],msg[in+3],msg[in+4]);
				
				in += 5;
				
				for(size_t j=0;j<n2;++j){
					ss << msg[in+j];
				}
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
				
				in=in+n2;
			}
		}
		
		else if(msg[0] == Protocol::ANS_CREATE_NG){
			if(msg[1] == Protocol::ANS_ACK) {
				res.push_back("Newsgroup created");
			} else {
				res.push_back("Newsgroup with same name already existed");
			}
		}
		
		else if(msg[0] == Protocol::ANS_DELETE_NG){
			if(msg[1] == Protocol::ANS_ACK) {
				res.push_back("Newsgroup deleted");
			} else {
				res.push_back("No newsgroup with that nbr exists");
			}
		}
		
		else if(msg[0] == Protocol::ANS_CREATE_ART){
			if(msg[1] == Protocol::ANS_ACK) {
				res.push_back("Article created");
			} else {
				res.push_back("No newsgroup with that nbr exists");
			}
		}
			
		else if(msg[0] == Protocol::ANS_LIST_ART){
			if(msg[1] ==  Protocol::ANS_ACK) {
				
				//msg[2] -> PAR_NUM
			
				//Antalet artiklar
				unsigned int n = bytesToInt(msg[3],msg[4],msg[5],msg[6]);
				ss << "Nbr of articles: " << n;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
			
				//msg[7] -> PAR_STRING
			
				size_t in=7;

				for(size_t i=0;i<n;++i){
					unsigned int n1 = bytesToInt(msg[in+1],msg[in+2],msg[in+3],msg[in+4]);
					ss << n1 << " ";
					
					in+=5;
				
					unsigned int n2 = bytesToInt(msg[in+1],msg[in+2],msg[in+3],msg[in+4]);
				
					in += 5;
				
					for(size_t j=0;j<n2;++j){
						ss << msg[in+j];
					}
					res.push_back(ss.str());
					ss.clear();
					ss.str(string());
					
					in=in+n2;
				}
			} else {
				res.push_back("No newsgroup with that nbr exists");
			}
		}
		
		else if(msg[0] == Protocol::ANS_DELETE_ART){
			if(msg[1] == Protocol::ANS_ACK) {
				res.push_back("Article deleted");
			} else {
				if(msg[2] == Protocol::ERR_NG_DOES_NOT_EXIST) {
					res.push_back("No newsgroup with that nbr exists");
				} else {
					res.push_back("No article with that nbr exists");	
				}
			}
		}
		
		else if(msg[0] == Protocol::ANS_GET_ART){
			if(msg[1] == Protocol::ANS_ACK) {
				
				//msg[2] -> PAR_STRING
				
				ss << "********************" << endl;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
				
				//TITLE
				unsigned int n = bytesToInt(msg[3],msg[4],msg[5],msg[6]);
				for(size_t i=7;i<n+7;++i){
					ss << msg[i];
				}
				ss << endl;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
				
				//msg[n+7] -> PAR_STRING
				
				// AUTHOR
				unsigned int n1 = bytesToInt(msg[n+8],msg[n+9],msg[n+10],msg[n+11]);
				
				ss << "AUTHOR: ";
				for(size_t i=n+12;i<n+12+n1;++i){
					ss << msg[i];
				}
				ss << endl;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
				
				//msg[n+12+n1] -> PAR_STRING
				
				// TEXT
				unsigned int n2 = bytesToInt(msg[n+n1+13],msg[n+n1+14],msg[n+n1+15],msg[n+n1+16]);
				
				for(size_t i=n+n1+17;i<n+n1+17+n2;++i){
					ss << msg[i];
				}
				ss << endl;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());
				
				ss << "********************" << endl;
				res.push_back(ss.str());
				ss.clear();
				ss.str(string());

			} else {
				if(msg[2] == Protocol::ERR_NG_DOES_NOT_EXIST) {
					res.push_back("No newsgroup with that nbr exists");
				} else {
					res.push_back("No article with that nbr exists");	
				}
			}
		}
		
		return res;
		
	}
}
