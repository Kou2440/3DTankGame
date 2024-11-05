#include "Bullet.h"
#include "ObjectManager.h"	
#include "AssetManager.h"


Bullet::Bullet(VECTOR pos, VECTOR dir, ObjectTag userTag) :
	ObjectBase(ObjectTag::Bullet)
{
	// アセットマネージャーからモデルをロード.
	modelHandle = AssetManager::GetMesh("data/beam.mv1");
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.08f));// サイズの変更.
	// 位置・方向を初期化.
	this->pos = pos;
	this->dir = dir;
	this->pos = VAdd(this->pos ,VScale(this->dir, BarrelHead));// 砲塔先頭にセットするため.

	MV1SetPosition(modelHandle, this->pos);
	MV1SetRotationZYAxis(modelHandle, this->dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

	// 当たり判定球セット.
	colType = CollisionType::Sphere;
	colSphere.worldCenter = pos;
	colSphere.worldCenter.y = 0.0f;
	colSphere.radius = ColRadius;
	CollisionUpdate();
	// 変数の初期化.
	velocity = InitVec;
	reflectionFlag = false;
	permitUpdate = true;
 	myTag = userTag;


}

Bullet::Bullet(ObjectTag userTag) : 
	ObjectBase(ObjectTag::Bullet)
{
	// アセットマネージャーからモデルをロード.
	modelHandle = AssetManager::GetMesh("data/beam.mv1");
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.08f));// サイズの変更.
	myTag = userTag;
	permitUpdate = false;
	visible = false;
	// 当たり判定球セット.
	colType = CollisionType::Sphere;
	colSphere.radius = ColRadius;
	Init();

}

Bullet::~Bullet()
{
	AssetManager::DeleteMesh(modelHandle);

}

void Bullet::Init()
{

	// 位置・方向を初期化.
	this->pos = InitVec;
	this->dir = InitVec;
	velocity = InitVec;
	reflectionFlag = false;
	permitUpdate = false;
	colSphere.worldCenter = pos;
	visible = false;
	CollisionUpdate();
	MV1SetPosition(modelHandle, this->pos);
	MV1SetRotationZYAxis(modelHandle, this->dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);



}

void Bullet::Generate(VECTOR pos, VECTOR dir)
{

	this->pos = pos;
	this->dir = dir;
	this->pos = VAdd(this->pos, VScale(this->dir, BarrelHead));// 砲塔先頭にセットするため.
	colSphere.worldCenter = pos;
	permitUpdate = true;
	visible = true;
	CollisionUpdate();
	MV1SetPosition(modelHandle, this->pos);
	MV1SetRotationZYAxis(modelHandle, this->dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);


}

void Bullet::Update(float deltaTime)
{
	if (permitUpdate)
	{
		velocity = VScale(VScale(dir, BulletSpeed), deltaTime);
		prevPos = VAdd(pos, velocity);

		if (offscreenDicision(pos))
		{
			SetVisible(false);
			alive = false;
		}

		CollisionUpdate(prevPos);

		pos = prevPos;
		// 位置の更新.
		MV1SetPosition(modelHandle, pos);
		MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI_F / 180.0f));
		VECTOR negativeVec = VTransform(dir, rotYMat);
		MV1SetRotationZYAxis(modelHandle, negativeVec, VGet(0.0f, 1.0f, 0.0f), 0.0f);

	}

}


void Bullet::Draw()
{
	MV1DrawModel(modelHandle);
	//DrawCollider();
	
}

void Bullet::OnCollisionEnter(const ObjectBase* other)
{
	ObjectTag tag = other->GetTag();
	if (tag == ObjectTag::BackGround)
	{
		int colModel = other->GetCollisionModel();

		MV1_COLL_RESULT_POLY_DIM colInfo;
		if (CollisionPair(colSphere, colModel, colInfo))
		{
			if (reflectionFlag)
			{
				SetAlive(false);
			}
			else
			{
				// 当たっている場合は押し量を計算.
 				VECTOR poshBuckVec = CalcSpherePushBackVecFromMesh(colSphere, colInfo);
				prevPos = VAdd(pos, poshBuckVec);

				VECTOR planeNormal;                    // ポリゴン平面法線
				for (int i = 0; i < colInfo.HitNum; ++i)
				{
					// 衝突ポリゴンの辺 
					VECTOR edge1, edge2;
					edge1 = colInfo.Dim[i].Position[1] - colInfo.Dim[i].Position[0];
					edge2 = colInfo.Dim[i].Position[2] - colInfo.Dim[i].Position[0];
					// 衝突ポリゴンの辺より、ポリゴン面の法線ベクトルを求める
					planeNormal = VCross(edge1, edge2);
					planeNormal = VNorm(planeNormal);

					// ちゃんと値が取れていればループから出る.
					if (planeNormal.x == 1 || planeNormal.z == 1)
					{
						break;
					}
				}
				// 反射の公式r = f + 2 ( -dot(f, n) * n.
				float a = VDot(VScale(velocity, -1.0f), planeNormal);
				VECTOR b = VScale(planeNormal, 2.0f * a);
				dir = VAdd(velocity, b);
				dir = VNorm(dir);

				// コリジョン情報の解放.
				MV1CollResultPolyDimTerminate(colInfo);
				colSphere.worldCenter = prevPos;
				reflectionFlag = true;
			}
		}
	}
	if (tag == ObjectTag::Player1 || tag == ObjectTag::Enemy)
	{

		int colModel = other->GetCollisionModel();
		MV1_COLL_RESULT_POLY_DIM colInfo;
		colSphere.worldCenter.y = 0.0f;
		if (CollisionPair(colSphere, colModel, colInfo))
		{
			alive = false;
			MV1CollResultPolyDimTerminate(colInfo);
		}
	}
	

}