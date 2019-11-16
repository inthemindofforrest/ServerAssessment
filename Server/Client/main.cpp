#include "ClientInit.h"
int main()
{
	Client NewClient;
	NewClient.StartClient();



	while (true)
	{
		NewClient.UpdateClient();
	}
	return 0;
}