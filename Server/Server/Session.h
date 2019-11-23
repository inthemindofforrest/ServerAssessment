#pragma once
#include <WinSock2.h>
#include <stdio.h>

class Session
{
private:
	static const int MaxClientAmount = 3;//Forever the max players allowed in the session
	
	SOCKADDR_IN Clients[MaxClientAmount];

public:
	int CurrentClientAmount = 0;

	Session();

	bool AddClientToSession(SOCKADDR_IN _Address);
	bool RemoveClientFromSession(SOCKADDR_IN _Address);
	bool AvailableSpace();
	bool DisplayConnectedClients(int _SessionID);

	bool CheckForClient(SOCKADDR_IN _Address);
};