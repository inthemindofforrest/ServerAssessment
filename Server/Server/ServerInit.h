#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include"Session.h"
#include<thread>
#include<atomic>

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
	
	void Shutdown();

public:
	std::thread Console_Thread;
	std::atomic<bool> is_running;

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
	void ProccessPacket();

	void CreatePacket();
	bool SendPacket();
	bool SendPacket(const char* _Message);

	void CheckForSession(SOCKADDR_IN _Address);
	void CheckForSession(SOCKADDR_IN _Address, int _SessionIndex);
	int* GetSessionCount();
	void DisconnectFromSessions(SOCKADDR_IN _Address);

	void ServerConsole();

	static bool MatchingSockAddress(SOCKADDR_IN _First, SOCKADDR_IN _Second);
	static bool ResetSockAddress(SOCKADDR_IN* _Address);
};