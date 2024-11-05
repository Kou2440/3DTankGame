#pragma once
#include <unordered_map>
#include <vector>
#include "EffectBase.h"

using namespace std;
/// <summary>
/// 演出用マネージャー.
/// </summary>
class EffectManager
{
public:

	static void Initialize();// 初期化.

	static void Entry(EffectBase* newEffect);
	static void Relese(EffectBase* releseEffevct);
	static void ReleseAllEffect();

	static void Update(float deltaTime);
	static void Play();
	static void Finalize();
private:
	EffectManager();
	~EffectManager();
	vector<EffectBase*>pendingEffect;
	vector<EffectBase*> effects;
	static EffectManager* Instance;
};

