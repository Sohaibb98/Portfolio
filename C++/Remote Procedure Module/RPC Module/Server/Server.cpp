#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <string>

#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")
#define Port 6000

using std::vector;
using std::thread;
using std::mutex;
using std::string;

vector<std::pair<SOCKET, sockaddr_in>> currentConnections;

mutex mtx;


void AcceptConnection() {

	SOCKET Socket, Sub;
	WSADATA Winsock;
	sockaddr_in Addr;
	sockaddr_in IncomingAddress;
	int AddressLen = sizeof(IncomingAddress);

	if (WSAStartup(MAKEWORD(2, 2), &Winsock) != 0) {
		std::cout << "WinSock startup failed." << std::endl;
		exit(-1);
	}

	if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
	{
		WSACleanup();
		exit(-1);
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET) {
		std::cout << "Server socket failed" << std::endl;
		exit(-1);
	}
	ZeroMemory(&Addr, sizeof(Addr));

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);

	long setOpt = 1;
	setsockopt(Socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&setOpt, sizeof(setOpt));

	if (bind(Socket, (sockaddr*)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
		std::cout << "Server socket bind failed" << std::endl;
		exit(-1);
	}


	if (listen(Socket, 10) == SOCKET_ERROR) {
		std::cout << "Listen error " << WSAGetLastError() << "! Listen failed." << std::endl;
		exit(-1);
	}
	else {
		std::cout << "Server listening on port " << Port << std::endl;
	}

	while (true) {
		if (Sub = accept(Socket, (sockaddr*)&IncomingAddress, &AddressLen)) {

			char *ClientIP = inet_ntoa(IncomingAddress.sin_addr);
			int ClientPort = ntohs(IncomingAddress.sin_port);

			std::cout << "Client conncted IP: " << ClientIP << ":" << ClientPort << std::endl;

			mtx.lock();
			currentConnections.push_back(std::make_pair(Sub, IncomingAddress));
			mtx.unlock();
		}
	}

	closesocket(Socket);
	WSACleanup();
}

bool SendFile(SOCKET& Sub, const int* Buffer, string fileName, int Size) {

	if (send(Sub, (char*)&Size, sizeof(Size), 0) != SOCKET_ERROR) {
		uint32_t dataLength = htonl(fileName.size());
		send(Sub, (char*)&dataLength ,sizeof(uint32_t) ,0); // Send the data length
		send(Sub, fileName.c_str(), fileName.size(),0);  
		for (int i = 0; i < Size;) {
			int tempSize = (Size - i >= 1024) ? 1024 : Size - i;
			int* temp = new int[tempSize];
			memcpy(temp, Buffer + i, tempSize * sizeof(int));
			if (send(Sub, (char*)temp, tempSize * sizeof(int), 0) == SOCKET_ERROR) {
				printf("Error sending binary!\n");
				delete[] temp;
				return false;
			}
			delete[] temp;
			i += tempSize;
		}
		return true;
	}
	else {
		printf("Error sending binary!\n");
		return false;
	}
}
int SendFile(const string& path) {

	FILE *File;
	long Size;
	
	
	string fileName = path;
	
	size_t pos = 0;
	string token;
	while ((pos = fileName.find("/")) != string::npos) {
	    token = fileName.substr(0, pos);
	    fileName.erase(0, pos + 1);
	}
	
	File = fopen(path.c_str(), "rb");
	if (!File) {
		std::cout << "Error while readaing the file" << std::endl;
		return 0;
	}

	fseek(File, 0, SEEK_END);
	Size = ftell(File);
	fseek(File, 0, SEEK_SET);

	int *Buffer = new int[Size];
	int ch;
	int ind = 0;
	while ((ch = fgetc(File)) != EOF) {
		Buffer[ind++] = ch;
	}
	fclose(File);

	int success = 0;
	for (auto& conn : currentConnections) {
		
		if (SendFile(conn.first, Buffer, fileName, Size))
			++success;
	}

	delete[] Buffer;
	return success;
}

vector<string> parse(const string& str, char delim = ' ') {

	vector<string> tokens;
	string curr;
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] != delim)
			curr += str[i];
		else {
			if (curr.length()) {
				tokens.push_back(curr);
				curr = "";
			}
		}
	}
	if (curr.length()) {
		tokens.push_back(curr);
		curr = "";
	}
	return tokens;
}
int main(int argc, char** argv) {

	std::thread listenThread(AcceptConnection);

	string cmd;
	while (true) {
		
		std::getline(std::cin, cmd);

		vector<string> tokens = parse(cmd);
		if (tokens.size() == 2 && tokens[0] == "send") {
			
			int sent = SendFile(tokens[1]);
			if (sent > 0) {
				std::cout << "Sent file to " << sent << " clients." << std::endl;
			}
			else {
				std::cout << "Could not send file to any clients." << std::endl;
			}
		}
	}

	getchar();
	return 0;
}
