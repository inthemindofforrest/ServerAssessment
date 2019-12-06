#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <list>

struct Positions
{
public:
	SOCKADDR_IN Address;
	int Value[2]{0};
};

class Session
{
private:
	static const int MaxClientAmount = 3;//Forever the max players allowed in the session

	

public:
	Positions ClientPositions[MaxClientAmount];
	SOCKADDR_IN Clients[MaxClientAmount];
	int CurrentClientAmount = 0;

	Session();

	bool AddClientToSession(SOCKADDR_IN _Address);
	bool RemoveClientFromSession(SOCKADDR_IN _Address);
	bool AvailableSpace();
	bool DisplayConnectedClients(int _SessionID);

	bool CheckForClient(SOCKADDR_IN _Address);
	bool UpdatePosition(SOCKADDR_IN _Address, float _NewX, float _NewY);

	static bool MatchingSockAddress(SOCKADDR_IN _First, SOCKADDR_IN _Second);

	std::list<Positions> RetrieveClientPositions();
	SOCKADDR_IN ClientIP(int _ClientIndex);
	std::list<SOCKADDR_IN> AvailableClientIP();
};