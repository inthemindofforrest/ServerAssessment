#include "ClientInit.h"
#include <cstdlib>

void Client::StartClient()
{
	WSAStart();
	AssignServerAddress();
	AssignServerSocket();

	is_running = true;
	HasSentMessage = false;
	IsConnected = false;
	//printf("Client: %d.%d.%d.%d:%d trying to join Server: %d.%d.%d.%d:%d")

	Recieve_Thread = std::thread([&]
	{
		while (is_running)
		{
			ReceivePacket();
		}
	});
}

bool Client::StopClient()
{
	if (shutdown(sock, SD_BOTH) == SOCKET_ERROR)
	{
		printf("Shutdown failed: %d on line: %d", WSAGetLastError(), __LINE__);
		closesocket(sock);
		WSACleanup();
		return false;
	}
	printf("Disconnected from server.\n");
	IsConnected = false;
	return true;
}

void Client::StartCustomClient()
{
	WSAStart();
	char Address[IDENTIFY_BUFFER_SIZE];
	char Port[IDENTIFY_BUFFER_SIZE];
	printf("IPv4: ");
	gets_s(Address, IDENTIFY_BUFFER_SIZE);
	printf("Port: ");
	gets_s(Port, IDENTIFY_BUFFER_SIZE);

	CustomServerAddress(Address, Port);
	AssignServerSocket();

	is_running = true;
	SendPacket("Join");



	Recieve_Thread = std::thread([&]
	{
		while (is_running)
		{
			ReceivePacket();
		}
	});


}
void Client::StartCustomClient(const char* _IP, int _Port)
{
	WSAStart();

	CustomServerAddress(_IP, _Port);
	AssignServerSocket();

	is_running = true;
	SendPacket("Join");



	Recieve_Thread = std::thread([&]
	{
		while (is_running)
		{
			ReceivePacket();
		}
	});


}

void Client::UpdateClient()
{
	if (is_running)
	{
		SendPacket();
	}
}

bool Client::WSAStart()
{
	if (WSAStartup(winsock_version, &winsock_data))//Return 0 if successful
	{
		printf("WSAStartup failed: %d on line: %d", WSAGetLastError(), __LINE__);
		return false;
	}
	return true;
}

void Client::AssignServerAddress()
{
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9999);
	server_address.sin_addr.S_un.S_addr = inet_addr("10.15.20.13");
}

void Client::AssignServerSocket()
{
	int address_family = AF_INET;//Acts as the IPv4 address
	int type = SOCK_DGRAM;// Type is a UDP socket(SOCK_DGRAM)
	int protocol = IPPROTO_UDP;//
	sock = socket(address_family, type, protocol);
}

void Client::CustomServerAddress(char * _Address, char* _Port)
{
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(_Port));
	server_address.sin_addr.S_un.S_addr = inet_addr(_Address);
}
void Client::CustomServerAddress(const char * _Address, int _Port)
{
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(_Port);
	server_address.sin_addr.S_un.S_addr = inet_addr(_Address);
}

