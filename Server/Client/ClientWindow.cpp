#include "ClientWindow.h"

void ClientWindow::InitClientWindow()
{
	ScreenWidth = 800;
	ScreenHeight = 450;

	InitWindow(ScreenWidth, ScreenHeight, "treelib - Client");
	SetTargetFPS(60);
}

void ClientWindow::Update(Tup _Function)
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		_Function();//Functions that should run while the drawing is in effect
		EndDrawing();
	}
}

void ClientWindow::Update(ClientTup _Function, Client* _Client, Player* _Player)
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		_Function(_Client, _Player);//Functions that should run while the drawing is in effect
		EndDrawing();
	}
	RayCloseWindow();
	(*_Client).is_running = false;
}
