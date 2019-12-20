#pragma once
#include <WinSock2.h>
#include "raylib.h"	
#include <stdio.h>
#include <iostream>
#include <list>
#include <string>
#include <mutex>

struct Positions
{
public:
	SOCKADDR_IN Address;
	int Value[2]{0};
	int Color;
	int Score = 0;

	bool operator==(const Positions _Other);
	Positions();
	Positions(int _X, int _Y, int _Color);
};

class Session
{
private:
	static const int MaxClientAmount = 3;//Forever the max players allowed in the session

	void UpdateAstroids();
	void UpdateBullets();
	void UpdateClients();

public:
	Positions ClientPositions[MaxClientAmount];
	SOCKADDR_IN Clients[MaxClientAmount];
	int CurrentClientAmount = 0;


	std::list<Positions> Bullets;
	std::list<Positions> Astroids;
	float AstTimer = GetTime();
	std::mutex BulletLock;
	std::mutex AstroidLock;

	Session();

	void SessionUpdate();

	bool AddClientToSession(SOCKADDR_IN _Address);
	bool RemoveClientFromSession(SOCKADDR_IN _Address);
	bool AvailableSpace();
	bool DisplayConnectedClients(int _SessionID);

	bool CheckForClient(SOCKADDR_IN _Address);
	bool UpdatePosition(SOCKADDR_IN _Address, float _NewX, float _NewY);
	bool UpdateColor(SOCKADDR_IN _Address, int _Color);

	static bool MatchingSockAddress(SOCKADDR_IN _First, SOCKADDR_IN _Second);

	std::list<Positions> RetrieveClientPositions();
	SOCKADDR_IN ClientIP(int _ClientIndex);
	std::list<SOCKADDR_IN> AvailableClientIP();

	bool CreateBullet(std::string _Data, SOCKADDR_IN _Address);

	std::string ParsePacket(std::string* _Packet);
};