#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include"Session.h"
#include<thread>

#pragma warning(disable:4996) 

typedef unsigned int int32;
typedef char int8;

class Server
{
#pragma region Networking Specific
	WORD winsock_version;
	WSADATA winsock_data;
    SOCKET sock;
	SOCKADDR_IN local_address;
	int8 buffer[IDENTIFY_BUFFER_SIZE]{""};
	int flags = 0; 
	SOCKADDR_IN from;
	int from_size;
	int bytes_recieved;
#pragma endregion

	

	static const int SessionsAmount = 5;
	Session Sessions[SessionsAmount];


	char SendingData[IDENTIFY_BUFFER_SIZE];
	
	

public:
	std::thread Console_Thread;
	bool is_running = 1;

	Server();

	void StartServer();
	void StartServer(char* _Address, char* _Port);
	void StartConfigServer();
	void ServerUpdate();

	bool WSAStart();

	bool CreateSocket();
	bool CreateSocket(int _AddressFamily, int _Type, int _Protocol);

	bool SetSocketAddress();
	bool SetSocketAddress(char* _Address, int _Port);

	bool RecievedPacket();

	void CreatePacket();
	bool SendPacket();

	void CheckForSession(SOCKADDR_IN _Address);


	void ServerConsole();
};