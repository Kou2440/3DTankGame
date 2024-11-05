#include "UI.h"
#include "SystemConstant.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::Draw(float deltaTime)
{
	Debug();
}

void UI::Winner(const PlayerBody* player)
{

	struct PlayerName
	{
		ObjectTag tag; 
		string winnerName;
	};
	PlayerName playerTag[PlayerNum] =
	{
		{ObjectTag::Player1, "Player1"},
		{ObjectTag::Player2, "Player2"}
	};

	for (int i = 0; i < PlayerNum; i++)
	{
		if (playerTag[i].tag == player->GetNameTag())
		{
			DrawStringInTheCenter(playerTag[i].winnerName);
		}
	}
}

void UI::Debug()
{
	//PlayerBody* player = dynamic_cast<PlayerBody*>(ObjectManager::GetFirstObject(ObjectTag::Player));
	//VECTOR pPos = player->GetPos();
	/*DrawFormatString(0, 0, White,
		"pos:\n%f\n%f\n%f\n\ndir:\n%f\n%f\n%f\n\nvel:\n%f\n%f\n%f\n\nsize:%f\ndot:%f"
		);*/
	//PlayerCannon* cannon = dynamic_cast<PlayerCannon*>(ObjectManager::GetFirstObject(ObjectTag::Cannon));
	/*DrawFormatString(0, 0, White, "0:%f\n1:%f\n2:%f", cannon->monitorNum[0], cannon->monitorNum[1],
		cannon->monitorNum[2]);*/
	//DrawFormatString(0, 0, White, "%d", GetJoypadInputState(DX_INPUT_PAD2));
}

void UI::DrawStringInTheCenter(const string str)
{
	int strWidth;
	strWidth = GetDrawFormatStringWidth(str.c_str());
	DrawFormatString(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY / 2, White, str.c_str());
		
}
