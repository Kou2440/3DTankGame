#include "PlayScene.h"
#include "ObjectManager.h"
#include "EffectManager.h"
#include "EndScene.h"
#include "Director.h"
#include "BreakExplosion.h"
#include "SceneManager.h"
#include "SpaceScene.h"

PlayScene::PlayScene(int mapNum)
{
	imgHandle = LoadGraph("data/floor.jpg");

	ObjectManager::ReleseAllObj();

	//　マップマネージャの生成.
	MapManager* map = new MapManager(mapNum);

	// カメラ生成.
	MainCamera* mainCam = new MainCamera;


	this->mapNum = mapNum;
	deltaWaitTime = 0.0f;
	TimerOfWaitingTimeNextScene = 0.0f;
	fontHandle = CreateFontToHandle(NULL, fontSize, fontThick);
	str = "Ready";

	SceneManager::permission2Proceed = false;
}

PlayScene::~PlayScene()
{
}

SceneBase* PlayScene::Update(float deltaTime)
{
#if 1
	if (deltaWaitTime < WaitingTimeBeforStart)
	{
		deltaWaitTime += deltaTime;
		str = "Ready";
	}
	else
#endif
	{
		if (deltaWaitTime < WaitingTimeBeforStart + StringDrawTime)
		{
			str = "Fight!";
			deltaWaitTime += deltaTime;
		}
		// 全オブジェクトの更新.
		ObjectManager::Update(deltaTime);
		ObjectManager::Collition();

		EffectManager::Update(deltaTime);

		if (ObjectManager::TagObjectSize(ObjectTag::Enemy) == 0)
		{
			comment = "clear";
			SceneManager::permission2Proceed = true;

		}
		else if (ObjectManager::TagObjectSize(ObjectTag::Player1) == 0)
		{
			comment = "failed";
			SceneManager::permission2Proceed = true;

		}


		if (SceneManager::permission2Proceed)
		{
			
			TimerOfWaitingTimeNextScene += deltaTime;
			if (TimerOfWaitingTimeNextScene >= WaitingTimeToSwitchToTheNextScene)
			{
				ObjectManager::ReleseAllObj();
				EffectManager::ReleseAllEffect();
				if (MapDataPath.size() > mapNum + 1 && comment != "failed")
				{
					return new SpaceScene(mapNum + 1);
				}
				else
				{
					return new EndScene(comment);

				}
			}
			
		}

		
	}
	/*if (CheckHitKey(KEY_INPUT_F8))
	{
		ObjectManager::ReleseAllObj();
		return new EndScene(1);
	}*/
	return this;
}

void PlayScene::Draw()
{
	DrawExtendGraph(0, 0, ScreenSizeX, ScreenSizeY, imgHandle, false);
	if (deltaWaitTime < WaitingTimeBeforStart + StringDrawTime)
	{
		int strWidth = GetDrawStringWidthToHandle(str.c_str(), strlen(str.c_str()), fontHandle);
		DrawStringToHandle(ScreenSizeX / 2 - strWidth / 2, ScreenSizeY / 2, str.c_str(), Red, fontHandle);

	}
	// 全オブジェクトの描画.
	ObjectManager::Draw();
	EffectManager::Play();
}

