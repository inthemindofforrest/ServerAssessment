#include "ServerInit.h"
#include <cstdlib>
#include <string.h>

void Server::Shutdown()
{
	if (shutdown(sock, SD_BOTH) == SOCKET_ERROR)
	{
		printf("Shutdown failed: %d on line: %d\n", WSAGetLastError(), __LINE__);
		closesocket(sock);
		WSACleanup();
		return;
	}
	printf("Shutdown Server\n");
	is_running = false;


	printf("\n\n");
	system("PAUSE");
	exit(0);
}

Server::Server()
{
	winsock_version = 0x202;//Which version of Winsock we are using
	winsock_data;//Struct of data

	is_running = true;

	Console_Thread = std::thread([&]
	{
		while (is_running)
		{
			ServerConsole();
		}
	});
}

void Server::StartServer()
{
	WSAStart();
	CreateSocket();
	SetSocketAddress();

}

void Server::StartServer(char * _Address, char* _Port)
{
	WSAStart();
	CreateSocket();
	SetSocketAddress(_Address, atoi(_Port));
}

void Server::StartConfigServer()
{
	char IP[IDENTIFY_BUFFER_SIZE];
	printf("IPv4: ");
	gets_s(IP, IDENTIFY_BUFFER_SIZE);
	char Port[IDENTIFY_BUFFER_SIZE];
	printf("Port: ");
	gets_s(Port, IDENTIFY_BUFFER_SIZE);
	StartServer(IP, Port);
}

void Server::ServerUpdate()
{
	if (is_running)
		RecievedPacket();
}

bool Server::WSAStart()
{
	if (WSAStartup(winsock_version, &winsock_data))
	{
		printf("WSAStartup failed: %d", WSAGetLastError());
		return false;
	}
	else
	{
		printf("Starting WSA\n");
	}
	return true;
}

bool Server::CreateSocket()
{
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)//If INVALID_SOCKET then it has failed
	{
		printf("socket failed: %d", WSAGetLastError());
		return false;
	}
	else
	{
		printf("Opening Socket\n");
	}
	return true;
}

bool Server::CreateSocket(int _AddressFamily, int _Type, int _Protocol)
{
	sock = socket(_AddressFamily, _Type, _Protocol);
	if (sock == INVALID_SOCKET)//If INVALID_SOCKET then it has failed
	{
		printf("socket failed: %d", WSAGetLastError());
		return false;
	}
	else
	{
		printf("Opening Socket\n");
	}
	return true;
}

bool Server::SetSocketAddress()
{
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9999);//Packet needs to be bound to a port. Ports below 1024 are reserved. Chose 9999 - ... just a high number 
	local_address.sin_addr.s_addr = INADDR_ANY;//inet_addr("10.15.20.13");

	printf("Binding Socket to 10.15.20.13:%d\n", local_address.sin_port);

	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return false;
	}
	else
	{
		printf("Server Finished Init!\n");
		printf("Server IP: %d.%d.%d.%d:%d\n",
			local_address.sin_addr.S_un.S_un_b.s_b1,
			local_address.sin_addr.S_un.S_un_b.s_b2,
			local_address.sin_addr.S_un.S_un_b.s_b3,
			local_address.sin_addr.S_un.S_un_b.s_b4,
			local_address.sin_port);
	}
	return true;
}

bool Server::SetSocketAddress(char* _Address, int _Port)
{
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(_Port);//Packet needs to be bound to a port. Ports below 1024 are reserved. Chose 9999 - ... just a high number 
	local_address.sin_addr.s_addr = inet_addr(_Address);

	printf("Binding Socket to %s:%d\n",_Address, _Port);

	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return false;
	}
	else
	{
		printf("Server Finished Init!\n");
		printf("Server IP: %d.%d.%d.%d:%d\n",
			local_address.sin_addr.S_un.S_un_b.s_b1,
			local_address.sin_addr.S_un.S_un_b.s_b2,
			local_address.sin_addr.S_un.S_un_b.s_b3,
			local_address.sin_addr.S_un.S_un_b.s_b4,
			local_address.sin_port);
	}
	return true;
}

