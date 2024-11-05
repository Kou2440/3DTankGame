#include "ObjectBase.h"
#include "AssetManager.h"


ObjectBase::ObjectBase()
	: pos()
	,prevPos()
	, dir()
	, modelHandle(-1)
	, visible(true)
	, alive(true)
{
}

ObjectBase::ObjectBase(ObjectTag tag)
	: tag(tag)
	, pos()
	, prevPos()
	, dir()
	, modelHandle(-1)
	, visible(true)
	, alive(true)
{

}

ObjectBase::ObjectBase(ObjectTag tag, VECTOR pos)
	: tag(tag)
	, pos()
	, prevPos()
	, dir()
	, modelHandle(-1)
	, alive(true)
	, visible(true)
{
	
	this->pos = pos;
}

ObjectBase::~ObjectBase()
{
	if (modelHandle != -1)
	{
		AssetManager::DeleteMesh(modelHandle);
		modelHandle = -1;
	}
}



//--------------------------------------------------------------------
// @brief コライダーの描画.
//--------------------------------------------------------------------
void ObjectBase::DrawCollider()
{

	// 全コライダー種の描画.
	DrawLine3D(colLine.worldStart, colLine.worldEnd, GetColor(255, 255, FALSE));
	DrawSphere3D(colSphere.worldCenter, colSphere.radius, 10, GetColor(0, 255, 255), GetColor(0, 0, 0), FALSE);
	DrawCapsule3D(colCapsule.worldStart, colCapsule.worldEnd, colCapsule.radius, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), FALSE);
	
	// 子リジョンモデルが存在する場合は半透明描画.
	if (colModel != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		MV1DrawModel(colModel);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

//---------------------------------------------------------------------------------
// @brief  当たり判定更新処理
// @detail GameObjectが移動したときには当たり判定位置もそれに合わせて更新する必要がある。
// 当たり判定がモデル(メッシュ)の場合、ポリゴンごとの当たり判定情報を再構築するため、
// 呼び出し回数を少なくする工夫が必要。その他の球・線分・カプセルは問題なく、毎フレーム
// 呼び出しても問題ない。
//---------------------------------------------------------------------------------
void ObjectBase::CollisionUpdate(VECTOR pos)
{
	colSphere.Move(pos);
	colLine.Move(pos);
	colCapsule.Move(pos);

	// 当たり判定情報を再構築.
	if (colModel != -1)
	{
		MV1SetPosition(colModel, pos);
		MV1SetupCollInfo(colModel);
	}
}

void ObjectBase::CollisionUpdate()
{
	colSphere.Move(pos);
	colLine.Move(pos);
	colCapsule.Move(pos);

	// 当たり判定情報を再構築.
	if (colModel != -1)
	{
		MV1SetPosition(colModel, pos);
		MV1SetupCollInfo(colModel);
	}
}
