#include "EnemyBody.h"
#include "AssetManager.h"
#include "ObjectTag.h"
#include "SystemConstant.h"
#include "ObjectManager.h"
#include "BreakExplosion.h"
#include "EffectManager.h"

EnemyBody::EnemyBody(VECTOR initPos, VECTOR initDir, int enemyNo):ObjectBase(ObjectTag::Enemy)
{
	modelHandle = AssetManager::GetMesh("data/enemy/enemyBody.mv1");
	if (modelHandle == -1)
	{
		printfDx("EnemyBodyを読み込めません");
	}
	MV1SetScale(modelHandle, MoveModelScale);

	// 位置・方向を初期化.
	this->initPos = initPos;
	this->initDir = initDir;
	Initialize();
	cannon = new EnemyCannon(pos, dir, ObjectTag::Enemy);
	
	// 当たり判定球セット.
	colModel = modelHandle;
	colType = CollisionType::Sphere;
	colSphere.worldCenter = pos;
	colSphere.radius = ColRadius;
	number = enemyNo;
}

void EnemyBody::Initialize()
{
	pos = initPos;
	dir = initDir;
	
	HP = MaxHP;
	velocity = InitVec;
	aimDir = dir;
	rotateNow = false;
	breakEffect = nullptr;
	hitBgObj = false;
	// 変更の反映.
	MV1SetPosition(modelHandle, pos);
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void EnemyBody::Update(float deltaTime)
{
	switch (number)
	{
	case 1:
		Behavioral1(deltaTime);
		break;
	case 2:
		Behavioral2(deltaTime);
		break;
	case 3:
		Begavioral3(deltaTime);
		break;
	default:
		Behavioral1(deltaTime);
		break;
	}
	
	Rotate(deltaTime);
	
	if (HP <= 0.0f)
	{
		if (breakEffect == nullptr)
		{
			breakEffect = new BreakExplosion(pos, dir);
			EffectManager::Entry(breakEffect);
			visible = false;
		}
		else if (!breakEffect->IsPlayNow())
		{
			SetAlive(false);
			breakEffect->DeletionPermission();
		}
	}
	else
	{
		cannon->Updateeeee(pos, deltaTime);
	}
	// 変更の反映.
	MV1SetPosition(modelHandle, pos);
	MV1SetRotationZYAxis(modelHandle, dir , VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void EnemyBody::Draw()
{
	MV1DrawModel(modelHandle);
	cannon->Draw();
	//DrawCollider();
}

void EnemyBody::Rotate(float deltaTIme)
{
	if (rotateNow)
	{
		if (IsNearAngle(aimDir, dir))
		{
			dir = aimDir;
			rotateNow = false;
		}
		else
		{
			//回転させる.
			VECTOR interPolateDir;
			interPolateDir = RotateForAimVecYAxis(dir, aimDir, Omega * deltaTIme);

			// 回転が目標角を超えていないか.
			VECTOR cross1, cross2;
			cross1 = VCross(dir, aimDir);
			cross2 = VCross(interPolateDir, aimDir);

			// 目標角度を超えたら終了.
			if (cross1.y * cross2.y < 0.0f)
			{
				interPolateDir = aimDir;
				rotateNow = false;
			}
			// 目標ベクトルに１０度だけ近づけた角度
			dir = interPolateDir;
		}
	}
}

void EnemyBody::OnCollisionEnter(const ObjectBase* other)
{
	ObjectTag tag = other->GetTag();
	// 背景
	if (tag == ObjectTag::BackGround)
	{
		int colModel = other->GetCollisionModel();

		MV1_COLL_RESULT_POLY_DIM colInfo;
		if (CollisionPair(colSphere, colModel, colInfo))
		{
			// 当たっている場合は押し量を計算.
			VECTOR poshBuckVec = CalcSpherePushBackVecFromMesh(colSphere, colInfo);
			pos = VAdd(pos, poshBuckVec);

			// コリジョン情報の解放.
			MV1CollResultPolyDimTerminate(colInfo);

			CollisionUpdate();
			hitBgObj = true;
		}
		else
		{
			hitBgObj = false;
		}
	}
	// 弾.
	if (tag == ObjectTag::Bullet)
	{
		Sphere colSphere = other->GetCollisionSphere();
		MV1_COLL_RESULT_POLY_DIM colInfo;
		if (CollisionPair(colSphere, modelHandle, colInfo))
		{
			HP = 0;
		}
	}


}
void EnemyBody::Behavioral1(float deltaTime)
{
	// 動かないため処理なし.
}
void EnemyBody::Behavioral2(float deltaTime)
{
	// 方向ベクトルに加速力を加えて加速ベクトルとする.
	velocity = VScale(dir * -1, accel);

	// 上下方向にいかないようにvelocityを整える.
	velocity = VGet(velocity.x, 0, velocity.z);

	// 予測ポジション更新.
	prevPos = VAdd(pos, VScale(velocity, deltaTime));

	// 画面外判定.
	if (offscreenDicision(prevPos, colSphere.radius))
	{
		if (!rotateNow)
		{
			
			aimDir *= -1;
			
			rotateNow = true;
		}

		velocity = InitVec;
		CollisionUpdate();
		prevPos = pos;
	}
	// 当たり判定球の位置更新.
	CollisionUpdate(prevPos);

	// ポジション更新.
	pos = prevPos;

	// 加速処理.
	if (accel <= MaxSpeed)
	{
		accel += Accel;
	
	}
	if (hitBgObj)
	{
		if (accel > 0)
		{
			accel = 0;
		}
		if (!rotateNow)
		{
			MATRIX rotYMat = MGetRotY(ToRadian(rollingDegree));
			aimDir = VTransform(aimDir, rotYMat);
			if (GetRand(2) == 0)
			{
				aimDir *= -1;
			}
			rotateNow = true;
		}

		velocity = InitVec;
	}
}

void EnemyBody::Begavioral3(float deltaTime)
{
	// 方向ベクトルに加速力を加えて加速ベクトルとする.
	velocity = VScale(dir * -1, accel);

	// 上下方向にいかないようにvelocityを整える.
	velocity = VGet(velocity.x, 0, velocity.z);

	// 予測ポジション更新.
	prevPos = VAdd(pos, VScale(velocity, deltaTime));

	// 画面外判定.
	if (offscreenDicision(prevPos, colSphere.radius))
	{
		accel = 0;
		if (!rotateNow)
		{
			aimDir *= -1;
			rotateNow = true;
		}

		velocity = InitVec;
		CollisionUpdate();
		prevPos = pos;
	}
	// 当たり判定球の位置更新.
	CollisionUpdate(prevPos);

	// ポジション更新.
	pos = prevPos;

	// 加速処理.

	if (accel <= MaxSpeed && !rotateNow)
	{
		accel += Accel;

	}
	if (hitBgObj)
	{
		if (accel > 0)
		{
			accel = 0;
		}
		if (!rotateNow)
		{
			MATRIX rotYMat = MGetRotY(ToRadian(rollingDegree));
			aimDir = VTransform(aimDir, rotYMat);
			if (GetRand(2) == 0)
			{
				aimDir *= -1;
			}
			rotateNow = true;
		}

		velocity = InitVec;
	}
}
