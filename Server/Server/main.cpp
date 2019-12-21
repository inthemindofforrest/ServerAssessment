#include "ServerInit.h"
#include "Session.h"
#include "ThreadManager.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include <thread>
#include <stdio.h>
#include <iostream>
#include <list>


std::thread Server_Thread;

Server* ServerPointer;
void ServerUpdate()
{
	ServerPointer->ServerUpdate();
}

void ServerInput(char * _IP)
{
	char TempHolder[IDENTIFY_BUFFER_SIZE];
	printf("IPv4: ");
	gets_s(_IP, IDENTIFY_BUFFER_SIZE);
}

void ServerHUD(Server* _Server, char* _IP)
{
	std::string Temp;
	for (int i = 0; i < 5; i++)
	{
		Temp.append("Session(" + std::to_string(i) + "): " + std::to_string((*_Server).GetSessionCount()[i]) + " Scores: "
			+ std::to_string((*_Server).Sessions[i].ClientPositions[0].Score) + ", " + std::to_string((*_Server).Sessions[i].ClientPositions[1].Score) + ", "
			+ std::to_string((*_Server).Sessions[i].ClientPositions[2].Score) + ", " + "\n");
	}
	std::string Text = ("Server: \n" + Temp);
	RayDrawText(Text.c_str(), 0, GetScreenHeight()/2, 20, LIGHTGRAY);

	Temp.clear();
	Temp = "Astroids: " + std::to_string((*_Server).Sessions[0].Astroids.size());
	Text.clear();
	Text = ("Astroids(0): \n" + Temp);
	RayDrawText(Text.c_str(), 650, 0, 20, BLUE);

	Temp.clear();
	Temp = "Bullets: " + std::to_string((*_Server).Sessions[0].Bullets.size());
	Text.clear();
	Text = ("Bullets(0): \n" + Temp);
	RayDrawText(Text.c_str(), 650, 75, 20, RED);


	std::string Connection = _IP;
	Connection.append(":9999");
	RayDrawText(Connection.c_str(), (GetScreenWidth() / 2) - MeasureText(Connection.c_str(), 50) / 2, 0, 50, LIGHTGRAY);
}

int main()
{
	bool ShowWindowBool = false;

	char IP[IDENTIFY_BUFFER_SIZE];
	ServerInput(IP);
	char Port[IDENTIFY_BUFFER_SIZE] = "9999";

	ShowWindow(GetConsoleWindow(), SW_HIDE);//Hides the console window

	Server NewServer;
	NewServer.StartServer(IP, Port);

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

	//float AstroidTimer = GetTime();

	while (!WindowShouldClose())    
	{
		BeginDrawing();

		ClearBackground(BLACK);

		for (int j = 0; j < NewServer.SessionsAmount; j++)
		{
			NewServer.Sessions[j].SessionUpdate();
		}

		ServerHUD(&NewServer, IP);


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

		for (int j = 0; j < NewServer.SessionsAmount; j++)
		{

			if (NewServer.Sessions[j].CurrentClientAmount > 0 && GetTime() - NewServer.Sessions[j].AstTimer > 1)
			{
				for (int k = 0; k < NewServer.Sessions[j].CurrentClientAmount; k++)
				{
					NewServer.Sessions[j].Astroids.push_front(Positions(GetScreenWidth() + 50, rand() % (GetScreenHeight() - 40 - 80 + 1) + 80, 0));
					NewServer.Sessions[j].AstTimer = GetTime();
				}
			}
			else if (NewServer.Sessions[j].CurrentClientAmount == 0)
			{
				NewServer.Sessions[j].Astroids.clear();
			}
		}

		EndDrawing();
	}

	RayCloseWindow();
	#pragma endregion
	NewServer.CloseAllThreads();
	return 0;
}
