#pragma once
#include <WinSock2.h>
#include <stdio.h>

class Session
{
private:
	static const int MaxClientAmount = 3;//Forever the max players allowed in the session
	int CurrentClientAmount = 0;
	SOCKADDR_IN Clients[MaxClientAmount];

public:
	Session();

	bool AddClientToSession(SOCKADDR_IN _Address);
	bool RemoveClientFromSession(SOCKADDR_IN _Address);
	bool AvailableSpace();
	bool DisplayConnectedClients(int _SessionID);
};