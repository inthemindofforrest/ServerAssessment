#include "ServerInit.h"
#include "Session.h"

#include<thread>



int main()
{
	Server NewServer;
	NewServer.StartServer();

	while (NewServer.is_running)
	{
		NewServer.ServerUpdate();
		NewServer.RecievedPacket();
	}

	return 0;
}