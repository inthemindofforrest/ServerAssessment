#include "Session.h"
#pragma warning(disable:4996) 
Session::Session()
{
	for (int i = 0; i < MaxClientAmount; i++)
	{
		Clients[i].sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
		Clients[i].sin_port = htons(0);
	}
}

bool Session::AddClientToSession(SOCKADDR_IN _Address)
{
	for (int i = 0; i < MaxClientAmount; i++)
	{
		if (Clients[i].sin_port == 0)
		{
			Clients[i] = _Address;
			CurrentClientAmount++;
			return true;
		}
	}
	return false;
}

bool Session::RemoveClientFromSession(SOCKADDR_IN _Address)
{
	for (int i = 0; i < MaxClientAmount; i++)
	{
		if (Clients[i].sin_addr.S_un.S_un_b.s_b1 == _Address.sin_addr.S_un.S_un_b.s_b1 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b2 == _Address.sin_addr.S_un.S_un_b.s_b2 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b3 == _Address.sin_addr.S_un.S_un_b.s_b3 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b4 == _Address.sin_addr.S_un.S_un_b.s_b4 &&
			Clients[i].sin_port == _Address.sin_port)
		{
			Clients[i].sin_port = 0;
			CurrentClientAmount--;
			return true;
		}
	}
	return false;
}

bool Session::AvailableSpace()
{
	return CurrentClientAmount < MaxClientAmount;
}

bool Session::DisplayConnectedClients(int _SessionID)
{
	for (int i = 0; i < MaxClientAmount; i++)
	{
		if (Clients[i].sin_port != 0)
		{
			printf("Session(%d) Client(%d) IP: %d.%d.%d.%d:%d\n",_SessionID,i,
				Clients[i].sin_addr.S_un.S_un_b.s_b1,
				Clients[i].sin_addr.S_un.S_un_b.s_b2,
				Clients[i].sin_addr.S_un.S_un_b.s_b3,
				Clients[i].sin_addr.S_un.S_un_b.s_b4,
				Clients[i].sin_port);
		}
		if (i == MaxClientAmount - 1)
		{
			return true;
		}
	}
	return false;
}
