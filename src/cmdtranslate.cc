/* comdtranslate.cc */

#include "databaseinterface.h"
#include "cmdtranslate.h"
#include "servercoder.h"
#include "protocol.h"

#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace protocol;

namespace client_server{
	
	/* 
	 * This function takes a encoded message in 
	 * Decode the message and Interprents it
	 * Creates an response containing appropriate data 
	 * Encodes the response
	 * Returns the encoded response
 	 */
	vector<unsigned char> CmdTranslate::execute(const vector<unsigned char>& encodedMsg){
		vector<string> decodedMsg = sc.serverDecode(encodedMsg);
		vector<string> resultMsg;
		stringstream ss;
		
		if(encodedMsg[0] == Protocol::COM_LIST_NG){
			vector<string> temp = db.listNewsGroups();

			resultMsg.push_back("ANS_LIST_NG");

			int size = temp.size()/2;
			ss << size;
			resultMsg.push_back(ss.str());
			ss.clear();
			ss.str(string());
			
			resultMsg.insert(resultMsg.end(),temp.begin(),temp.end());
		} 
		
		else if(encodedMsg[0] == Protocol::COM_CREATE_NG){
			resultMsg.push_back("ANS_CREATE_NG");
			
			if(db.createNewsGroup(decodedMsg[0])){
				resultMsg.push_back("ANS_ACK");
			} else {
				resultMsg.push_back("ANS_NAK");
			}
		}
		
		else if(encodedMsg[0] == Protocol::COM_DELETE_NG){
			resultMsg.push_back("ANS_DELETE_NG");
			
			if(db.deleteNewsGroup(atoi(decodedMsg[0].c_str()))){
				resultMsg.push_back("ANS_ACK");
			} else {
				resultMsg.push_back("ANS_NAK");
			}
		}
		
		else if(encodedMsg[0] == Protocol::COM_LIST_ART){
			vector<string> temp = db.listArticles(atoi(decodedMsg[0].c_str()));
			resultMsg.push_back("ANS_LIST_ART");
			
				if(temp[0] == "ERR_NG_DOES_NOT_EXIST"){
					resultMsg.push_back("ANS_NAK");
				} else if(temp[0] == "ERR_NO_ARTS"){
					resultMsg.push_back("ANS_ACK");
					resultMsg.push_back("0");
				} else {
					resultMsg.push_back("ANS_ACK");
					int size = temp.size()/2;
					ss << size;
					resultMsg.push_back(ss.str());
					ss.clear();
					ss.str(string());
					resultMsg.insert(resultMsg.end(),temp.begin(),temp.end());
				}
		}
		
		else if(encodedMsg[0] == Protocol::COM_CREATE_ART){
			resultMsg.push_back("ANS_CREATE_ART");

			if(db.createArticle(atoi(decodedMsg[0].c_str()), decodedMsg[1], decodedMsg[2], decodedMsg[3])){
				resultMsg.push_back("ANS_ACK");
			} else {
				resultMsg.push_back("ANS_NAK");
			}
		}
		
		else if(encodedMsg[0] == Protocol::COM_GET_ART){
			vector<string> temp = db.getArticle(atoi(decodedMsg[0].c_str()), atoi(decodedMsg[1].c_str()));
			resultMsg.push_back("ANS_GET_ART");

			if(temp[0] == "ERR_NG_DOES_NOT_EXIST") {
				resultMsg.push_back("ANS_NAK");
				resultMsg.push_back("ERR_NG_DOES_NOT_EXIST");
			} else if (temp[0] == "ERR_ART_DOES_NOT_EXIST") {
				resultMsg.push_back("ANS_NAK");
				resultMsg.push_back("ERR_ART_DOES_NOT_EXIST");
			} else {
				resultMsg.push_back("ANS_ACK");
				resultMsg.insert(resultMsg.end(),temp.begin(),temp.end());
			}
		}
		
		else if(encodedMsg[0] == Protocol::COM_DELETE_ART){
			string temp = db.deleteArticle(atoi(decodedMsg[0].c_str()), atoi(decodedMsg[1].c_str()));
			resultMsg.push_back("ANS_DELETE_ART");
			
			if(temp == "ERR_NG_DOES_NOT_EXIST") {
				resultMsg.push_back("ANS_NAK");
				resultMsg.push_back("ERR_NG_DOES_NOT_EXIST");
			}
			else if(temp == "ERR_ART_DOES_NOT_EXIST") {
					resultMsg.push_back("ANS_NAK");
					resultMsg.push_back("ERR_ART_DOES_NOT_EXIST");
			} else {
				resultMsg.push_back("ANS_ACK");
			}
		}
		
		return sc.serverEncode(resultMsg);
		
	}
}
