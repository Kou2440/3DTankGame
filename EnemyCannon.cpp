#include "EnemyCannon.h"
#include "AssetManager.h"
#include "SystemConstant.h"
#include "ObjectManager.h"
#include "Bullet.h"
#include "MazzleFlashEffect.h"
#include "EffectManager.h"
#include "PlayerBody.h"
#include <cmath>

EnemyCannon::EnemyCannon(VECTOR initPos, VECTOR initDir, ObjectTag userTag) :
	ObjectBase(ObjectTag::Enemy)
{
	modelHandle = AssetManager::GetMesh("data/enemy/enemyCannon.mv1");
	if (modelHandle == -1)
	{
		printfDx("enemycannon == -1");
	}
	MV1SetScale(modelHandle, MoveModelScale);

	// 位置・方向を初期化.
	Initialize(initPos, initDir);
	this->userTag = userTag;

	colType = CollisionType::Line;
	colLine.worldStart = pos;
	colLine.worldEnd = InitVec;

	Bullet* bullet = new Bullet(pos, dir, ObjectTag::Enemy);
	ObjectManager::Entry(bullet);
	ObjectManager::Relese(bullet);
	EffectBase* mazzleFlash = new MazzleFlashEffect(pos, dir * -1);
	EffectManager::Entry(mazzleFlash);
	EffectManager::Relese(mazzleFlash);
}

void EnemyCannon::Initialize(VECTOR initPos, VECTOR initDir)
{
	// 値の初期化.
	pos = initPos;
	pos.y = AdjustPos;
	dir = initDir;
	aimDir = dir;
	rotateNow = false;
	shotTime = 0.0f;
	jbPlayerPos = InitVec;
	rotateAngle = Omega;
	// 変更の反映.
	MV1SetPosition(modelHandle, pos);
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

}




EnemyCannon::~EnemyCannon()
{
	AssetManager::DeleteMesh(modelHandle);
}

/// <summary>
/// 更新処理.
/// </summary>
/// <param name="bodyPos">車体位置.</param>
/// <param name="deltaTime"></param>
void EnemyCannon::Updateeeee(VECTOR bodyPos, float deltaTime)
{
	if (shotTime >= 0)
	{
		shotTime -= deltaTime;
	}
	
	DiviationValculation(deltaTime);
	dir = VNorm(dir);
	pos = bodyPos;
	Rotate(deltaTime);
	MV1SetPosition(modelHandle, this->pos);
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);
	
}

void EnemyCannon::Draw()
{
	MV1DrawModel(modelHandle);
	/*DrawLine3D(pos, pos + a * 100, Green);
	DrawLine3D(pos, pos + dir*-100, Red);
	
	DrawFormatString(0, 100, Green, "%f %f", leftOrRight);*/
}

/// <summary>
/// 旋回処理.
/// </summary>
/// <param name="deltaTime"></param>
void EnemyCannon::Rotate(float deltaTime)
{
	if (rotateNow)
	{
		if (IsNearAngle(aimDir, dir))
		{
			dir = aimDir;
			rotateNow = false;
			rotateAngle = Omega;
		}
		//回転させる.
		VECTOR interPolateDir;
		interPolateDir = RotateForAimVecYAxis(dir, aimDir, rotateAngle * deltaTime);

		// 回転が目標角を超えていないか.
		VECTOR cross1, cross2;
		cross1 = VCross(dir, aimDir);
		cross2 = VCross(interPolateDir, aimDir);

		// 目標角度を超えたら終了.
		if (cross1.y * cross2.y < 0.0f)
		{
			rotateAngle = rotateAngle / 3;
			/*interPolateDir = aimDir;
			rotateNow = false;*/
		}
		// 目標ベクトルにOmegaだけ近づけた角度
		dir = interPolateDir;

	}
}

/// <summary>
/// プレイヤーとの間に障害物がないか.
/// </summary>
/// <param name="playerPos">プレイヤー座標</param>
/// <returns>何もなければtrue</returns>
bool EnemyCannon::Search(VECTOR playerPos)
{
	colLine.localStart = pos;
	colLine.worldEnd = playerPos;
	for (int i = 0; i < ObjectManager::TagObjectSize(ObjectTag::BackGround); ++i)
	{
		ObjectBase* bgObj = ObjectManager::GetAllObjectsInTag(ObjectTag::BackGround, i);
		if (bgObj)
		{
			int colModel = bgObj->GetCollisionModel();
			MV1_COLL_RESULT_POLY colInfo;
			if (CollisionPair(colLine, colModel, colInfo))
			{
				return false;
			}
		}
	}
	// 射線にいないかどうか.
	if (ObjectManager::TagObjectSize(ObjectTag::Enemy) > 1)
	{
		for (int i = 0; i < ObjectManager::TagObjectSize(ObjectTag::Enemy); ++i)
		{
			ObjectBase* enemyObj = ObjectManager::GetAllObjectsInTag(ObjectTag::Enemy, i);
			if (enemyObj)
			{
				Sphere colSphere = enemyObj->GetCollisionSphere();
				if (!CollisionPair(colLine, colSphere))
				{
					leftOrRight = CalcRotationDirectionYAxis(dir, aimDir);
					Fire();
				}
			}
		}
	}
	else
	{
		leftOrRight = CalcRotationDirectionYAxis(dir, aimDir);
		Fire();
	}

	
	if (!IsNearAngle(aimDir, dir))
	{
		rotateNow = true;
	}
	return true;
}

/// <summary>
/// 偏差位置算出.
/// </summary>
/// <param name="deltaTime"></param>
void EnemyCannon::DiviationValculation(float deltaTime)
{
	ObjectBase* player = ObjectManager::GetFirstObject(ObjectTag::Player1);
	VECTOR nowPlayerPos = dir * -1;
	if (player)
	{
		nowPlayerPos = player->GetPos();
		if (Search(nowPlayerPos))
		{
			if (VDot(VNorm(jbPlayerPos),VNorm(nowPlayerPos)))
			{
				// プレイヤーと時期の位置から弾着時間を算出.
			float length = VSize(nowPlayerPos - pos);
			float time2Inpact = length / (BulletSpeed * deltaTime);

			//偏差位置の算出.
			VECTOR distanceTraveled = nowPlayerPos - jbPlayerPos;// 移動した距離.
			VECTOR deviationPos = nowPlayerPos + (distanceTraveled * time2Inpact);

			aimDir = VNorm(deviationPos - pos) * -1;
			}
			

		}

	}
	jbPlayerPos = nowPlayerPos;
}

void EnemyCannon::Fire()
{
	// 一定車閣内に入ったら撃つ.
	VECTOR enemyDir = VNorm(colLine.worldEnd - pos);
	MATRIX mat = MGetRotY(ToRadian(FOVDegree) * -leftOrRight);
	VECTOR rot = VTransform(dir, mat) * -1;
	float dot1 = VDot(dir, enemyDir);
	float dot2 = VDot(dir, rot);
	if(dot1 < dot2)
	{
		if (shotTime < 0)
		{
			shotTime = ShotIntervalTime;
			ObjectBase* bullet = new Bullet(pos, dir * -1, userTag);
			ObjectManager::Entry(bullet);
			EffectBase* mazzleFlash = new MazzleFlashEffect(pos, dir * -1);
			EffectManager::Entry(mazzleFlash);

		}
	}
	
}

