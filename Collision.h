#pragma once
#include "DxLib.h"
#include "Math.h"
#include <vector>


// 線分当たり判定.
struct LineSegment
{
	LineSegment();
	LineSegment(const VECTOR& start, const VECTOR& end);
	void Move(const VECTOR& pos);

	VECTOR localStart;
	VECTOR localEnd;
	VECTOR worldStart;
	VECTOR worldEnd;
};
// 球体当たり判定.
struct Sphere
{
	Sphere();
	Sphere(const VECTOR& center, float radius);

	void Move(const VECTOR& pos);

	VECTOR localCenter;
	VECTOR worldCenter;
	float radius;
};
// カプセル当たり判定.
struct Capsule
{
	Capsule();
	Capsule(const VECTOR& start, const VECTOR& end, float radius);
	void Move(const VECTOR& pos);


	VECTOR localStart;
	VECTOR localEnd;
	VECTOR worldStart;
	VECTOR worldEnd;
	float radius;
};
// 当たり判定関数組み合わせ.
bool CollisionPair(const Sphere& sphere1, const Sphere& sphere2);
bool CollisionPair(const LineSegment& line, const Sphere& sphere);
bool CollisionPair(const Sphere& sphere, const LineSegment& line);
bool CollisionPair(const Sphere& sphere, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo);
bool CollisionPair(const Capsule& capsule, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo);
bool CollisionPair(const LineSegment& line, const int modelHandle, MV1_COLL_RESULT_POLY& collisionInfo);
bool CollisionPair(const int modelHandle, const LineSegment& line, MV1_COLL_RESULT_POLY& collisionInfo);
bool CollisionPair(const Capsule& capsule1, const Sphere& sphere);
// 球体とメッシュとの当たり押し戻し量計算.
VECTOR CalcSpherePushBackVecFromMesh(const Sphere& sphere, const MV1_COLL_RESULT_POLY_DIM& collisionInfo);

bool offscreenDicision(VECTOR pos, float radius = 0.0f);// 画面外判定.