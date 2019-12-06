#include "ThreadManager.h"

void RunThreadWithLoop(Thup _Thread, Tup _Function)
{
	*_Thread = std::thread([&]
	{
		Tup Function = _Function;
		while (true)
		{
			Function();
		}
	});
}
