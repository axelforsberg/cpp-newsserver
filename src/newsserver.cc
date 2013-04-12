/* newsserver.cc */

#include "hddstorage.h"
#include "ramstorage.h"
#include "cmdtranslate.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
using namespace client_server;
using namespace protocol;

/*
 * Read chars from client and put in vector
 * Lets the cmd translate the charvector -> stringvector
 * Builds a result stringvector and translate stringvector -> charvector
 * Writes the charvector to client
 */
void exchangeData(CmdTranslate cmd ,Connection* conn) {
	vector<unsigned char> in;
	vector<unsigned char> out;
	unsigned char ch;
		
	while ((ch = conn->read()) != Protocol::COM_END) {
		in.push_back(ch);
	}
	
	out = cmd.execute(in);
	
	for(unsigned char c: out){
		conn->write(c);
	}
	
	in.clear();
	out.clear();
}

/* 
 * Mainfunction for the serverside
 * Creates a storage solution (HDD or RAM)
 * Creates a commandtranslator
 * Creates a server listening on port [argv[1]
 * Reads data from socket and passes it on to the commandtranslator
 * Writes the respones from the commandtranslator to the socket
 */
int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: newsserver port-number" << endl;
		exit(1);
	}
		
	Server server(atoi(argv[1]));
	
	/* CHANGE HERE BETWEEN THE USE OF A RAM/HDD STORAGE SOLUTION */
	//RAMStorage ds;
	HDDStorage ds;

	CmdTranslate cmd(ds);	

	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}
		
	while (true) {
		Connection* conn = server.waitForActivity();
		if (conn != 0) {
			try {
				exchangeData(cmd, conn);
			}
			catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		}
		else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
	}
	return 0;
}
