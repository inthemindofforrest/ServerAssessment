#include "ClientInit.h"

int main()
{
	Client NewClient;
	NewClient.StartClient();


	//std::this_thread::sleep_for(std::chrono::seconds(2));

	while (NewClient.is_running)
	{
		NewClient.UpdateClient();
	}	
	return 0;
}