#include "PlayerCannon.h"
#include "AssetManager.h"
#include "ObjectManager.h"
#include "EffectManager.h"
#include "SystemConstant.h"
#include "Bullet.h"
#include "MazzleFlashEffect.h"

using namespace std;
PlayerCannon::PlayerCannon(VECTOR initPos, VECTOR initDir, int inputState, ObjectTag userTag, const char* failName) :
	ObjectBase(userTag)
{
	// アセットマネージャーからモデルをロード.
	string str = "playerCannon.mv1";
	modelHandle = AssetManager::GetMesh(failName + str);
	MV1SetScale(modelHandle, MoveModelScale);
	
	// 位置・方向を初期化.
	Initialize(initPos, initDir);
	this->userTag = userTag;

	padInput = inputState;

	Bullet* bullet = new Bullet(pos, dir, ObjectTag::Enemy);
	ObjectManager::Entry(bullet);
	ObjectManager::Relese(bullet);
	EffectBase* mazzleFlash = new MazzleFlashEffect(pos, dir * -1);
	EffectManager::Entry(mazzleFlash);
	EffectManager::Relese(mazzleFlash);
}

void PlayerCannon::Initialize(VECTOR initPos, VECTOR initDir)
{
	// 値の初期化.
	pos = initPos;
	pos.y = AdjustPos;
	dir = initDir;
	aimDir = InitVec;
	rotateNow = false;
	shotTime = 0.0f;
	// 変更の反映.
	MV1SetPosition(modelHandle, pos);
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

PlayerCannon::~PlayerCannon()
{
	AssetManager::DeleteMesh(modelHandle);
}


void PlayerCannon::Update(float deltaTime)
{
	// 引数にdeltaTime以外のものも使いたいから不使用.
}
void PlayerCannon::Updateeeee(VECTOR bodyPos, float deltaTime)
{
	Rotate(deltaTime);
	dir = VNorm(dir);
	pos = bodyPos;

	MV1SetPosition(modelHandle, this->pos);
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI_F / 180.0f));
	VECTOR negativeVec = VTransform(dir, rotYMat);
	MV1SetRotationZYAxis(modelHandle, negativeVec, VGet(0.0f, 1.0f, 0.0f), 0.0f);



}
void PlayerCannon::Draw()
{
	MV1DrawModel(modelHandle);
	
}



void PlayerCannon::Input(float deltaTime, XINPUT_STATE pad)
{
	// キーボード入力.
	if (CheckHitKey(KEY_INPUT_A))// 右.
	{
		VECTOR left = VCross(VGet(0.0f, -1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(left, TurnPerformance * deltaTime));
	}
	if (CheckHitKey(KEY_INPUT_D))// 左.
	{
		VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(right, TurnPerformance * deltaTime));
	}

	VECTOR padVec = VGet(pad.ThumbRX, 0.0f, pad.ThumbRY);
	
	if (VectorSize(padVec) != 0.0f)
	{
		padVec = VNorm(padVec);
		if (IsNearAngle(padVec, dir))
		{
			dir = padVec;
		}
		else
		{
			rotateNow = true;
			aimDir = padVec;
		}
	}
	shotTime -= deltaTime;
	if (shotTime < 0 && (CheckHitKey(KEY_INPUT_SPACE) || pad.Buttons[9]))
	{
		shotTime = ShotIntervalTime;
		ObjectBase* bullet = new Bullet(pos, dir, userTag);
		ObjectManager::Entry(bullet);
		EffectBase* mazzleFlash = new MazzleFlashEffect(pos, dir);
		EffectManager::Entry(mazzleFlash);
	}


}

void PlayerCannon::Rotate(float deltaTime)
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
			interPolateDir = RotateForAimVecYAxis(dir, aimDir, Omega * deltaTime);

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


