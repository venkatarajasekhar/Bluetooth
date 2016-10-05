#include <iostream>
#include <WinSock2.h>
#include <ws2bth.h>

#pragma comment (lib, "Ws2_32.lib")

#define CHANNEL 30

int main()
{
	//Startup WSA
	WSADATA wsaData;
	std::cout << "Starting WSA...";
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	std::cout << "Started." << std::endl;

	//Create Socket
	std::cout << "Creating Socket...";
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)) == INVALID_SOCKET)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	std::cout << "Created." << std::endl;

	//Define Socket Address
	SOCKADDR_BTH sa;
	memset(&sa, 0, sizeof(sa));
	sa.addressFamily = AF_BTH;
	sa.btAddr = 0xe006e6d5e4de;
	sa.port = CHANNEL & 0xff;

	//Connect to Server
	std::cout << "Connecting To Server...";
	if ((connect(clientSocket, (SOCKADDR *)&sa, sizeof(sa))) == SOCKET_ERROR)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	std::cout << "Connected." << std::endl;

	//Send Data
	std::cout << "Sending Data...";
	char * sendbuf = "this is a test";
	if ((send(clientSocket, sendbuf, strlen(sendbuf), 0)) == SOCKET_ERROR)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}

	//Cleanup
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}
