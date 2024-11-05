#include "SceneManager.h"

SceneManager::~SceneManager()
{
}

int SceneManager::Update(float& deltaTime)
{
	if (!nowScene)
	{
		return 0;
	}

	SceneBase* tmpScene;
	tmpScene = nowScene->Update(deltaTime);
	if (tmpScene != nowScene)
	{
		delete nowScene;
		nowScene = tmpScene;
		return 1;
	}
	return 0;
}

void SceneManager::Draw()
{
	if (!nowScene)
	{
		return;
	}
	nowScene->Draw();
}
