#pragma once
#include "EnemyCannon.h"
class EffectBase;
class EnemyBody :
    public ObjectBase
{
public:
    EnemyBody(VECTOR initPos, VECTOR initDir, int enemyNo);
    void Initialize();
    void Update(float deltaTime);
    void Draw();
    void OnCollisionEnter(const ObjectBase* other) override;
private:
    void Behavioral1(float deltaTime);
    void Behavioral2(float deltaTime);
    void Begavioral3(float deltaTime);
    void Rotate(float deltaTIme);
    EnemyCannon* cannon;
    const float ColRadius = 32.0f;// 当たり判定半径.
    VECTOR initPos;
    VECTOR initDir;
    float MaxHP = 1;
    VECTOR velocity;
    float accel;
    const float Accel = 6.0f;// 通常の加速.
    const float Back = 5.0f;// 後退速度.
    const float MaxSpeed = 300.0f;// 最高前進速度.
    const float MinSpeed = -200.0f;// 最高後退速度.
    const float DefaultDecel = 0.97f;// なにもしない時の減速.
    bool rotateNow;
    VECTOR aimDir;
    const float Omega = 30.0f;// 旋回速度(degree).
    float rollingDegree = 40.0f;
    EffectBase* breakEffect;
    int number;
    bool hitBgObj;

};

