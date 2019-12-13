#pragma once
#include <WinSock2.h>
#include "raylib.h"
#include "ClientInit.h"

class Player
{
	RayRectangle Rect;

	
	int speed;
	bool ShowWindowBool = false;

	float clamp(float _min, float _max, float _value);
	int MyColor;

public:
	float ClientPosition[2];
	
	Player();
	//void Update(Client* _Client);
	void Update(Client* _Client, Player* _Player);
	void Start();
	void Draw();
	void ColorDraw();

	void UpdatePosition(float _NewX, float _NewY);

	void DrawClients(Client* _Client);

	void SendClientPosition(Client* _Client);

	void DisconnectFromServer(Client* _Client);
	void ConsoleDisplay();
	void ManageConnectionStatus(Client* _Client);
};