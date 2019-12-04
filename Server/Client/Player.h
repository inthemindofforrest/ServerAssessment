#pragma once
#include <WinSock2.h>
#include "raylib.h"

class Player
{
	RayRectangle Rect;

	float Position[2];
	int speed;

	float clamp(float _min, float _max, float _value);

public:

	void Update();
	void Start();
	void Draw();

	void UpdatePosition(float _NewX, float _NewY);

	void SendPosition();

};