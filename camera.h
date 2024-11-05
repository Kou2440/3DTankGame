#pragma once
#include "Math.h"
#include "ObjectBase.h"

class MainCamera : public ObjectBase
{
public:
	MainCamera();

	void Update(float deltaTime);

private:

	const VECTOR InitPos =  VGet(0.0f, 870.0f, -0.1f);//ポジション.
	VECTOR aimTargetPos;// 注目点目標.
	const float CameraRangeNear = 0.1f;// カメラ描画範囲.
	const float CameraRangeFar = 1000.0f;// カメラ描画範囲.
};

