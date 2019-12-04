#include "Player.h"
#include <stdio.h>

float Player::clamp(float _min, float _max, float _value)
{
	return (_value < _max) ? (_value > _min ? _value : _min) : _max;
}

void Player::Update()
{
	if (IsKeyDown(KEY_A)) 
		UpdatePosition(Position[0] - speed * GetFrameTime(), Position[1]);
	if (IsKeyDown(KEY_D)) 
		UpdatePosition(Position[0] + speed * GetFrameTime(), Position[1]);
	if (IsKeyDown(KEY_W)) 
		UpdatePosition(Position[0], Position[1] - speed * GetFrameTime());
	if (IsKeyDown(KEY_S)) 
		UpdatePosition(Position[0], Position[1] + speed * GetFrameTime());
	//printf("(%f,%f)\n", Position[0], Position[1]);


	Draw();
}

void Player::Start()
{
	Position[0] = 0;
	Position[1] = 0;
	speed = 10;
}

void Player::Draw()
{
	DrawRectangle(Position[0], Position[1], 50, 50, RED);
}

void Player::UpdatePosition(float _NewX, float _NewY)
{
	Position[0] = clamp(0, GetScreenWidth(), _NewX);
	Position[1] = clamp(0, GetScreenHeight(), _NewY);
}

void Player::SendPosition()
{
}
