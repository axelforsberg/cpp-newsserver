/* servercoder.cc */

#include "servercoder.h"
#include "protocol.h"

#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace protocol;

namespace client_server{

	/*
	 * Help fuction to translate between a unsigned int and 4 unsigned chars.
	 */
	vector<unsigned char> ServerCoder::intToChar(const unsigned int& i){
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
	unsigned int ServerCoder::bytesToInt(const unsigned char& c1,const unsigned char& c2,const unsigned char& c3,const unsigned char& c4){
		unsigned int i = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
		return i;
	}
		
	/*
	 * Encodes the servers response to the client on the low level protocol
	 */
	vector<unsigned char> ServerCoder::serverEncode(const vector<string>& msg){
		vector<unsigned char> res;
		stringstream ss;
		
		if(msg[0] == "ANS_LIST_NG"){
			res.push_back(Protocol::ANS_LIST_NG);
			res.push_back(Protocol::PAR_NUM);
			vector<unsigned char> bytes = intToChar(atoi(msg[1].c_str()));
			res.insert(res.end(),bytes.begin(),bytes.end());
			
			for(int i=2; i < (atoi(msg[1].c_str())*2+2);i+=2){
				res.push_back(Protocol::PAR_NUM);
				vector<unsigned char> bytes1 = intToChar(atoi(msg[i].c_str()));
				res.insert(res.end(),bytes1.begin(),bytes1.end());

				res.push_back(Protocol::PAR_STRING);
				vector<unsigned char> bytes2 = intToChar(msg[i+1].length());
				res.insert(res.end(),bytes2.begin(),bytes2.end());
				
				for(char c: msg[i+1]){
					res.push_back(c);
				}
			}
			res.push_back(Protocol::ANS_END);
		} 
		
		else if (msg[0] == "ANS_CREATE_NG"){
			res.push_back(Protocol::ANS_CREATE_NG);
			
			if(msg[1] == "ANS_ACK"){
				res.push_back(Protocol::ANS_ACK);
			} else {
				res.push_back(Protocol::ANS_NAK);
				res.push_back(Protocol::ERR_NG_ALREADY_EXISTS);
			}
			res.push_back(Protocol::ANS_END);
		} 
		
		else if (msg[0] == "ANS_DELETE_NG"){
			res.push_back(Protocol::ANS_DELETE_NG);
			
			if(msg[1] == "ANS_ACK"){
				res.push_back(Protocol::ANS_ACK);
			} else {
				res.push_back(Protocol::ANS_NAK);
				res.push_back(Protocol::ERR_NG_DOES_NOT_EXIST);
			}
			res.push_back(Protocol::ANS_END);
		}
		
		else if (msg[0] == "ANS_LIST_ART"){
			res.push_back(Protocol::ANS_LIST_ART);
			
			if(msg[1]=="ANS_ACK"){
				res.push_back(Protocol::ANS_ACK);
				res.push_back(Protocol::PAR_NUM);
				vector<unsigned char> bytes = intToChar(atoi(msg[2].c_str()));
				res.insert(res.end(),bytes.begin(),bytes.end());
				
				for(int i=3; i<(atoi(msg[2].c_str())*2)+3;i+=2){
					res.push_back(Protocol::PAR_NUM);
					vector<unsigned char> bytes1 = intToChar(atoi(msg[i].c_str()));
					res.insert(res.end(),bytes1.begin(),bytes1.end());
					
					res.push_back(Protocol::PAR_STRING);
					
					vector<unsigned char> bytes2 = intToChar(msg[i+1].length());
					res.insert(res.end(),bytes2.begin(),bytes2.end());
					
					for(char c: msg[i+1]){
						res.push_back(c);
					}
				}
			} else {
				res.push_back(Protocol::ANS_NAK);
				res.push_back(Protocol::ERR_NG_DOES_NOT_EXIST);
			}
			res.push_back(Protocol::ANS_END);
		}
		
		else if(msg[0] == "ANS_CREATE_ART"){
			res.push_back(Protocol::ANS_CREATE_ART);
		
			if(msg[1] == "ANS_ACK"){
				res.push_back(Protocol::ANS_ACK);
			} else {
				res.push_back(Protocol::ANS_NAK);
				res.push_back(Protocol::ERR_NG_DOES_NOT_EXIST);
			}
			res.push_back(Protocol::ANS_END);
		}
		
		else if(msg[0] == "ANS_DELETE_ART"){
			res.push_back(Protocol::ANS_DELETE_ART);
			
			if(msg[1] == "ANS_ACK"){
				res.push_back(Protocol::ANS_ACK);	
			} else {
				res.push_back(Protocol::ANS_NAK);
				if(msg[2] == "ERR_NG_DOES_NOT_EXIST"){
					res.push_back(Protocol::ERR_NG_DOES_NOT_EXIST);
				} else {
					res.push_back(Protocol::ERR_ART_DOES_NOT_EXIST);
				}
			}
			res.push_back(Protocol::ANS_END);
		}
		
		else if(msg[0] == "ANS_GET_ART") {
			res.push_back(Protocol::ANS_GET_ART);
			
			if(msg[1]  == "ANS_ACK"){
			
				res.push_back(Protocol::ANS_ACK);
				
				//Title
				res.push_back(Protocol::PAR_STRING);
				vector<unsigned char> bytes = intToChar(msg[2].length());
				res.insert(res.end(),bytes.begin(),bytes.end());
				
				for(char c: msg[2]){
					res.push_back(c);
				}
				
				//Author
				res.push_back(Protocol::PAR_STRING);
				vector<unsigned char> bytes2 = intToChar(msg[3].length());
				res.insert(res.end(),bytes2.begin(),bytes2.end());
				
				for(char c: msg[3]){
					res.push_back(c);
				}
				
				//Text
				res.push_back(Protocol::PAR_STRING);
				vector<unsigned char> bytes3 = intToChar(msg[4].length());
				res.insert(res.end(),bytes3.begin(),bytes3.end());
				
				for(char c: msg[4]){
					res.push_back(c);
				}
			} else {
				res.push_back(Protocol::ANS_NAK);
				if(msg[2]=="ERR_NG_DOES_NOT_EXIST"){
					res.push_back(Protocol::ERR_NG_DOES_NOT_EXIST);
				} else {
					res.push_back(Protocol::ERR_ART_DOES_NOT_EXIST);
				}
			}
			res.push_back(Protocol::ANS_END);
		} 
		
		return res;
		
	}
	
	/*
	 * Decodes the clients request to the server from the low level protocol
	 */
	vector<string> ServerCoder::serverDecode(const vector<unsigned char>& msg){
		vector<string> res;
		stringstream ss;

		if(msg[0] == Protocol::COM_LIST_NG){
			//Included for completeness of protocol list
		}

		else if(msg[0] == Protocol::COM_CREATE_NG){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			
			for(size_t i=6;i<n+6;++i){
				ss << msg[i];
			}
			
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
		} 
		
		else if(msg[0] == Protocol::COM_DELETE_NG){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
		} 
		
		else if(msg[0] == Protocol::COM_LIST_ART){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
		} 
		
		else if(msg[0] == Protocol::COM_CREATE_ART){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[6] -> PAR_STRING
			
			//Title
			unsigned int n1 = bytesToInt(msg[7],msg[8],msg[9],msg[10]);
			for(size_t i=11;i<n1+11;++i){
				ss << msg[i];
			}
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[11] -> PAR_STRING
			
			//Author
			unsigned int n2 = bytesToInt(msg[n1+12],msg[n1+13],msg[n1+14],msg[n1+15]);
			for(size_t i=n1+16;i<(n1+16)+n2;++i){
				ss << msg[i];
			}
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[n1+n2+16] -> PAR_STRING
	
			//Text
			unsigned int n3 = bytesToInt(msg[(n1+17)+n2],msg[(n1+18)+n2],msg[(n1+19)+n2],msg[(n1+20)+n2]);
			for(size_t i=(n1+21)+n2;i<((n1+21)+n2)+n3;++i){
				ss << msg[i];
			}
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());	
		} 
		
		else if(msg[0] == Protocol::COM_DELETE_ART){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[6] -> PAR_NUM
			
			unsigned int n1 = bytesToInt(msg[7],msg[8],msg[9],msg[10]);
			ss << n1;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
		} 
		
		else if(msg[0] == Protocol::COM_GET_ART){
			//msg[1] -> PAR_NUM
			
			unsigned int n = bytesToInt(msg[2],msg[3],msg[4],msg[5]);
			ss << n;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			//msg[6] -> PAR_NUM
			
			unsigned int n1 = bytesToInt(msg[7],msg[8],msg[9],msg[10]);
			ss << n1;
			res.push_back(ss.str());
			ss.clear();
			ss.str(string());
		}
		
		return res;
		
	}
}
