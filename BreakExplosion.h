#pragma once
#include "EffectBase.h"
class BreakExplosion :
    public EffectBase
{
public:
    BreakExplosion(VECTOR pos, VECTOR dir);
    ~BreakExplosion();
    void Update(float deltaTime);
    bool finisht() { return finish; }
    void Play();
private:
    const float Size = 8.0f;
    const float PlayEffectSpeed = 1.0f;
    bool finish;
};

