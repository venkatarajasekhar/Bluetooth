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
	SOCKET serverSocket;
	if ((serverSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)) == INVALID_SOCKET)
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
	sa.port = CHANNEL & 0xff;

	//Bind to Address
	std::cout << "Binding Address...";
	if (bind(serverSocket, (SOCKADDR *)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	std::cout << "Bound." << std::endl;

	//Set Up Listening
	std::cout << "Listening...";
	if (listen(serverSocket, 5) != 0)
	{
		std::cout << "Failed: " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	std::cout << "Set Up." << std::endl;

	//Keep Recieving Connections
	while (true)
	{
		//Accept Connection
		std::cout << "Accepting Connection...";
		SOCKADDR_BTH sa2;
		int size = sizeof(sa2);
		SOCKET s2;
		if ((s2 = accept(serverSocket, (SOCKADDR *)&sa2, &size)) == INVALID_SOCKET)
		{
			std::cout << "Failed: " << WSAGetLastError() << std::endl;
			getchar();
			return 1;
		}
		std::cout << "Accepted." << std::endl;

		//Recieve Data
		std::cout << "Recieving Data...";
		char recvbuf[512];
		int result;
		if ((result = recv(s2, recvbuf, 512, 0)) == SOCKET_ERROR)
		{
			std::cout << "Failed: " << WSAGetLastError() << std::endl;
			getchar();
			return 1;
		}
		std::cout << "Recieved." << std::endl << std::endl;

		//Display Data
		for (int i = 0; i < result; i++)
			std::cout << recvbuf[i];

		std::cout << std::endl << std::endl;

		//Close Recieved Socket
		closesocket(s2);
	}

	//Cleanup
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}
