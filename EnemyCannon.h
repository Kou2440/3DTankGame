#pragma once
#include "ObjectBase.h"
#include "Math.h"
#include <map>
class EnemyCannon : public ObjectBase
{
public:
	EnemyCannon(VECTOR initPos, VECTOR initDir, ObjectTag userTag);
	~EnemyCannon();
	void Update(float deltaTime) {};// 更新(不使用).
	void Updateeeee(VECTOR bodyPos, float deltaTime);// 更新.
	void Draw();// 描画.
	void Initialize(VECTOR initPos, VECTOR initDir);
private:
	void Rotate(float deltaTime);
	bool Search(VECTOR playerPos);
	void DiviationValculation(float deltaTime);
	void Fire();
	VECTOR aimDir;
	bool rotateNow;
	int padInput;
	const float TurnPerformance = 3.50f;// 旋回性能.
	const float AdjustPos = 0.5f;
	const float ShotIntervalTime = 2.0f;
	const float Omega = 25.0f;// 主砲旋回速度.
	float shotTime;
	//const float RadarRange = ToRadian(150.0f);
	const float Radius = 50.0f;
	ObjectTag userTag;
	VECTOR jbPlayerPos;// 直前のプレイヤー位置.jb=just before
	int stageNum;
	const float FOVDegree = 8.0f;
	float leftOrRight;
	float rotateAngle;
	VECTOR a;
};

