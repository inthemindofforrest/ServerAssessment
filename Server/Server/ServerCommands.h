#pragma once
#include <stdio.h>


class CommandReader
{
public:
	CommandReader();

	bool RecieveCommand();
	bool CheckCommand();

private:

	int MaxCommandLength;
	char* Command;


};