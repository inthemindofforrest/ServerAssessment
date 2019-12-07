#include "Player.h"
#include <stdio.h>
#include <mutex>

float Player::clamp(float _min, float _max, float _value)
{
	return (_value < _max) ? (_value > _min ? _value : _min) : _max;
}

void Player::Update(Client* _Client)
{
	if (IsKeyDown(KEY_A))
		UpdatePosition(ClientPosition[0] - speed, ClientPosition[1]);
	if (IsKeyDown(KEY_D))
		UpdatePosition(ClientPosition[0] + speed, ClientPosition[1]);
	if (IsKeyDown(KEY_W))
		UpdatePosition(ClientPosition[0], ClientPosition[1] - speed);
	if (IsKeyDown(KEY_S))
		UpdatePosition(ClientPosition[0], ClientPosition[1] + speed);

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
		SendClientPosition(_Client);

	//Update AllClientPositions
	DrawClients(_Client);
	//Draw();
}

void Player::Start()
{
	ClientPosition[0] = 0;
	ClientPosition[1] = 0;
	speed = 1;
}

void Player::Draw()
{
	DrawRectangle(ClientPosition[0], ClientPosition[1], 50, 50, RED);
}

void Player::UpdatePosition(float _NewX, float _NewY)
{
	ClientPosition[0] = clamp(0, GetScreenWidth(), _NewX);
	ClientPosition[1] = clamp(0, GetScreenHeight(), _NewY);
}

void Player::DrawClients(Client* _Client)
{
	if ((*_Client).AllClientPositions.size() > 0)
	{
		_Client->IsDrawing.lock();
		for (std::list<Positions>::iterator i = (*_Client).AllClientPositions.begin();
			i != (*_Client).AllClientPositions.end(); i++)
		{
			DrawRectangle((*i).Value[0], (*i).Value[1], 50, 50, RED);
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