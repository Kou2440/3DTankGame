#include "EffectManager.h"
#include "EffekseerForDXLib.h"

EffectManager* EffectManager::Instance = nullptr;

//------------------------------------------------------------------------------
// @brief オブジェクトマネージャ　コンストラクタ.
//------------------------------------------------------------------------------
EffectManager::EffectManager() :
	effects()
{
	Instance = nullptr;
}
//------------------------------------------------------------------------------
// @brief オブジェクトマネージャ　デストラクタ.
//------------------------------------------------------------------------------
EffectManager::~EffectManager()
{
	ReleseAllEffect();
}

void EffectManager::Initialize()
{
	if (!Instance)
	{
		Instance = new EffectManager();
	}
}

/// <summary>
/// マネージャーへエフェクトの追加.
/// </summary>
/// <param name="newEffect"></param>
void EffectManager::Entry(EffectBase* newEffect)
{
	Instance->pendingEffect.push_back(newEffect);
}

/// <summary>
/// 指定のエフェクトをマネージャー削除.
/// </summary>
/// <param name="releseEffevct"></param>
void EffectManager::Relese(EffectBase* releseEffevct)
{

	// ペンディングオブジェクト内から検索.
	auto iter = find(Instance->pendingEffect.begin(), Instance->pendingEffect.end(), releseEffevct);
	if (iter != Instance->pendingEffect.end())
	{
		// 見つけたオブジェクトを最後尾に移動してデータを消す.
		iter_swap(iter, Instance->pendingEffect.end() - 1);
		Instance->pendingEffect.pop_back();
		return;
	}

	// アクティブオブジェクト内から削除Objectを検索.
	iter = find(Instance->effects.begin(), Instance->effects.end(), releseEffevct);
	if (iter != Instance->effects.end())
	{
		// 見つけたオブジェクトを末尾に移動し、削除.
		iter_swap(iter, Instance->effects.end() - 1);
		delete Instance->effects.back();
	}
}
/// <summary>
/// マネージャーから全てのエフェクトを削除.
/// </summary>
void EffectManager::ReleseAllEffect()
{
	// 末尾からペンディングオブジェクトをすべてを削除
	while (!Instance->pendingEffect.empty())
	{

		delete Instance->pendingEffect.back();
		Instance->pendingEffect.pop_back();
	}

	// すべてのアクティブオブジェクトを削除
		// 末尾から削除
	while (!Instance->effects.empty())
	{
		delete Instance->effects.back();
		Instance->effects.pop_back();
	}
}
/// <summary>
///  更新.
/// </summary>
/// <param name="deltaTime"></param>
void EffectManager::Update(float deltaTime)
{
	// すべてのアクターの更新
		// 該当タグにあるすべてのオブジェクトを更新
	for (int i = 0; i < Instance->effects.size(); ++i)
	{
		Instance->effects[i]->Update(deltaTime);
	}
	// ペンディング中のオブジェクトをアクティブリストに追加
	for (auto pending : Instance->pendingEffect)
	{
		Instance->effects.emplace_back(pending);
	}
	Instance->pendingEffect.clear();

	// すべてのアクター中で死んでいるアクターをdeadObjectへ一時保管
	vector<EffectBase*> deadObject;
	//deadObjectに移動
	for (auto obj : Instance->effects)
	{
		if (!obj->GetValid())
		{
			deadObject.emplace_back(obj);
		}
	}
	Instance->effects.erase(remove_if(begin(Instance->effects), end(Instance->effects), [](EffectBase* b) { return !b->GetValid(); }), cend(Instance->effects));

	// 死んでいるオブジェクトをdelete
	while (!deadObject.empty())
	{
		delete deadObject.back();
		deadObject.pop_back();
	}
}
/// <summary>
/// 再生.
/// </summary>
void EffectManager::Play()
{
	for (int i = 0; i < Instance->effects.size(); ++i)
	{
		// 描画可能なオブジェクトのみ描画.
		if (Instance->effects[i]->GetValid())
		{
			DrawEffekseer3D_Begin();
			Instance->effects[i]->Play();
			DrawEffekseer3D_End();
		}
	}

}

void EffectManager::Finalize()
{
	ReleseAllEffect();
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}
