#pragma once
#include "ObjectBase.h"

class PlayerCannon : public ObjectBase
{
public:
	
	PlayerCannon(VECTOR initPos, VECTOR initDir, int inputState, ObjectTag userTag, const char* failName);
	~PlayerCannon();



	void Update(float deltaTime);// 更新(不使用).
	void Updateeeee(VECTOR bodyPos, float deltaTime);// 更新.
	void Draw();// 描画.


	VECTOR GetPos() const { return pos; }
	VECTOR GetDir() const { return dir; }

	void Initialize(VECTOR initPos, VECTOR initDir);
	void Input(float deltaTime, XINPUT_STATE pad);
private:
	void Rotate(float deltaTime);
	
	VECTOR aimDir;
	bool rotateNow;
	int padInput;
	const float TurnPerformance = 3.50f;// 旋回性能.
	const float AdjustPos = 0.5f;
	const float ShotIntervalTime = 1.0f;
	const float Omega = 50.0f;
	float shotTime;

	ObjectTag userTag;
};


