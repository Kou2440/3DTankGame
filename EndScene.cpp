#include "EndScene.h"
#include "SystemConstant.h"
#include "SceneManager.h"
#include "StartScene.h"
EndScene::EndScene(std::string comment)
{
	fontHandle = CreateFontToHandle(NULL, fontSize, fontThick);
	alpha = 0;
	winOrlose = comment;
}

EndScene::~EndScene()
{
	DeleteFontToHandle(fontHandle);
}

SceneBase* EndScene::Update(float deltaTime)
{
	if (CheckHitKeyAll())
	{
		WaitTimer(WaitTime);
		return new StartScene;
	}
	return this;
}

void EndScene::Draw()
{
	int strWidth = GetDrawStringWidthToHandle(winOrlose.c_str(), strlen(winOrlose.c_str()), fontHandle);
	DrawFormatStringToHandle(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY / 2, Green, fontHandle, winOrlose.c_str());
	alpha += addition;
	if (alpha > 255)
	{
		addition *= -1;
	}
	if (alpha < 0)
	{
		addition *= -1;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	int strLne = strlen(Str.c_str());
	strWidth = GetDrawStringWidth(Str.c_str(), strLne);
	DrawFormatString(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY * 2 / 3, Green, Str.c_str());
	strLne = strlen(Str2.c_str());
	strWidth = GetDrawStringWidth(Str2.c_str(), strLne);
	DrawFormatString(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY * 2 / 3 + str2Pos, Green, Str2.c_str());
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