bool Server::RecievedPacket()
{
	from_size = sizeof(from);
	bytes_recieved = recvfrom(sock, buffer, IDENTIFY_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_recieved == SOCKET_ERROR)
	{
		printf("Recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		return false;
	}

	ProccessPacket();

	return true;
}

void Server::ProccessPacket()
{
	//Process of the PACKETS
	int32 read_index = 0;

	char Command[IDENTIFY_BUFFER_SIZE] = {'\0'};
	int Size;
	int SessionID;

	memcpy(&SessionID, &buffer[read_index], sizeof(SessionID));
	read_index += sizeof(SessionID);
	
	memcpy(&Size, &buffer[read_index], sizeof(Size));
	read_index += sizeof(Size);

	memcpy(&Command, &buffer[read_index], Size);
	read_index += Size;

	

	if (strcmp(Command, "Join") == 0)
	{
		printf("Player requested to Join Session: %d\n", SessionID);
		CheckForSession(from, SessionID);
	}
 	else if (strcmp(Command, "Disconnect") == 0)
	{
		DisconnectFromSessions(from);
	}
}

void Server::CreatePacket()
{
	//Create return Packet
	strcpy(SendingData, buffer);
	int32 write_index = 0;
	memcpy(&buffer[write_index], &SendingData, sizeof(SendingData));
	//write_index += sizeof(buffer);

	//memcpy(&buffer[write_index], &is_running, sizeof(is_running));
}

bool Server::SendPacket()
{
	//Send packet to Client
	int buffer_length = sizeof(SendingData);
	flags = 0;
	SOCKADDR* to = (SOCKADDR*)&from;
	int to_length = sizeof(from);

	if (sendto(sock, buffer, buffer_length, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool Server::SendPacket(const char* _Message, int _Size)
{
	//Send packet to Client
	int buffer_length = sizeof(SendingData);
	flags = 0;
	SOCKADDR* to = (SOCKADDR*)&from;
	int to_length = sizeof(from);

	if (sendto(sock, _Message, _Size, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		return false;
	}
	return true;
}
bool Server::SendPacket(const char* _Message, int _Size, SOCKADDR_IN _Return)
{
	//Send packet to Client
	int buffer_length = sizeof(SendingData);
	flags = 0;
	SOCKADDR* to = (SOCKADDR*)&_Return;
	int to_length = sizeof(from);

	if (sendto(sock, _Message, _Size, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		return false;
	}
	return true;
}

void Server::SendClientsInfo()
{
	int Size = sizeof(Sessions) / sizeof(Session);
	for (int i = 0; i < Size; i++)//Sessions
	{
		for (int j = 0; j < Sessions[i].CurrentClientAmount; j++)//Each player
		{
			char Message[IDENTIFY_BUFFER_SIZE];
			int Index = 0;
			int CommandLength = 15;

			memcpy(&Message[Index], &CommandLength, sizeof(CommandLength));
			Index += sizeof(CommandLength);

			memcpy(&Message[Index], "AmountOfPlayers", sizeof("AmountOfPlayers"));
			Index += sizeof("AmountOfPlayers");

			memcpy(&Message[Index], std::to_string(Sessions[i].CurrentClientAmount).c_str(), sizeof(std::to_string(Sessions[i].CurrentClientAmount).c_str()));
			Index += sizeof(std::to_string(Sessions[i].CurrentClientAmount).c_str());

			SendPacket(Message,Index, Sessions[i].Clients[j]);
			//Send Each player amount of people are in session
		}
	}
}

void Server::CheckForSession(SOCKADDR_IN _Address)
{

	CreatePacket();
	SendPacket();

	for (int i = 0; i < SessionsAmount; i++)
	{
		if (Sessions[i].CheckForClient(_Address))
		{
			return;
		}
	}

	for (int i = 0; i < SessionsAmount; i++)
	{
		if (Sessions[i].AvailableSpace())//If there is space in the Session
		{
			//Place the use into that session
			Sessions[i].AddClientToSession(_Address);
			printf("Client IP: %d.%d.%d.%d:%d joined the session\n",
				_Address.sin_addr.S_un.S_un_b.s_b1,
				_Address.sin_addr.S_un.S_un_b.s_b2,
				_Address.sin_addr.S_un.S_un_b.s_b3,
				_Address.sin_addr.S_un.S_un_b.s_b4,
				_Address.sin_port);

			return;
		}
	}
}

void Server::CheckForSession(SOCKADDR_IN _Address, int _SessionIndex)
{
	//CreatePacket();
	//SendPacket();

	for (int i = 0; i < SessionsAmount; i++)
	{
		if (Sessions[i].CheckForClient(_Address))
		{
			return;
		}
	}

	if (Sessions[_SessionIndex].AvailableSpace())//If there is space in the Session
	{
		//Place the use into that session
		Sessions[_SessionIndex].AddClientToSession(_Address);
		printf("Client IP: %d.%d.%d.%d:%d joined the session\n",
			_Address.sin_addr.S_un.S_un_b.s_b1,
			_Address.sin_addr.S_un.S_un_b.s_b2,
			_Address.sin_addr.S_un.S_un_b.s_b3,
			_Address.sin_addr.S_un.S_un_b.s_b4,
			_Address.sin_port);

		return;
	}
}

int* Server::GetSessionCount()
{
	int Total[5];

	for (int i = 0; i < 5; i++)
	{
		Total[i] = Sessions[i].CurrentClientAmount;
	}

	return Total;
}

void Server::DisconnectFromSessions(SOCKADDR_IN _Address)
{
	int Size = sizeof(Sessions) / sizeof(Session);
	for (int i = 0; i < Size; i++)
	{
		if (Sessions[i].RemoveClientFromSession(_Address))
		{
			printf("%d.%d.%d.%d:%d disconnected from Session(%d)\n",
				_Address.sin_addr.S_un.S_un_b.s_b1, _Address.sin_addr.S_un.S_un_b.s_b2,
				_Address.sin_addr.S_un.S_un_b.s_b3, _Address.sin_addr.S_un.S_un_b.s_b4,
				_Address.sin_port, i);
			//SendPacket("Disconnect", 10);
			return;
		}
	}
	printf("Client not connected to Server...\n");
}

void Server::ServerConsole()
{
	const int AmountOfCommands = 2;
	const int LengthOfCommands = 10;
	char Commands[AmountOfCommands][LengthOfCommands] = {"session", "\\help"};

	char Message[IDENTIFY_BUFFER_SIZE];
	gets_s(Message, IDENTIFY_BUFFER_SIZE);
	if (is_running)
	{
		if (strcmp(Message, "session") == 0)
		{
			for (int i = 0; i < SessionsAmount; i++)
			{
				Sessions[i].DisplayConnectedClients(i);
			}
			if (SessionsAmount == 0)
			{
				printf("No Sessions Opened\n");
			}
		}
		else if (strcmp(Message, "shutdown") == 0)
		{
			Shutdown();
		}
		else if(strcmp(Message, "Send") == 0)
		{
			SendClientsInfo();
		}
		else
		{
			printf("Command: \"%s\" not found.\n", Message);
		}
	}
}

bool Server::MatchingSockAddress(SOCKADDR_IN _First, SOCKADDR_IN _Second)
{
	return(_First.sin_addr.S_un.S_un_b.s_b1 == _Second.sin_addr.S_un.S_un_b.s_b1 &&
		_First.sin_addr.S_un.S_un_b.s_b2 == _Second.sin_addr.S_un.S_un_b.s_b2 &&
		_First.sin_addr.S_un.S_un_b.s_b3 == _Second.sin_addr.S_un.S_un_b.s_b3 &&
		_First.sin_addr.S_un.S_un_b.s_b4 == _Second.sin_addr.S_un.S_un_b.s_b4 &&
		_First.sin_port == _Second.sin_port);
}

bool Server::ResetSockAddress(SOCKADDR_IN * _Address)
{
	(*_Address).sin_addr.S_un.S_un_b.s_b1 = 0;
	(*_Address).sin_addr.S_un.S_un_b.s_b2 = 0;
	(*_Address).sin_addr.S_un.S_un_b.s_b3 = 0;
	(*_Address).sin_addr.S_un.S_un_b.s_b4 = 0;
	(*_Address).sin_port = 0;
	return false;
}



