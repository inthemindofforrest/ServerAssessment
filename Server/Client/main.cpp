#include "ClientInit.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include "Player.h"


std::thread Client_Thread;

int main()
{
	bool ShowWindowBool = false;
	ShowWindow(GetConsoleWindow(), SW_HIDE);// HIdes the console window

	Client NewClient;
	//NewClient.StartCustomClient("10.15.40.10", 9999);
	NewClient.StartClient();
	
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

		if (IsKeyPressed(KEY_SPACE)) NewClient.SendPacket("Disconnect", 10, 0);
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C))
		{
			if (ShowWindowBool)
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);// HIdes the console window
				ShowWindowBool = false;
			}
			else
			{
				system("CLS");
				ShowWindow(GetConsoleWindow(), SW_SHOW);// HIdes the console window
				ShowWindowBool = true;
				printf("Console Visibility = true\n");
			}
		}

		if (!NewClient.IsConnected)
		{
			ClearBackground(DARKGRAY);
			int Temp = MeasureText("Press 1,2,3,4,5 to join a session!", 20) / 2;
			RayDrawText("Press 1,2,3,4,5 to join a session!", (GetScreenWidth() / 2) - Temp, GetScreenHeight() / 2, 20, LIGHTGRAY);

			if (IsKeyPressed(KEY_ONE))	NewClient.SendPacket("Join", 4, 0);
			if (IsKeyPressed(KEY_TWO))	NewClient.SendPacket("Join", 4, 1);
			if (IsKeyPressed(KEY_THREE))NewClient.SendPacket("Join", 4, 2);
			if (IsKeyPressed(KEY_FOUR))	NewClient.SendPacket("Join", 4, 3);
			if (IsKeyPressed(KEY_FIVE))	NewClient.SendPacket("Join", 4, 4);
		}
		EndDrawing();
	}
 
	RayCloseWindow();        
	
	return 0;
}