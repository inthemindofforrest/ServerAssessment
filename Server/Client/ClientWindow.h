#pragma once
#include "raylib.h"
#include "ClientInit.h"
#include "Player.h"

using Tup = void(*)();
using ClientTup = void(*)(Client*, Player*);

class ClientWindow
{
	int ScreenWidth;
	int ScreenHeight;

public:
	void InitClientWindow();
	void Update(Tup _Function);
	void Update(ClientTup _Function, Client* _Client, Player* _Player);
};