#include "ROTException.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

const int TEMP_BUFFER_SIZE = 128;

class thread_obj {
public:
	
	static string getHostDescription(const sockaddr_in &sockAddr) {
		ostringstream stream;
		stream << inet_ntoa(sockAddr.sin_addr) << ":" << ntohs(sockAddr.sin_port);
		return stream.str();
	}

	static void browse(string url){
		string command = "rundll32 url.dll,FileProtocolHandler " + url;
		system(command.c_str());
		// todo. call the command more directly//MSVCP120D.dll
	}

	int operator()(SOCKET clientSocket, const sockaddr_in &sockAddr){

		cout << "Connected with" << getHostDescription(sockAddr) << ".\n";
		char tempBuffer[TEMP_BUFFER_SIZE];
		string url;

		while (true){
			int res;
			res = recv(clientSocket, tempBuffer, sizeof(tempBuffer), 0);
			if (res == 0){
				break;
			}
			else if (res == SOCKET_ERROR){
				throw ROTException("socket error while receiving. ");
			}
			else{
				
				cout << tempBuffer[0] << endl;
				if (tempBuffer[0] == '\r' || tempBuffer[0] == '\n'){
					cout << "found a enter" << endl;
					cout << "url: " << url << endl;
					if (!isEmpty(url))
						browse(url);
					url = "";
				}
				else{
					//url += tempBuffer[0];
					for (int i = 0; i < res; i++)
						url += tempBuffer[i];
				}
				
				/*
				for (int i = 0; i < res; i++){
					if (tempBuffer[i] != '\0' 
						&& tempBuffer[i] != '\n' 
						&& tempBuffer[i] != '\r'){
						url += tempBuffer[i];
					}
					else{
						cout << "url: " << url << endl;
						browse(url);
						url = "";
					}
						
				}
				*/
				if (send(clientSocket, tempBuffer, res, 0) == SOCKET_ERROR)
					throw ROTException("socket error while sending");
			}
		}

		cout << "Connection closed.\n";
	}

	static boolean isEmpty(string url){
		if (url.empty() || url.length() == 0)
			return true;

		int characterCount = 0;
		for (int i = 0; i < url.length(); i++){
			if (url.at(i) != '\n'
				&& url.at(i) != '\r'
				&& url.at(i) != '\0'
				&& url.at(i) != ' ')
				
				characterCount++;
		}

		return characterCount == 0;
	}
};