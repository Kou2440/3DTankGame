#include "StartScene.h"
#include "SystemConstant.h"
#include "SceneManager.h"
#include <cmath>
#include "SpaceScene.h"
StartScene::StartScene()
{
	alpha = 0.0f;
}
SceneBase* StartScene::Update(float deltaTime)
{

	if (CheckHitKeyAll())
	{
		return new SpaceScene(1);
	} 
	return this;
}

void StartScene::Draw()
{
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
	DrawFormatString(ScreenSizeX / 2 - StrWidth / 2, ScreenSizeY * 2 / 3, Green, Str.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}
