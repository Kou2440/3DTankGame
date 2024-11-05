#pragma once
#include "DxLib.h"
#include "ObjectTag.h"
#include "Collision.h"
#include	 "CollisionType.h"

class ObjectBase
{
public:
	ObjectBase();
	ObjectBase(ObjectTag tag);  // ゲームオブジェクトコンストラクタ
	ObjectBase(ObjectTag tag, VECTOR pos);  // ゲームオブジェクトコンストラクタ

	virtual ~ObjectBase(); // デストラクタ

	const int GetModelHandle() const { return modelHandle; }

	const VECTOR& GetPos() const { return pos; }// 位置取得 

	const VECTOR& GetDir() const { return dir; }// 方向取得.

	bool GetVisible() const { return visible; }// 可視状態の取得

	bool GetAlive() const { return alive; }// ゲームオブジェクト生きているか 

	float GetHP() const { return HP; }// 装甲値の取得.

	virtual void Update(float deltaTime) = 0;  // ゲームオブジェクト更新処理
	virtual void Draw() {};// ゲームオブジェクト描画
	ObjectTag GetTag() const { return tag; } // ゲームオブジェクトの種類取得

	virtual void OnCollisionEnter(const ObjectBase* other) {};// このオブジェクトが持っている当たり判定を取得 ObjectManager::Collision.
	virtual void Collition(ObjectBase* other) {};// このオブジェクトとの当たり判定.
	CollisionType GetCollisionType()const { return colType; }// このオブジェクトが持っている当たり判定種を取得.
	LineSegment GetCollisionLine() const { return colLine; }// 当たり判定Lineを返却（持っていれば）.
	Sphere GetCollisionSphere() const { return colSphere; }// 当たり判定球を返却（持っていれば）.
	Capsule GetCollisionCapsule() const { return colCapsule; }// 当たり判定カプセルを返却（持っていれば）.
	int GetCollisionModel() const { return colModel; }// 当たり判定モデルを返却（持っていれば).

protected:
	void SetPos(const VECTOR set) { pos = set; }// 位置セット

	void SetVisible(bool set) { visible = set; }// 可視状態のセット

	void SetAlive(bool setFlag) { alive = setFlag; }// ゲームオブジェクト生死セット

	void SetHP(const float set) { HP = set; }// 装甲値のセット.
	void OnDamage(const float set) { HP -= set; };// ダメージを受けたとき.

	void DrawCollider();
	void CollisionUpdate(VECTOR pos);
	void CollisionUpdate();
	ObjectTag tag;// 登録名(登録票).
	VECTOR pos;// 位置ベクトル.
	VECTOR prevPos;// 予測位置ベクトル.
	VECTOR dir;// 方向ベクトル.
	int modelHandle;// モデルハンドル.
	bool visible;// 表示するかどうかのハンドル（何もしなければ表示）.
	bool alive;// 生きているかどうかのハンドル.
	float HP;// 残りの体力(HP).

	// 当たり判定関連.
	CollisionType colType;// 当たり判定タイプ.
	LineSegment colLine;// 当たり判定Line.
	Sphere colSphere;// 当たり判定球.
	Capsule colCapsule;// 当たり判定カプセル.
	int colModel;// 当たり判定用モデル（表示モデルと別に簡易当たり判定モデルをセット）
};

