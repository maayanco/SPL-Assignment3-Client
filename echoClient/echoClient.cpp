#include <stdlib.h>
#include <boost/locale.hpp>
//#include "C:/installation/path/include/boost-1_60/boost/typeof/std/locale.hpp"
#include "connectionHandler.h"
#include "../encoder/utf8.h"
#include "../encoder/encoder.h"
#include <boost/thread/thread.hpp>
//#include "C:/installation/path/include/boost-1_60/boost/thread/thread.hpp"


std::string readFromUser(){
	const short bufsize = 1024;
	char buf[bufsize];
	std::cin.getline(buf, bufsize);
	std::string line(buf);
	return line;
}


void readInputFromUserAndSendToServer(ConnectionHandler* connectionHandler){
	std::string input = readFromUser();
	if(input=="quit"){
		connectionHandler->close();
	}
	std::cout << "sending to server: "<< input << std::endl;
	connectionHandler->sendLine(input); //debug: what if returns false?
}

void getMessageFromUser(ConnectionHandler* connectionHandler){
	std::cout << "inside thread "<<std::endl;
	while(connectionHandler->isOpen()==true){
		readInputFromUserAndSendToServer(connectionHandler);
	}

}

void getMessageFromServer(ConnectionHandler* connectionHandler){
	std::cout << "inside thread" << std::endl;
	while(connectionHandler->isOpen()==true){
		std::string answer;
		bool responseStatus = connectionHandler->getLine(answer);
		if(responseStatus==true){
			std::cout << answer << std::endl; //debug?
			//we will check if the message that was received requires respond, if so we will request input
			//from the user and send it
			if((answer.find("ASKTXT")!=std::string::npos) || (answer.find("ASKCHOICES")!=std::string::npos)){
				readInputFromUserAndSendToServer(connectionHandler);
			}
		}
	}

}

void readerFunc(ConnectionHandler* connectionHandler){}

/**
 * This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
 */
int main (int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "Please enter host and port parameters" << std::endl;
		return -1;
	}
	std::string host = argv[1];
	short port = atoi(argv[2]);

	ConnectionHandler connectionHandler(host, port);
	if (!connectionHandler.connect()) {
		std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
		return 1;
	}
	else{
		std::cerr << "Connection to server was established successfully" << std::endl;
	}

	std::cout<< "give me the status of the connectionHandler: " << connectionHandler.isOpen() <<std::endl;

	boost::thread serverListener (getMessageFromServer, &connectionHandler);
	boost::thread userInputListener (getMessageFromUser, &connectionHandler);

	serverListener.join();
	userInputListener.join();

	return 0;
}


