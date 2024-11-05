#pragma once
#include "ObjectBase.h"

#include "PlayerCannon.h"
#include "HPGauge.h"
#include "EffectBase.h"
class EffectBase;
class PlayerBody :
	public ObjectBase
{
public:

	PlayerBody(VECTOR initPos, VECTOR initDir, int inputState, ObjectTag myTag, const char* failName);
	~PlayerBody();

	//static PlayerBody& Instance() { static PlayerBody body; return body; }

	void Update(float deltaTime) override;// 更新.
	void Draw() override;// 描画.
	void OnCollisionEnter(const ObjectBase* other) override;

	void Initialize();

	void Input(float deltaTime);
	void AddWinNum() { winNum += 1; }
	int GetWinNum() const { return winNum; }
	ObjectTag GetNameTag() const { return nameTag; }
private:
	void Rotate(float deltaTIme);

	VECTOR initPos;// 初期ポジション.
	VECTOR initDir;// 初期ディレクション.
	VECTOR aimDir;// 目標方向.
	VECTOR velocity;// 加速ベクトル.
	bool rotateNow;// 回転しているかどうか.
	float accel;// 加速力.
	int winNum;// 勝利数.
	const float MaxHP = 10.0f;
	ObjectTag nameTag;
	// コントローラー変数.
	int padInput;
	XINPUT_STATE pad;

	class PlayerCannon* cannon;
	class HPGauge* hpGauge;

	const float Accel = 6.0f;// 通常の加速.
	const float Back = 5.0f;// 後退速度.
	const float MaxSpeed = 300.0f;// 最高前進速度.
	const float MinSpeed = -200.0f;// 最高後退速度.
	const float DefaultDecel = 0.97f;// なにもしない時の減速.
	//const float GripDecel = -5.0f;// グリップの減速.
	const float TurnPerformance = 5.0f;// 旋回性能.
	const float DamagePoint = 20.0f;
	const float ColRadius = 32.0f;// 当たり判定半径.
	const float Epsilon = 8.0f;// 誤差.
	const float Omega = 10.0f;
	const float CapsuleLen = 20.0f;// カプセルの長さ.
	EffectBase* breakEffect;
};

