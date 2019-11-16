#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996) 

typedef unsigned int int32;
typedef char int8;//8 bytes of memory

class Client
{
	WORD winsock_version;
	WSADATA winsock_data;
	SOCKET sock;
	SOCKADDR_IN server_address;

	int8 buffer[IDENTIFY_BUFFER_SIZE];
	int flags;

	SOCKADDR_IN from;
	int from_size;
	int bytes_received;

	char RecievedData[IDENTIFY_BUFFER_SIZE];
	char SentData[IDENTIFY_BUFFER_SIZE];

public:
	bool is_running = 1;

	void StartClient();
	void StartCustomClient();
	void UpdateClient();

	bool WSAStart();
	void AssignServerAddress();
	void AssignServerSocket();
	
	void CustomServerAddress(char* _Address, char* _Port);

	bool SendPacket();
	bool SendPacket(const char* _data);
	bool ReceivePacket();
};