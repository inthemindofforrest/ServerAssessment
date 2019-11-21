#include "ClientInit.h"
int main()
{
	Client NewClient;
	NewClient.StartClient();

	//std::this_thread::sleep_for(std::chrono::seconds(1));

	while (true)
	{
		NewClient.UpdateClient();
	}
	return 0;
}