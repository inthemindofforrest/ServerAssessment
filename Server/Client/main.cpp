#include "ClientInit.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include "Player.h"


std::thread Client_Thread;

int main()
{
	Client NewClient;
	NewClient.StartCustomClient("10.15.40.10", 9999);
	//NewClient.StartClient();
	
	Client_Thread = std::thread([&]
	{
		while (true)
		{
			NewClient.UpdateClient();
		}
	});
	
	Player P;
	P.Start();

	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "treelib - Client");

	SetTargetFPS(60);
	system("CLS");

	while (!WindowShouldClose())    
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);

		P.Update();

		RayDrawText("Press 1,2,3,4,5 to join a session!", 0, 0, 20, LIGHTGRAY);

		if (IsKeyPressed(KEY_ONE))	NewClient.SendPacket("Join", 4, 0);
		if (IsKeyPressed(KEY_TWO))	NewClient.SendPacket("Join", 4, 1);
		if (IsKeyPressed(KEY_THREE))	NewClient.SendPacket("Join", 4, 2);
		if (IsKeyPressed(KEY_FOUR))	NewClient.SendPacket("Join", 4, 3);
		if (IsKeyPressed(KEY_FIVE))	NewClient.SendPacket("Join", 4, 4);

		if (IsKeyPressed(KEY_SPACE))	
			NewClient.SendPacket("Disconnect", 10, 0);

		EndDrawing();
	}
 
	RayCloseWindow();        
	
	return 0;
}