bool Client::SendPacket()
{
	gets_s(SentData, IDENTIFY_BUFFER_SIZE);

	if (ClientConsole(SentData))
	{
		return false;
	}

	if (is_running)
	{
		flags = 0;
		if (sendto(sock, SentData, sizeof(SentData), flags, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
		{
			printf("sendto failed: %d on line: %d", WSAGetLastError(), __LINE__);

			return false;
		}
		else
		{
			printf("Sending Package to: %d.%d.%d.%d:%d\n",
				server_address.sin_addr.S_un.S_un_b.s_b1,
				server_address.sin_addr.S_un.S_un_b.s_b2,
				server_address.sin_addr.S_un.S_un_b.s_b3,
				server_address.sin_addr.S_un.S_un_b.s_b4,
				server_address.sin_port);
		}
		HasSentMessage = true;
	}
	return true;
}
bool Client::SendPacket(const char* _data)
{
	strcpy(SentData, _data);
	if (is_running)
	{
		flags = 0;
		if (sendto(sock, SentData, sizeof(SentData), flags, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
		{
			printf("sendto failed: %d on line: %d", WSAGetLastError(), __LINE__);

			return false;
		}
		else
		{
			printf("Sending Package to: %d.%d.%d.%d:%d\n",
				server_address.sin_addr.S_un.S_un_b.s_b1,
				server_address.sin_addr.S_un.S_un_b.s_b2,
				server_address.sin_addr.S_un.S_un_b.s_b3,
				server_address.sin_addr.S_un.S_un_b.s_b4,
				server_address.sin_port);
		}
		HasSentMessage = true;
	}
	return true;
}

bool Client::SendPacket(const char * _data, int _Size, int _NumData)
{
	if (is_running)
	{
		int Index = 0;
		char temp[IDENTIFY_BUFFER_SIZE];

		memcpy(&temp[Index], &_NumData, sizeof(_NumData));
		Index += sizeof(_NumData);
		
		memcpy(&temp[Index], &_Size, sizeof(_Size));
		Index += sizeof(_Size);

		memcpy(&temp[Index], _data, _Size);
		Index += _Size;

		flags = 0;
		if (sendto(sock, temp, sizeof(temp), flags, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
		{
			printf("sendto failed: %d on line: %d", WSAGetLastError(), __LINE__);

			return false;
		}
		else
		{
			printf("Sending Package to: %d.%d.%d.%d:%d\n",
				server_address.sin_addr.S_un.S_un_b.s_b1,
				server_address.sin_addr.S_un.S_un_b.s_b2,
				server_address.sin_addr.S_un.S_un_b.s_b3,
				server_address.sin_addr.S_un.S_un_b.s_b4,
				server_address.sin_port);
		}
		HasSentMessage = true;

		DisplayConnection(_data);
	}
	return true;
}

bool Client::ReceivePacket()
{
	if (is_running && HasSentMessage)
	{
		//Waiting for reply ;)
		flags = 0;
		from_size = sizeof(from);

		bytes_received = recvfrom(sock, buffer, IDENTIFY_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

		if (bytes_received == SOCKET_ERROR)
		{
			printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d on line: %d\n", WSAGetLastError(), __LINE__);
			return false;
		}

		ProcessPacket(buffer);

		//Grab data from packet
		int32 read_index = 0;
		char Temp[IDENTIFY_BUFFER_SIZE] = { '\0' };
		memcpy(&RecievedData, &Temp[read_index], sizeof(RecievedData));
		read_index += sizeof(RecievedData);
	}
	return true;
}

bool Client::ClientConsole(char * _Message)
{
	if (is_running)
	{
		if (strcmp(_Message, "shutdown") == 0)
		{
			SendPacket("Remove");
			StopClient();
			is_running = false;

			printf("\n\n");
			system("PAUSE");
			//exit(0);
			return true;
		}
		return false;
	}
}

void Client::DisplayConnection(const char * _data)
{
	if (strcmp(_data, "Join") == 0)
	{
		printf("Joined %d.%d.%d.%d:%d\n",
			server_address.sin_addr.S_un.S_un_b.s_b1,
			server_address.sin_addr.S_un.S_un_b.s_b2,
			server_address.sin_addr.S_un.S_un_b.s_b3,
			server_address.sin_addr.S_un.S_un_b.s_b4,
			server_address.sin_port);
		IsConnected = true;
	}
	if (strcmp(_data, "Disconnect") == 0)
	{
		printf("Disconnected from %d.%d.%d.%d:%d\n",
			server_address.sin_addr.S_un.S_un_b.s_b1,
			server_address.sin_addr.S_un.S_un_b.s_b2,
			server_address.sin_addr.S_un.S_un_b.s_b3,
			server_address.sin_addr.S_un.S_un_b.s_b4,
			server_address.sin_port);
		IsConnected = false;
	}
}

void Client::ProcessPacket(const char * _Data)//NEED TO FIX
{
	int Index = 0;
	char Command[IDENTIFY_BUFFER_SIZE] = {'\0'};
	int SizeOfCommand;

	memcpy(&SizeOfCommand, &_Data[Index], sizeof(int));
	Index += sizeof(int);

	memcpy(&Command, &_Data[Index], SizeOfCommand);
	Index += SizeOfCommand;

	if (strcmp(Command, "AmountOfPlayers") == 0)
	{
		int AmountOnSession = 0;
		memcpy(&AmountOnSession, &_Data[Index], AmountOnSession);
		Index += AmountOnSession;

		printf("Recieved");
	}

}
