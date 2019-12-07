#include "ServerInit.h"
#include "Session.h"
#include "ThreadManager.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include<thread>
#include <stdio.h>
#include <iostream>
#include <list>


std::thread Server_Thread;

Server* ServerPointer;
void ServerUpdate()
{
	ServerPointer->ServerUpdate();
}


int main()
{
	bool ShowWindowBool = false;
	ShowWindow(GetConsoleWindow(), SW_HIDE);//Hides the console window

	Server NewServer;
	NewServer.StartServer();

	ServerPointer = &NewServer;
	RunThreadWithLoop(&Server_Thread, ServerUpdate);


#pragma region Visuals



	//int screenWidth = 250;
	//int screenHeight = 200;
	int screenWidth = 800;
	int screenHeight = 450;

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
		std::list<Positions> Pos = NewServer.Sessions[0].RetrieveClientPositions();
		for (std::list<Positions>::iterator i = Pos.begin();
			i != Pos.end(); i++)
		{
			DrawRectangle((*i).Value[0], (*i).Value[1], 50, 50, RED);
		}
		EndDrawing();
	}

	RayCloseWindow();
#pragma endregion
	NewServer.CloseAllThreads();
	return 0;
}
