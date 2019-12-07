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

#pragma region Visuals

	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "treelib - Client");

	SetTargetFPS(60);
	system("CLS");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);

		P.Update(&NewClient);

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			NewClient.SendPacket("Disconnect", 10, 0);
			NewClient.IsConnected = false;
		}
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
			int Temp = MeasureText("Press Space to join a session!", 20) / 2;
			RayDrawText("Press Space to join a session!", (GetScreenWidth() / 2) - Temp, GetScreenHeight() / 2, 20, LIGHTGRAY);
			RayDrawText(std::to_string(GetFrameTime() * 60 * 60).c_str(), (GetScreenWidth() / 2), 0, 20, LIGHTGRAY);
			if (IsKeyPressed(KEY_SPACE))
			{
				NewClient.SendPacket("Join");
				NewClient.IsConnected = true;
			}
		}
		EndDrawing();
	}
 
	RayCloseWindow(); 
#pragma endregion
	
	NewClient.CloseAllThreads();
	return 0;
}