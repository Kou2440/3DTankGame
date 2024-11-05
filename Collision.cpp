#include "Collision.h"
#include "SystemConstant.h"
#include <cassert>
#include <algorithm>
#include <functional>
//-----------------------------------------------------------------------------
// @brief 線分コンストラクタ
//-----------------------------------------------------------------------------
LineSegment::LineSegment()
	:localStart()
	,localEnd()
	,worldStart()
	,worldEnd()
{
}
//-----------------------------------------------------------------------------
// @brief 線分コンストラクタ初期位置セット
//-----------------------------------------------------------------------------

LineSegment::LineSegment(const VECTOR& start, const VECTOR& end)
	:localStart(start)
	,localEnd(end)
	,worldStart(start)
	,worldEnd(end)
{
}

//-----------------------------------------------------------------------------
// @brief 線分のワールド位置を移動する
//-----------------------------------------------------------------------------
void LineSegment::Move(const VECTOR& pos)
{
	worldStart = localStart + pos;
	worldEnd = localEnd + pos;
}


//-----------------------------------------------------------------------------
// @brief 球体コンストラクタ
//-----------------------------------------------------------------------------
Sphere::Sphere()
	:localCenter()
	, worldCenter()
	, radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief 球体コンストラクタ
//-----------------------------------------------------------------------------
Sphere::Sphere(const VECTOR& center, float radius)
	:localCenter(center)
	,worldCenter(center)
	,radius(radius)
{
}

//-----------------------------------------------------------------------------
// @brief 球体のワールド座標を移動する
//-----------------------------------------------------------------------------
void Sphere::Move(const VECTOR& pos)
{
	worldCenter = localCenter + pos;
}

//-----------------------------------------------------------------------------
// @brief カプセル当たり判定コンストラクタ
//-----------------------------------------------------------------------------
Capsule::Capsule()
	:localStart()
	,localEnd()
	,worldStart()
	,worldEnd()
	,radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief カプセル当たり判定コンストラクタ
//-----------------------------------------------------------------------------
Capsule::Capsule(const VECTOR& start, const VECTOR& end, float radius)
	:localStart(start)
	,localEnd(end)
	,worldStart(start)
	,worldEnd(end)
	,radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief カプセルのワールド座標を移動する
