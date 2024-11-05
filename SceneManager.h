#pragma once
#include "SceneBase.h"
#include "Director.h"
class SceneManager
{
public:
	~SceneManager();
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
	void SetScene(SceneBase* scene) { nowScene = scene; }
	int Update(float& deltaTime);
	void Draw();
	static bool exit;
	class SceneBase* nowScene;
	static bool permission2Proceed;// êiçsãñâ¬.

private:

};

#define SCENEINSTANCE SceneManager::GetInstance()

