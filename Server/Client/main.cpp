#include "ClientInit.h"
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include "Player.h"
#include "ClientWindow.h"
#include "Selection.h"


std::thread Client_Thread;

Player* PlayerPointer;
ClientSelection Selection;
void PlayerUpdate(Client* _Client, Player* _Player)
{
	PlayerPointer->Update(_Client, _Player);
	Selection.Update(PlayerPointer);
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);// HIdes the console window

	ClientWindow NewWindow;
	Player P;
	Client NewClient;
	

	std::atomic<bool> Join = false;

	P.Start();
	PlayerPointer = &P;
	NewClient.StartClient();
	NewWindow.InitClientWindow();
	
	Client_Thread = std::thread([&]
	{
		while (!Join)
		{
			NewClient.UpdateClient();
		}
	});

	NewWindow.Update(PlayerUpdate, &NewClient, PlayerPointer);
 
	NewClient.CloseAllThreads();
	Join = true;
	if (Client_Thread.joinable())
	{
		//Client_Thread.join();
		Client_Thread.detach();
	}
	ShowWindow(GetConsoleWindow(), SW_SHOW);// Show the console window

	NewClient.SendPacket("Disconnect", 10, 0);
	NewClient.IsConnected = false;

	return 0;
}