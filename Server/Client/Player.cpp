#include "Player.h"
#include <stdio.h>
#include <mutex>

//Star Functions

Star::Star(){}
Star::Star(int _X, int _Y, float _Speed, float _Size)
{
	Pos.Value[0] = _X;
	Pos.Value[1] = _Y;
	Speed = _Speed;
	Size = _Size;
}

//Player Functions

float Player::clamp(float _min, float _max, float _value)
{
	return (_value < _max) ? (_value > _min ? _value : _min) : _max;
}

Player::Player()
{
	Start();
}

void Player::Update(Client * _Client, Player * _Player)
{
	UpdateStars();
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

		if (IsMouseButtonDown(0) && GetTime() - ShootTimer > ShootDelay)
		{
			std::string Temp = "Bullet,";
			Temp.append(std::to_string(_Player->ClientPosition[0] + Size[0]) + ",");
			Temp.append(std::to_string(_Player->ClientPosition[1] + (Size[1] / 2)) + ",");
			Temp.append(std::to_string(_Player->MyColor) + ";");
			_Client->SendPacket(Temp.c_str());
			ShootTimer = GetTime();
		}

		//Update AllClientPositions
		DrawClients(_Client);
		DrawBullets(_Client);
		DrawAstroids(_Client);
		Draw(_Client);
	}
	else
	{

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
	Size[0] = 50;
	Size[1] = 20;
	MyColor = ColorToInt(BLUE);
	speed = 2;

	for (int i = 0; i < 100; i++)
	{
		float TempSize = (rand() % (200 - 10 + 10) + 10) / 100;
		float TempSpeed = TempSize * .5f;
		Stars.push_front(Star(rand() % (GetScreenWidth() - 800 + 1) + 2, rand() % (GetScreenHeight() - 450 + 1) + 2, 
			TempSpeed, TempSize));
	}
}

void Player::Draw(Client * _Client)
{
	DrawRectangle(ClientPosition[0], ClientPosition[1], 50, 20, GetColor(MyColor));
	RayDrawText(std::to_string(_Client->MyScore).c_str(), GetScreenWidth() / 2 - MeasureText(std::to_string(_Client->MyScore).c_str(), 80) / 2, 0, 80, WHITE);
}

void Player::UpdatePosition(float _NewX, float _NewY)
{
	ClientPosition[0] = clamp(0, GetScreenWidth() - 50, _NewX);
	ClientPosition[1] = clamp(80, GetScreenHeight() - 20, _NewY);
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
			DrawRectangle((*i).Value[0], (*i).Value[1], 40, 15, GetColor((*i).Color));
			RayDrawText(std::to_string((*i).Score).c_str(), (*i).Value[0] + 15, (*i).Value[1] - 25, 25, WHITE);
		}
		_Client->IsDrawing.unlock();
	}
}

void Player::DrawBullets(Client* _Client)
{
	//Draw();
	if ((*_Client).Bullets.size() > 0)
	{
		_Client->IsDrawing.lock();
		for (std::list<Positions>::iterator i = (*_Client).Bullets.begin();
			i != (*_Client).Bullets.end(); i++)
		{
			DrawRectangle((*i).Value[0], (*i).Value[1], 20, 5, GetColor((*i).Color));
		}
		_Client->IsDrawing.unlock();
	}
}

void Player::DrawAstroids(Client* _Client)
{
	//Draw();
	if ((*_Client).Astroids.size() > 0)
	{
		_Client->IsDrawing.lock();
		for (std::list<Positions>::iterator i = (*_Client).Astroids.begin();
			i != (*_Client).Astroids.end(); i++)
		{
			DrawRectangle((*i).Value[0], (*i).Value[1], 50, 50, DARKGRAY);
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
		ClientPosition[0] = 0;
		ClientPosition[1] = 80;

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
		ClearBackground(BLACK);
		int Temp = MeasureText("Press Space to join a session!", 30) / 2;
		RayDrawText("Press Space to join a session!", (GetScreenWidth() / 2) - Temp, GetScreenHeight() / 2, 30, LIGHTGRAY);
		RayDrawText("Ray Blaster 3.0", (GetScreenWidth() / 2) - MeasureText("Ray Blaster 3.0", 100) / 2, 0, 100, LIGHTGRAY);
		RayDrawText("MULTIPLAYER", (GetScreenWidth() / 2) + (GetScreenWidth() / 4) - MeasureText("MULTIPLAYER", 30) / 2, 100, 30, RED);
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

void Player::UpdateStars()
{
	for (std::list<Star>::iterator i = Stars.begin();
		i != Stars.end(); i++)
	{
		i->Pos.Value[0]-= 3 * i->Speed;
		if (i->Pos.Value[0] < 0)
		{
			i->Pos.Value[0] = GetScreenWidth();
			i->Pos.Value[1] = rand() % (GetScreenHeight() - 0 + 1) + 0;
		}
		DrawCircle(i->Pos.Value[0], i->Pos.Value[1], 2 * i->Size, YELLOW);
	}

}