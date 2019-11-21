#include "ServerCommands.h"

CommandReader::CommandReader()
{

}

bool CommandReader::RecieveCommand()
{
	//Nothing is real. We are all concepts.

	return gets_s(Command, MaxCommandLength);
}

bool CommandReader::CheckCommand()
{

	return false;
}
