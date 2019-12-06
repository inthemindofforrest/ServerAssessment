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
			ClientPositions[i].Address = _Address;
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

bool Session::CheckForClient(SOCKADDR_IN _Address)
{
	for (int i = 0; i < CurrentClientAmount; i++)
	{
		if (Clients[i].sin_addr.S_un.S_un_b.s_b1 == _Address.sin_addr.S_un.S_un_b.s_b1 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b2 == _Address.sin_addr.S_un.S_un_b.s_b2 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b3 == _Address.sin_addr.S_un.S_un_b.s_b3 &&
			Clients[i].sin_addr.S_un.S_un_b.s_b4 == _Address.sin_addr.S_un.S_un_b.s_b4 &&
			Clients[i].sin_port == _Address.sin_port)
		{
			return true;
			//Client Already Exists
		}
	}
	return false;
}

bool Session::UpdatePosition(SOCKADDR_IN _Address, float _NewX, float _NewY)
{
	for (int i = 0; i < CurrentClientAmount; i++)
	{
		if (ClientPositions[i].Address.sin_addr.S_un.S_un_b.s_b1 == _Address.sin_addr.S_un.S_un_b.s_b1 &&
			ClientPositions[i].Address.sin_addr.S_un.S_un_b.s_b2 == _Address.sin_addr.S_un.S_un_b.s_b2 &&
			ClientPositions[i].Address.sin_addr.S_un.S_un_b.s_b3 == _Address.sin_addr.S_un.S_un_b.s_b3 &&
			ClientPositions[i].Address.sin_addr.S_un.S_un_b.s_b4 == _Address.sin_addr.S_un.S_un_b.s_b4 &&
			ClientPositions[i].Address.sin_port == _Address.sin_port)
		{
			//Update the clients position server side
			ClientPositions[i].Value[0] = _NewX;
			ClientPositions[i].Value[1] = _NewY;
			return true;
		}
	}
	return false;
}

bool Session::MatchingSockAddress(SOCKADDR_IN _First, SOCKADDR_IN _Second)
{
	return(_First.sin_addr.S_un.S_un_b.s_b1 == _Second.sin_addr.S_un.S_un_b.s_b1 &&
		_First.sin_addr.S_un.S_un_b.s_b2 == _Second.sin_addr.S_un.S_un_b.s_b2 &&
		_First.sin_addr.S_un.S_un_b.s_b3 == _Second.sin_addr.S_un.S_un_b.s_b3 &&
		_First.sin_addr.S_un.S_un_b.s_b4 == _Second.sin_addr.S_un.S_un_b.s_b4 &&
		_First.sin_port == _Second.sin_port);
}

std::list<Positions> Session::RetrieveClientPositions()
{
	std::list<Positions> ClientPos;
	for (int i = 0; i < MaxClientAmount; i++)
	{
		if (Clients[i].sin_port == 0)
			continue;
		ClientPos.push_back(ClientPositions[i]);
	}
	return ClientPos;
}

SOCKADDR_IN Session::ClientIP(int _ClientIndex)
{
	return Clients[_ClientIndex];
}

std::list<SOCKADDR_IN> Session::AvailableClientIP()
{
	std::list<SOCKADDR_IN> TempList;

	for (int i = 0; i < MaxClientAmount; i++)
	{
		SOCKADDR_IN TempAdd = ClientIP(i);
		if (TempAdd.sin_port == 0)//If Port == 0, Session space is empty
			continue;

		TempList.push_back(TempAdd);
	}
	return TempList;
}
