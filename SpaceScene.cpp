#include "SpaceScene.h"
#include "SystemConstant.h"
#include "SceneManager.h"
#include "PlayScene.h"
SpaceScene::SpaceScene(int stage)
{
	fontHandle = CreateFontToHandle(NULL, fontSize, fontThick);
	stageNum = stage;
	Timer = 0.0f;
}

SpaceScene::~SpaceScene()
{
}

SceneBase* SpaceScene::Update(float deltaTime)
{
	Timer += deltaTime;
	if (Timer >= WaitTime)
	{
		return new PlayScene(stageNum);
	}
	return this;
}

void SpaceScene::Draw()
{
	int strWidth = GetDrawStringWidthToHandle("Stage ",strlen("Stage "), fontHandle);
	DrawFormatStringToHandle(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY / 2, Green, fontHandle, "Stage%d", stageNum);

}
