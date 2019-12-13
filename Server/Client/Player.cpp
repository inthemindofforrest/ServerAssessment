#include "Player.h"
#include <stdio.h>
#include <mutex>

float Player::clamp(float _min, float _max, float _value)
{
	return (_value < _max) ? (_value > _min ? _value : _min) : _max;
}

Player::Player()
{
	Start();
}

//void Player::Update(Client* _Client)
//{
//	if (IsKeyDown(KEY_A))
//		UpdatePosition(ClientPosition[0] - speed, ClientPosition[1]);
//	if (IsKeyDown(KEY_D))
//		UpdatePosition(ClientPosition[0] + speed, ClientPosition[1]);
//	if (IsKeyDown(KEY_W))
//		UpdatePosition(ClientPosition[0], ClientPosition[1] - speed);
//	if (IsKeyDown(KEY_S))
//		UpdatePosition(ClientPosition[0], ClientPosition[1] + speed);
//
//	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
//		SendClientPosition(_Client);
//
//	//Update AllClientPositions
//	DrawClients(_Client);
//	//Draw();
//
//
//	//Manage
//	DisconnectFromServer(_Client);
//	ConsoleDisplay();
//	ManageConnectionStatus(_Client);
//}

void Player::Update(Client * _Client, Player * _Player)
{
	if ((*_Client).IsConnected)
	{
		if (IsKeyDown(KEY_A))
			UpdatePosition((*_Player).ClientPosition[0] - speed, (*_Player).ClientPosition[1]);
		if (IsKeyDown(KEY_D))
			UpdatePosition(ClientPosition[0] + speed, (*_Player).ClientPosition[1]);
		if (IsKeyDown(KEY_W))
			UpdatePosition((*_Player).ClientPosition[0], (*_Player).ClientPosition[1] - speed);
		if (IsKeyDown(KEY_S))
			UpdatePosition((*_Player).ClientPosition[0], (*_Player).ClientPosition[1] + speed);

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
			SendClientPosition(_Client);

		//Update AllClientPositions
		DrawClients(_Client);
		Draw();
	}

	if (IsKeyDown(KEY_ONE))
	{
		MyColor = ColorToInt(BLUE);
	}
	if (IsKeyDown(KEY_TWO))
	{
		MyColor = ColorToInt(RED);
	}
	if (IsKeyDown(KEY_THREE))
	{
		MyColor = ColorToInt(GREEN);
	}
	//Manage
	DisconnectFromServer(_Client);
	ConsoleDisplay();
	ManageConnectionStatus(_Client);
}

void Player::Start()
{
	ClientPosition[0] = 0;
	ClientPosition[1] = 0;
	MyColor = ColorToInt(BLUE);
	speed = 1;
}

void Player::Draw()
{
	DrawRectangle(ClientPosition[0], ClientPosition[1], 50, 20, GetColor(MyColor));
}
void Player::ColorDraw()
{
	DrawRectangle(GetScreenWidth() / 2 - 25, GetScreenHeight() / 2 - 10 - 50, 50, 20, GetColor(MyColor));

}

void Player::UpdatePosition(float _NewX, float _NewY)
{
	ClientPosition[0] = clamp(0, GetScreenWidth(), _NewX);
	ClientPosition[1] = clamp(0, GetScreenHeight(), _NewY);
}

void Player::DrawClients(Client* _Client)
{
	

	//Draw();
	if ((*_Client).AllClientPositions.size() > 0)
	{
		_Client->IsDrawing.lock();
		for (std::list<Positions>::iterator i = (*_Client).AllClientPositions.begin();
			i != (*_Client).AllClientPositions.end(); i++)
		{
			//if ((*_Client).CompareAddresses((*i).Address));
			DrawRectangle((*i).Value[0], (*i).Value[1], 40, 15, GetColor((*i).Color));
		}
		_Client->IsDrawing.unlock();
	}
}

void Player::SendClientPosition(Client* _Client)
{
	std::string StrPos = "PlayerPos,";
	StrPos.append(std::to_string(ClientPosition[0]) + ',');
	StrPos.append(std::to_string(ClientPosition[1]));

	_Client->SendPacket(StrPos.c_str());
}

void Player::DisconnectFromServer(Client * _Client)
{
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		(*_Client).SendPacket("Disconnect", 10, 0);
		(*_Client).IsConnected = false;
	}
}

void Player::ConsoleDisplay()
{
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

}

void Player::ManageConnectionStatus(Client* _Client)
{
	if (!(*_Client).IsConnected)
	{
		ClearBackground(DARKGRAY);
		ColorDraw();
		int Temp = MeasureText("Press Space to join a session!", 20) / 2;
		RayDrawText("Press Space to join a session!", (GetScreenWidth() / 2) - Temp, GetScreenHeight() / 2, 20, LIGHTGRAY);
		RayDrawText(std::to_string(GetFrameTime() * 60 * 60).c_str(), (GetScreenWidth() / 2), 0, 20, LIGHTGRAY);
		if (IsKeyPressed(KEY_SPACE))
		{
			(*_Client).SendPacket("Join");
			(*_Client).IsConnected = true;
			std::string T = "PlayerColor,";
			T.append(std::to_string(ColorToInt(GetColor(MyColor))));
			(*_Client).SendPacket(T.c_str());
		}
	}
}
