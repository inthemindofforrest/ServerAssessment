#pragma once
#include "Player.h"
#include "raylib.h"

class ClientSelection
{
	enum Colors { MBLUE, MRED, MGREEN };
	int MyColor;
	int Selected;

	int Min;
	int Max;

	double Timer;

public:

	ClientSelection();
	void Update(Player* _Client);
	int UpdateSelected(int _Value);
};