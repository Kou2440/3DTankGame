#include "PlayerBody.h"
#include "AssetManager.h"
#include "SystemConstant.h"
#include "EffectManager.h"
#include "BreakExplosion.h"
#include "ObjectManager.h"
PlayerBody::PlayerBody(VECTOR initPos, VECTOR initDir, int inputState, ObjectTag myTag, const char* failName) :
	ObjectBase(myTag)
	, rotateNow(false)
	, accel()
{
	// 砲を生成.
	cannon = new PlayerCannon(initPos, initDir, inputState, myTag, failName);
	// HPゲージを生成.
	////hpGauge = new //hpGauge(HP, DamagePoint);

	// アセットマネージャーからモデルをロード.
	string str = "playerBody.mv1";
	modelHandle = AssetManager::GetMesh(failName + str);
	if (modelHandle == -1)
	{
		printfDx("playerBodyを読み込めません");
	}
	MV1SetScale(modelHandle, MoveModelScale);

	// 位置・方向を初期化.
	this->initPos = initPos;
	this->initDir = initDir;
	Initialize();

	// 当たり判定球セット.
	colModel = modelHandle;

	colType = CollisionType::Sphere;
	colSphere.worldCenter = pos;
	colSphere.radius = ColRadius;


	// 変数の初期化.
	padInput = inputState;
	winNum = 0;
	nameTag = myTag;
}

void PlayerBody::Initialize()
{
	// 値の初期化.
	pos = initPos;
	prevPos = pos;
	dir = initDir;
	aimDir = dir;
	velocity = InitVec;
	HP = MaxHP;

	// オブジェクトの初期化.
	cannon->Initialize(pos, dir);
	//hpGauge->Initialize(HP);
	breakEffect = nullptr;
	// 変更の反映.
	MV1SetPosition(modelHandle, pos);
	MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

PlayerBody::~PlayerBody()
{
	AssetManager::DeleteMesh(modelHandle);
}

void PlayerBody::Update(float deltaTime)
{
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
		Input(deltaTime);
	}

	// 方向ベクトルに加速力を加えて加速ベクトルとする.
	velocity = VScale(dir, accel);

	// 上下方向にいかないようにvelocityを整える.
	velocity = VGet(velocity.x, 0, velocity.z);

	// 予測ポジション更新.
	prevPos = VAdd(pos, VScale(velocity, deltaTime));

	// 画面外判定.
	if (offscreenDicision(prevPos, colSphere.radius))
	{
		velocity = InitVec;
		prevPos = pos;
	}
	CollisionUpdate(prevPos);
	
	
	// ポジション更新.
	pos = prevPos;
	cannon->Updateeeee(pos, deltaTime);
	 //hpGauge->Update(pos, HP, deltaTime);
	// 3Dモデルのポジション設定.
	MV1SetPosition(modelHandle, pos);
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI_F / 180.0f));
	VECTOR negativeVec = VTransform(dir, rotYMat);
	MV1SetRotationZYAxis(modelHandle, negativeVec, VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void PlayerBody::Draw()
{
	MV1DrawModel(modelHandle);
	//DrawFormatString(0, 0, Green, "%f %f", pos.x, pos.z);
	cannon->Draw();
	//hpGauge->Draw();
	//DrawCollider();
}

void PlayerBody::OnCollisionEnter(const ObjectBase* other)
{
	ObjectTag tag = other->GetTag();
	if (tag == ObjectTag::BackGround)
	{

		int colModel = other->GetCollisionModel();

		MV1_COLL_RESULT_POLY_DIM colInfo;
		bool hit = false;
		colSphere.worldCenter = colSphere.worldCenter + dir * 20;
		if (CollisionPair(colSphere, colModel, colInfo))
		{
			hit = true;
		}
		else 
		{
			colSphere.worldCenter = this->colSphere.worldCenter + dir * -20;

			if (CollisionPair(colSphere, colModel, colInfo))
			{
				hit = true;
			}
		}
		if (hit)
		{
			// 当たっている場合は押し量を計算.
			VECTOR poshBuckVec = CalcSpherePushBackVecFromMesh(colSphere, colInfo);
			pos = VAdd(pos, poshBuckVec);

			// コリジョン情報の解放.
			MV1CollResultPolyDimTerminate(colInfo);

			if (accel > 0)
			{
				accel = 0;
			}

			velocity = InitVec;
			CollisionUpdate();
		}
	}
	if (tag == ObjectTag::Bullet)
	{
		Sphere colSphere = other->GetCollisionSphere();
		MV1_COLL_RESULT_POLY_DIM colInfo;
		if (HP > 0)
		{
			if (CollisionPair(colSphere, modelHandle, colInfo))
			{
				HP -= DamagePoint;
				MV1CollResultPolyDimTerminate(colInfo);
			}
		}
		
	}
	
	
}



void PlayerBody::Input(float deltaTime)
{
	// キー入力取得.
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	GetJoypadXInputState(padInput, &pad);
	cannon->Input(deltaTime, pad);
	// 加速処理.
	if (accel <= MaxSpeed)
	{
		// 上を押していたら加速.
		if (CheckHitKey(KEY_INPUT_UP))
		{
			accel += Accel;
		}
		if (pad.RightTrigger != 0)
		{
			accel += Accel;
		}
	}
	if (accel >= MinSpeed)
	{
		//下を押していたら減速.
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			accel -= Back;
		}
		if (pad.LeftTrigger)
		{
			accel -= Back;
		}
	}
	// 旋回処理.
	if (CheckHitKey(KEY_INPUT_RIGHT))// 右旋回.
	{
		VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(right, TurnPerformance * deltaTime));
	}
	else if (CheckHitKey(KEY_INPUT_LEFT))// 左旋回.
	{
		VECTOR left = VCross(VGet(0.0f, -1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(left, TurnPerformance * deltaTime));
	}
#if 1
	if (pad.ThumbLX > 0)// 右旋回.
	{
		VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(right, TurnPerformance * deltaTime));
	}
	else if (pad.ThumbLX < 0)// 左旋回.
	{
		VECTOR left = VCross(VGet(0.0f, -1.0f, 0.0f), dir);
		dir = VAdd(dir, VScale(left, TurnPerformance * deltaTime));
	}
#else
	if (pad.ThumbLX || pad.ThumbLY)
	{
		aimDir = VGet(pad.ThumbLY, 0.0f, pad.ThumbLX);
		aimDir = VNorm(aimDir);
	}
#endif
	// 自然停止.
	if (!(CheckHitKey(KEY_INPUT_UP)) && !(CheckHitKey(KEY_INPUT_DOWN)) && pad.LeftTrigger - pad.RightTrigger == 0)
	{
		accel *= DefaultDecel;
		if (abs(VSize(velocity)) <= Epsilon)
		{
			accel = 0;
		}
	}

	// 正規化.
	dir = VNorm(dir);

	
	
}

void PlayerBody::Rotate(float deltaTIme)
{
	if (rotateNow)
	{
		if (IsNearAngle(aimDir,dir))
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
