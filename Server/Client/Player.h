#pragma once
#include <WinSock2.h>
#include "raylib.h"
#include "ClientInit.h"

class Player
{
	RayRectangle Rect;

	float ClientPosition[2];
	int speed;

	float clamp(float _min, float _max, float _value);

public:
	

	void Update(Client* _Client);
	void Start();
	void Draw();

	void UpdatePosition(float _NewX, float _NewY);

	void DrawClients(Client* _Client);

	void SendClientPosition(Client* _Client);
};