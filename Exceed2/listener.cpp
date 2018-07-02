#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

#define WIN32_MEAN_AND_LEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class ROTException{
public:
	ROTException() :
		message(""){}
	virtual ~ROTException() {}
	ROTException(char * msg) :
		message(msg){}
	const char * what(){
		return message;
	}
private:
	const char* message;
};

class thread_obj {
public:
	int operator()(){
		
		return 0;
	}
};

const int REQ_WINSOCK_VER = 2;
const int PORT = 6000;
const int TEMP_BUFFER_SIZE = 128;

string getHostDescription(const sockaddr_in &sockAddr) {
	ostringstream stream;
	stream << inet_ntoa(sockAddr.sin_addr) << ":" << ntohs(sockAddr.sin_port);
	return stream.str();
}

void setServerSockAddr(sockaddr_in *sockAddr, int portNumber){
	sockAddr->sin_family = AF_INET;
	sockAddr->sin_port = htons(portNumber);
	sockAddr->sin_addr.S_un.S_addr = INADDR_ANY;
}

void browse(string url){
	string command = "rundll32 url.dll,FileProtocolHandler " + url;
	system(command.c_str());
	// todo. call the command more directly
}

void handleConnection(SOCKET clientSocket, const sockaddr_in &sockAddr){
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
			if (tempBuffer[0] == '\r'){
				cout << "found a enter" << endl;
				cout << "url: " << url << endl;
				browse(url);
				url = "";
			}

			url += tempBuffer[0];
			if (send(clientSocket, tempBuffer, res, 0) == SOCKET_ERROR)
				throw ROTException("socket error while sending");
		}
	}

	cout << "Connection closed.\n";
}

bool RunServer(int portNumber){
	SOCKET hSocket = INVALID_SOCKET,
		hClientSocket = INVALID_SOCKET;

	bool success = true;
	sockaddr_in sockAddr = { 0 };

	try{
		cout << "creating socket... ";
		if ((hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
			throw ROTException("Could not create socket");
		cout << "created.\n";

		cout << "Binding socket... ";
		setServerSockAddr(&sockAddr, portNumber);
		if (bind(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) != 0)
			throw ROTException("could not bind socket.");
		cout << "bound. \n";

		cout << "Putting socket in listening mode... ";
		if (listen(hSocket, SOMAXCONN) != 0)
			throw ROTException("could not put socket in listening mode.");
		cout << "done.\n";

		cout << "Waiting for incoming connection... ";

		sockaddr_in clientSockAddr;
		int	clientSockSize = sizeof(clientSockAddr);


		hClientSocket = accept(hSocket, reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockSize);

		if (hClientSocket == INVALID_SOCKET)
			throw ROTException("accept function failed.");
		cout << "accepted.\n";

		handleConnection(hClientSocket, clientSockAddr);
	}
	catch (ROTException e){
		cerr << "\nError: " << e.what() << endl;
		success = false;
	}

	if (hSocket != INVALID_SOCKET)
		closesocket(hSocket);
	if (hClientSocket != INVALID_SOCKET)
		closesocket(hClientSocket);

	return success;
}

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	int iRet = 1;

	if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER, 0), &wsaData) == 0){
		if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER){
			cout << "Initialized. " << endl;

			int port = PORT;
			iRet = !RunServer(port);
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


	return iRet;
}
