#pragma once
#include "SystemConstant.h"
#include "EffectBase.h"

class MazzleFlashEffect :
    public EffectBase
{
public:
    MazzleFlashEffect(VECTOR pos, VECTOR dir);
    ~MazzleFlashEffect();
    void Update(float deltaTime);
    void Play();
private:
    
    const float Size = 2.0f;
    const float BarrelHeadLen = 100.0f;// �C�g��[(����).
    const float BarrelHeadHeight = 13.0f;// �C�g��[(����).
    const float PlayEffectSpeed = 30.0f;
};

