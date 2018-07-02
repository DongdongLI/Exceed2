#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#define WIN32_MEAN_AND_LEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include "ListeningThread.h"
#pragma comment(lib, "Ws2_32.lib")

//using namespace std;

const int REQ_WINSOCK_VER = 2;
const int PORT = 6000;

void main(int argc, char* argv[]) {
	WSADATA wsaData;
	int iRet = 1;

	if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER, 0), &wsaData) == 0){
		if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER){
			cout << "Initialized. " << endl;

			thread listeningThread(ListeningThread(), PORT);
			listeningThread.join();
		}
		else{
			cerr << "required version not supported!";
		}

		cout << "Cleaning up winsocket... ";

		if (WSACleanup() != 0){
			cerr << "cleanup failed! \n";
			iRet = 1;
		}
		cout << "done.\n ";
	}
	else{
		cerr << "start up failed.\n";
	}
}
