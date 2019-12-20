#include "Session.h"
#pragma warning(disable:4996) 


Positions::Positions(){}
Positions::Positions(int _X, int _Y, int _Color)
{
	Value[0] = _X;
	Value[1] = _Y;
	Color = _Color;
}
bool Positions::operator==(const Positions _Other)
{
	return
		(Address.sin_addr.S_un.S_un_b.s_b1 == _Other.Address.sin_addr.S_un.S_un_b.s_b1 &&
			Address.sin_addr.S_un.S_un_b.s_b2 == _Other.Address.sin_addr.S_un.S_un_b.s_b2 &&
			Address.sin_addr.S_un.S_un_b.s_b3 == _Other.Address.sin_addr.S_un.S_un_b.s_b3 &&
			Address.sin_addr.S_un.S_un_b.s_b4 == _Other.Address.sin_addr.S_un.S_un_b.s_b4 &&
			Address.sin_port == _Other.Address.sin_port) &&
		(Color == _Other.Color) &&
		(Value[0] == _Other.Value[0] && Value[1] == _Other.Value[1]);
}


//PRIVATE:
void Session::UpdateAstroids()
{
	for (std::list<Positions>::iterator i = Astroids.begin();
		i != Astroids.end();)
	{
		if ((*i).Value[0] < 0)
		{
			i = Astroids.erase(i);
			continue;
		}
		i->Value[0]--;
		for (std::list<Positions>::iterator j = Bullets.begin();
			j != Bullets.end();)
		{
			if (CheckCollisionRecs(RayRectangle(i->Value[0], i->Value[1], 50, 50),
				RayRectangle(j->Value[0], j->Value[1], 20, 5)))
			{
				for (int i = 0; i < MaxClientAmount; i++)
				{
					if (MatchingSockAddress(Clients[i], (*j).Address))
					{
						ClientPositions[i].Score++;
					}
				}

				i = Astroids.erase(i);
				j = Bullets.erase(j);
				goto JumpPoint;
			}
			j++;
		}
		for (int j = 0; j < MaxClientAmount; j++)
		{
			if (CheckCollisionRecs(RayRectangle(i->Value[0], i->Value[1], 50, 50),
				RayRectangle(ClientPositions[j].Value[0], ClientPositions[j].Value[1], 50, 20)))
			{
				for (int i = 0; i < MaxClientAmount; i++)
				{
					if (MatchingSockAddress(Clients[i], ClientPositions[j].Address))
					{
						ClientPositions[i].Score = 0;
					}
				}

				i = Astroids.erase(i);
				goto JumpPoint;
			}
		}

		Color AstColor;
		AstColor.r = 100;
		AstColor.b = 100;
		AstColor.g = 100;
		AstColor.a = 50;

		DrawRectangle(i->Value[0], i->Value[1], 50, 50, AstColor);
		i++;

	JumpPoint: NULL;
	}
}

void Session::UpdateBullets()
{
	for (std::list<Positions>::iterator i = Bullets.begin();
		i != Bullets.end();)
	{
		DrawRectangle((*i).Value[0], (*i).Value[1], 20, 5, GetColor((*i).Color));
		(*i).Value[0] += 3;
		if ((*i).Value[0] > GetScreenWidth() - 10)
		{
			i = Bullets.erase(i);
		}
		else { i++; }
	}
}

void Session::UpdateClients()
{
	std::list<Positions> Pos = RetrieveClientPositions();
	for (std::list<Positions>::iterator i = Pos.begin();
		i != Pos.end(); i++)
	{
		DrawRectangle((*i).Value[0], (*i).Value[1], 40, 15, GetColor((*i).Color));
	}
}

//PUBLIC:
Session::Session()
{
	for (int i = 0; i < MaxClientAmount; i++)
	{
		Clients[i].sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
		Clients[i].sin_port = htons(0);
		ClientPositions->Score = 0;
	}
}

void Session::SessionUpdate()
{
	UpdateAstroids();
	UpdateBullets();
	UpdateClients();
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
			ClientPositions[i].Score = 0;
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

bool Session::UpdateColor(SOCKADDR_IN _Address, int _Color)
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
			ClientPositions[i].Color = _Color;
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

bool Session::CreateBullet(std::string _Data, SOCKADDR_IN _Address)
{
	Positions Bullet;
	Bullet.Value[0] = std::stoi(ParsePacket(&_Data));
	Bullet.Value[1] = std::stoi(ParsePacket(&_Data));
	Bullet.Color = std::stoi(ParsePacket(&_Data));
	Bullet.Address = _Address;
	Bullets.push_back(Bullet);

	return true;
}

std::string Session::ParsePacket(std::string* _Packet)
{
	std::string Parsed;

	while ((*_Packet)[0] != ',' && (*_Packet)[0] != ';' && (*_Packet)[0] != '\0')
	{
		Parsed += (*_Packet)[0];
		(*_Packet).erase(0, 1);
	}

	if ((*_Packet)[0] == ',' || (*_Packet)[0] == ';')
		(*_Packet).erase(0, 1);

	return Parsed;
}