//-----------------------------------------------------------------------------
void Capsule::Move(const VECTOR& pos)
{
	worldStart = localStart + pos;
	worldEnd = localEnd + pos;
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 球・球
// @param[in] sphere1 球
// @param[in] sphere2 球
// @return bool 球同士がぶつかっている場合はtrue を返す
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere1, const Sphere& sphere2)
{
	return HitCheck_Sphere_Sphere(sphere1.worldCenter, sphere1.radius, sphere2.worldCenter, sphere2.radius);
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 線分・球
// @param[in] line1 線分
// @param[in] sphere1 球
// @return bool 線分と球がぶつかっている場合はtrue を返す
//-----------------------------------------------------------------------------
bool CollisionPair(const LineSegment& line, const Sphere& sphere)
{
	return HitCheck_Line_Sphere(line.worldStart, line.worldEnd, sphere.worldCenter, sphere.radius);
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 線分・球
// @param[in] sphere 球
// @param[in] line 線分
// @return bool 球と線分がぶつかっている場合はtrue を返す
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere, const LineSegment& line)
{
	return HitCheck_Line_Sphere(line.worldStart, line.worldEnd, sphere.worldCenter, sphere.radius);
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 線分・メッシュ
// @param[in]  line          線分
// @param[in]  modelHandle   メッシュのモデルID
// @param[out] collisionInfo 当たった場合のポリゴン数、ポリゴンリスト
// @return     線分とモデルが衝突している場合はtrueを返す。collisionInfoに当たり判定詳細情報が返る
//-----------------------------------------------------------------------------
bool CollisionPair(const LineSegment& line, const int modelHandle, MV1_COLL_RESULT_POLY& collisionInfo)
{
	collisionInfo = MV1CollCheck_Line(modelHandle, -1, line.worldStart, line.worldEnd);
	return collisionInfo.HitFlag;
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 メッシュ・線分
// @param[in]  modelHandle   メッシュのモデルID
// @param[in]  line          線分
// @param[out] collisionInfo 当たった場合のポリゴン数、ポリゴンリスト
// @return     線分とモデルが衝突している場合はtrueを返す。collisionInfoに当たり判定詳細情報が返る
// @detail     線分とモデル衝突の当たり判定引数が逆バージョン
//-----------------------------------------------------------------------------
bool CollisionPair(const int modelHandle, const LineSegment& line, MV1_COLL_RESULT_POLY& collisionInfo)
{
	return CollisionPair(line, modelHandle, collisionInfo);
}

//-----------------------------------------------------------------------------
// @brief 当たり判定 球・メッシュ
// @param[in]  sphere        球
// @param[in]  modelHandle   メッシュのモデルID
// @param[out] collisionInfo 当たった場合のポリゴン数、ポリゴンリスト
// @return     球とモデルが衝突している場合はtrueを返す
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	collisionInfo = MV1CollCheck_Sphere(modelHandle, -1, sphere.worldCenter, sphere.radius);
	if (collisionInfo.HitNum == 0)
	{
		return false;
	}
	return true;
}

bool CollisionPair(const Capsule& capsule, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	collisionInfo = MV1CollCheck_Capsule(modelHandle, -1, capsule.worldStart, capsule.worldEnd, capsule.radius);
	if (collisionInfo.HitNum == 0)
	{
		return false;
	}
	return true;
}

bool CollisionPair(const Capsule& capsule1, const Sphere& sphere)
{
	VECTOR startToSphere = capsule1.worldStart - sphere.worldCenter;// startToSphere   
	VECTOR startToEnd = capsule1.worldEnd - sphere.worldCenter;// startToEnd      
	VECTOR nAB = VNorm(startToSphere);//
	float dot = VDot(nAB, startToEnd);//nearLength      
	VECTOR n = capsule1.worldStart + (nAB * dot);
	
	float ret = dot / VSize(startToSphere);//nearLengthRate  
	VECTOR a = startToSphere * min(max(ret, 0), 1);
	VECTOR mNear = capsule1.worldStart + a;//near            

	float distance = 0.0f;
	VECTOR endToSphere = sphere.worldCenter - capsule1.worldEnd;
	VECTOR nearToSphere = sphere.worldCenter - n;
	if (ret < 0)
	{
		distance = VSize(capsule1.worldStart) * VSize(capsule1.worldStart);
	}
	else if (ret > 1)
	{
		distance = VSize(capsule1.worldEnd) * VSize(capsule1.worldEnd);
	}
	else
	{
		distance = VSize(sphere.worldCenter - mNear) * VSize(sphere.worldCenter - mNear);
	}
	return distance - (sphere.radius + capsule1.radius) * (sphere.radius + capsule1.radius) <= 0;

}
//-----------------------------------------------------------------------------
// 押し戻す量を返す.
//-----------------------------------------------------------------------------
VECTOR CalcSpherePushBackVecFromMesh(const Sphere& sphere, const MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	// 衝突球
	std::vector<VECTOR> moveCandidate; // 球中心候補 
	float  radius = sphere.radius; // 球半径
	VECTOR planeNormal;                    // ポリゴン平面法線
	VECTOR moveVec = VGet(0, 0, 0);    // 移動ベクトル
	float  moveLen = 0.0f;           // 移動量
	VECTOR newCenter = sphere.worldCenter; // 移動候補  
	int i = 0;

	int   minindex = 0;                  // 最短距離候補インデックス
	float minLen = sphere.radius * 2.0f; // 押し戻し最短距離（直径以上は押し戻しはないはず）

	// 衝突ポリゴンをすべて回って、球のめり込みを解消する
	for (i = 0; i < collisionInfo.HitNum; ++i)
	{
		moveCandidate.push_back(sphere.worldCenter);
		// 衝突ポリゴンの辺 
		VECTOR edge1, edge2;
		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];

		// 衝突ポリゴンの辺より、ポリゴン面の法線ベクトルを求める
		planeNormal = VCross(edge1, edge2);
		planeNormal = VNorm(planeNormal);

		// 球中心に最も近いポリゴン平面の点を求める
		VECTOR tmp = moveCandidate[i] - collisionInfo.Dim[i].Position[0];
		float  dot = VDot(planeNormal, tmp);

		// 衝突点
		VECTOR hitPos = moveCandidate[i] - planeNormal * dot;

		// 球がどれくらいめり込んでいるかを算出
		VECTOR tmp2 = moveCandidate[i] - hitPos;
		float  len = VSize(tmp2);

		// めり込んでいる場合は球の中心を押し戻し
		if (HitCheck_Sphere_Triangle(moveCandidate[i], radius,
			collisionInfo.Dim[i].Position[0],
			collisionInfo.Dim[i].Position[1],
			collisionInfo.Dim[i].Position[2]) == TRUE)
		{
			// めり込み解消する位置まで移動
			VECTOR moveVec;
			
			minLen = radius - len;
			//len += 0.0001f;
			moveVec = planeNormal * minLen;
			moveCandidate[i] = moveVec;

			// 今押し戻した距離はこれまでの候補よりも短い距離か
			if (minLen < len)
			{
				minLen = len;
				minindex = i;
			}
		}
	}

	//moveCandidate[minindex]には最短移動距離が入っており、移動後の新しい球の中心位置とする
	return moveCandidate[minindex];
}

//VECTOR CalCapsulePushBackVecFromMesh(const Capsule& capsule, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
//{
//	// 衝突球
//	std::vector<VECTOR> moveCandidate; // 球中心候補 
//	float  radius = capsule.radius; // 球半径
//	VECTOR planeNormal;                    // ポリゴン平面法線
//	VECTOR moveVec = VGet(0, 0, 0);    // 移動ベクトル
//	float  moveLen = 0.0f;           // 移動量
//	
//	//VECTOR newCenter = sphere.worldCenter; // 移動候補 
//	int i = 0;
//
//	int   minindex = 0;                  // 最短距離候補インデックス
//	float minLen = sphere.radius * 2.0f; // 押し戻し最短距離（直径以上は押し戻しはないはず）
//
//	// 衝突ポリゴンをすべて回って、球のめり込みを解消する
//	for (i = 0; i < collisionInfo.HitNum; ++i)
//	{
//		moveCandidate.push_back(sphere.worldCenter);
//		// 衝突ポリゴンの辺 
//		VECTOR edge1, edge2;
//		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
//		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];
//
//		// 衝突ポリゴンの辺より、ポリゴン面の法線ベクトルを求める
//		planeNormal = VCross(edge1, edge2);
//		planeNormal = VNorm(planeNormal);
//
//		// 球中心に最も近いポリゴン平面の点を求める
//		VECTOR tmp = moveCandidate[i] - collisionInfo.Dim[i].Position[0];
//		float  dot = VDot(planeNormal, tmp);
//
//		// 衝突点
//		VECTOR hitPos = moveCandidate[i] - planeNormal * dot;
//
//		// 球がどれくらいめり込んでいるかを算出
//		VECTOR tmp2 = moveCandidate[i] - hitPos;
//		float  len = VSize(tmp2);
//
//		// めり込んでいる場合は球の中心を押し戻し
//		if (HitCheck_Sphere_Triangle(moveCandidate[i], radius,
//			collisionInfo.Dim[i].Position[0],
//			collisionInfo.Dim[i].Position[1],
//			collisionInfo.Dim[i].Position[2]) == TRUE)
//		{
//			// めり込み解消する位置まで移動
//			VECTOR moveVec;
//
//			minLen = radius - len;
//			//len += 0.0001f;
//			moveVec = planeNormal * minLen;
//			moveCandidate[i] = moveVec;
//
//			// 今押し戻した距離はこれまでの候補よりも短い距離か
//			if (minLen < len)
//			{
//				minLen = len;
//				minindex = i;
//			}
//		}
//	}
//
//	//moveCandidate[minindex]には最短移動距離が入っており、移動後の新しい球の中心位置とする
//	return moveCandidate[minindex];
//}

VECTOR PlaneNormal(const MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	VECTOR planeNormal;                    // ポリゴン平面法線
	for (int i = 0; i < collisionInfo.HitNum; ++i)
	{
		// 衝突ポリゴンの辺 
		VECTOR edge1, edge2;
		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];

		// 衝突ポリゴンの辺より、ポリゴン面の法線ベクトルを求める
		planeNormal = VCross(edge1, edge2);
		planeNormal = VNorm(planeNormal);
	}
	return VECTOR();
}


bool offscreenDicision(VECTOR pos, float radius)
{
	if (ConvWorldPosToScreenPos(pos).x - radius < 0 || ConvWorldPosToScreenPos(pos).x + radius > ScreenSizeX ||
		ConvWorldPosToScreenPos(pos).y - radius < 0 || ConvWorldPosToScreenPos(pos).y + radius > ScreenSizeY)
	{
		return true;
	}
	return false;
}
