#pragma once
#include <WinSock2.h>
#include "raylib.h"
#include "ClientInit.h"

struct Star
{
	Positions Pos;
	float Speed;
	float Size;

	Star();
	Star(int _X, int _Y, float _Speed, float _Size);
};


class Player
{
	RayRectangle Rect;

	
	int speed;
	bool ShowWindowBool = false;

	float clamp(float _min, float _max, float _value);
	double ShootTimer;
	float ShootDelay = .5f;

public:
	int Size[2];
	int MyColor;
	float ClientPosition[2];
	std::list<Star> Stars;
	
	Player();
	//void Update(Client* _Client);
	void Update(Client* _Client, Player* _Player);
	void Start();
	void Draw();

	void UpdatePosition(float _NewX, float _NewY);

	void DrawClients(Client* _Client);
	void DrawBullets(Client* _Client);

	void SendClientPosition(Client* _Client);

	void DisconnectFromServer(Client* _Client);
	void ConsoleDisplay();
	void ManageConnectionStatus(Client* _Client);

	void UpdateStars();
};