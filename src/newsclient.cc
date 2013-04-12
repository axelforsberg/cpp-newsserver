/* newsclient.cc */

#include "connection.h"
#include "connectionclosedexception.h"
#include "clientcoder.h"
#include "protocol.h"

#include <vector>
#include <limits>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using namespace client_server;
using namespace protocol;

/*
 * Takes a stringvector in with clients request.
 * Lets the ClientCoder encode the msg to a charvector.
 * Sends it to the server.
 * Writes the response to a charvector.
 * Decode the response and prints the result to cout.
 */
void exchangeData(ClientCoder cc ,Connection& conn, vector<string> reqMsg) {
	vector<unsigned char> out;
	vector<unsigned char> in;
	vector<string> ansMsg;
	unsigned char ch;
	
	out = cc.clientEncode(reqMsg);
	
	for(unsigned char c: out){
		conn.write(c);
	}
	
	while ((ch = conn.read()) != Protocol::ANS_END) {
		in.push_back(ch);
	}
	
	ansMsg = cc.clientDecode(in);

	cout << endl;
	for(string s: ansMsg){
		cout << "  " << s << endl;
	}
	
	ansMsg.clear();
	in.clear();
	out.clear();
	
}

/*
 * The following fuctions prints questions to commandline and read the the users answers. 
 */

void printCreateNG(vector<string>& msg){
		string line;

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << "Pick a groupname:" << endl;
		getline(cin,line);
		msg.push_back(line);
}
	
void printListARTandDeleteNG(vector<string>& msg){
		string line;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << "Pick a group number:" << endl;
		getline(cin,line);
		msg.push_back(line);
}
	
void printCreateART(vector<string>& msg){
		string line;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << "Pick a group number:" << endl;
		getline(cin,line);
		msg.push_back(line);
		cout << "Pick a title:" << endl;
		getline(cin,line);
		msg.push_back(line);
		cout << "Pick a author" << endl;
		getline(cin,line);
		msg.push_back(line);
		cout << "Write your text:" << endl;
		getline(cin,line);
		msg.push_back(line);
}
	
void printGetARTandDeleteART(vector<string>& msg){
		string line;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << "Pick a group number:" << endl;
		getline(cin,line);
		msg.push_back(line);
		cout << "Pick a article number:" << endl;
		getline(cin,line);
		msg.push_back(line);
}

void printHelp(){
	cout << endl << "  Write a command and press enter and the " << endl << "  program will guide you through that commands options" << endl;
	cout <<"  * create_ng   <- To create a newsgroup" << endl;
	cout <<"  * list_ng     <- To list existing newsgroups" <<endl;
	cout <<"  * delete_ng   <- To delete a newsgroup" <<endl;
	cout <<"  * create_art  <- To create an article"  <<endl;
	cout <<"  * list_art    <- To list articles in a newsgroup" <<endl;
	cout <<"  * delete_art  <- To delete an article" << endl;
	cout <<"  * get_art     <- To read an article" << endl;
	cout <<"  * exit        <- To disconnect for server" << endl;
}

/* 
 * Mainfunction for the clientside
 * Reads input from commandline
 * Creates a vector<string> with input
 * Encode the vector with the low level protocol
 * Writes the data to the socket
 * Read the response from socket
 * Decode the respose and print to commandline
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
	
	ClientCoder cc;
    string line;
	vector<string> reqMsg;
	
    Connection conn(argv[1], atoi(argv[2]));
    if (! conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
	
	cout << endl << "Welcome to AS News!" << endl;
	cout << "Type help to list available comamnds"<< endl << endl;
	
	while(true) {
		try {
			cout << endl << "Type a command:" << endl;
			cin>>line;
			reqMsg.push_back(line);

			if (reqMsg[0] == "help"){
				printHelp();
				reqMsg.clear();
			} else { 
				if(reqMsg[0] == "create_ng") {
					printCreateNG(reqMsg);
					exchangeData(cc, conn, reqMsg);
				reqMsg.clear();
				} else if(reqMsg[0] == "delete_ng" ||reqMsg[0] == "list_art"){
					printListARTandDeleteNG(reqMsg);
					exchangeData(cc, conn, reqMsg);
				} else if(reqMsg[0] == "create_ng"){
					printCreateART(reqMsg);
					exchangeData(cc, conn, reqMsg);
				} else if(reqMsg[0] == "create_art"){
					printCreateART(reqMsg);
					exchangeData(cc, conn, reqMsg);
				} else if(reqMsg[0] == "get_art" ||reqMsg[0] == "delete_art"){
					printGetARTandDeleteART(reqMsg);
					exchangeData(cc, conn, reqMsg);
				} else if(reqMsg[0] == "list_ng"){
					exchangeData(cc, conn, reqMsg);
				} else if(reqMsg[0] == "exit"){
					return 0;
				} else {
					cout << "Undefined command" << endl;
					cout << "Type help to list available commands" << endl;
				}
				reqMsg.clear();
			}
        } 

        catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            exit(1);
        }
    }
}
