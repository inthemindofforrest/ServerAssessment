#include "Selection.h"

ClientSelection::ClientSelection()
{
	MyColor = ColorToInt(BLUE);
	Selected = 0;
	Min = 0; 
	Max = 2;
	Timer = GetTime();
}
void ClientSelection::Update(Player* _Client)
{
	float Delay = .35f;
	if (IsKeyDown(KEY_A) && GetTime() - Timer  > Delay)
	{
		UpdateSelected(-1);
		Timer = GetTime();
	}
	if (IsKeyDown(KEY_D) && GetTime() - Timer > Delay)
	{
		UpdateSelected(1);
		Timer = GetTime();
	}
	

	DrawRectangle(GetScreenWidth() / 2 - 105 + (Selected * 75), GetScreenHeight() / 2 - 15 - 50, 60, 30, YELLOW);

	DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 10 - 50, 50, 20, BLUE);
	DrawRectangle(GetScreenWidth() / 2 - 25, GetScreenHeight() / 2 - 10 - 50, 50, 20, RED);
	DrawRectangle(GetScreenWidth() / 2 + 50, GetScreenHeight() / 2 - 10 - 50, 50, 20, GREEN);

	switch (Selected)
	{
	case 0:
		MyColor = ColorToInt(BLUE);
		break;
	case 1:
		MyColor = ColorToInt(RED);
		break;
	case 2:
		MyColor = ColorToInt(GREEN);
		break;
	default:
		break;
	}

	(*_Client).MyColor = MyColor;
}

int ClientSelection::UpdateSelected(int _Value)
{
	Selected += _Value;
	if (Selected < Min) Selected = Max;
	if (Selected > Max) Selected = Min;
	return Selected;
}