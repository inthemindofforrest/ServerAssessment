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
	if (!(*_Client).IsConnected)
	{
		Selection.Update(PlayerPointer);
		
	}
	else
	{
		PlayerPointer->DrawAstroids(_Client);
	}
	
}

void PlayerServerInput(char * _IP, int* _Port)
{
	char TempHolder[IDENTIFY_BUFFER_SIZE];
	printf("IPv4: ");
	gets_s(_IP, IDENTIFY_BUFFER_SIZE);
	printf("Port: ");
	gets_s(TempHolder, IDENTIFY_BUFFER_SIZE);
	*_Port = std::stoi(TempHolder);
}

int main()
{
	char IPAdd[IDENTIFY_BUFFER_SIZE];
	int Port;

	ClientWindow NewWindow;
	Player P;
	Client NewClient;
	

	std::atomic<bool> Join = false;

	P.Start();
	PlayerPointer = &P;

	//PlayerServerInput(IPAdd, &Port);

	//NewClient.StartCustomClient(IPAdd, Port);
	NewClient.StartClient();
	NewWindow.InitClientWindow();
	
	Client_Thread = std::thread([&]
	{
		while (!Join)
		{
			NewClient.UpdateClient();
		}
	});
	
	ShowWindow(GetConsoleWindow(), SW_HIDE);// HIdes the console window

	NewWindow.Update(PlayerUpdate, &NewClient, PlayerPointer);

	NewClient.ForceSendPacket("Disconnect", 10, 0);
	NewClient.IsConnected = false;
	NewClient.CloseAllThreads();
	Join = true;
	if (Client_Thread.joinable())
	{
		//Client_Thread.join();
		Client_Thread.detach();
	}

	system("CLS");
	ShowWindow(GetConsoleWindow(), SW_SHOW);// Show the console window
	printf("Window forced to a close.\n");
	system("PAUSE");
	return 0;
}