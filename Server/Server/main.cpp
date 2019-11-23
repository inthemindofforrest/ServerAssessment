#include "ServerInit.h"
#include "Session.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include<thread>



std::thread Server_Thread;


int main()
{
	Server NewServer;
	NewServer.StartServer();

	Server_Thread = std::thread([&]
	{
		while (true)
		{
			NewServer.ServerUpdate();
		}
	});


	int screenWidth = 250;
	int screenHeight = 200;

	InitWindow(screenWidth, screenHeight, "treelib - Server");

	SetTargetFPS(60);
	system("CLS");

	while (!WindowShouldClose())    
	{
		BeginDrawing();

		ClearBackground(BLACK);

		std::string Temp;
		for (int i = 0; i < 5; i++)
		{
			Temp.append("Session(" + std::to_string(i) + "): " + std::to_string(NewServer.GetSessionCount()[i]) + "\n");
		}


		std::string Text = ("Server: \n" + Temp);

		RayDrawText(Text.c_str(), 0, 0, 20, LIGHTGRAY);

		EndDrawing();
	}

	RayCloseWindow();

	return 0;
}