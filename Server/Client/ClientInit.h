#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include<thread>
#include <atomic>

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

	std::atomic<bool> HasSentMessage;

public:

	std::thread Recieve_Thread;

	std::atomic<bool> is_running;

	void StartClient();
	bool StopClient();
	void StartCustomClient();
	void StartCustomClient(const char* _IP, int _Port);
	void UpdateClient();

	bool WSAStart();
	void AssignServerAddress();
	void AssignServerSocket();
	
	void CustomServerAddress(char* _Address, char* _Port);
	void CustomServerAddress(const char* _Address, int _Port);

	bool SendPacket();
	bool SendPacket(const char* _data);
	bool SendPacket(const char* _data, int _Size, int _NumData);
	bool ReceivePacket();
	bool ClientConsole(char* _Message);

	void DisplayConnection(const char * _data);
	void ProcessPacket(const char* _Data);

};