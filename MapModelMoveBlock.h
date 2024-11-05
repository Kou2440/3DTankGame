#pragma once
#include "ObjectBase.h"
class MapModelMoveBlock :
    public ObjectBase
{
public:
    MapModelMoveBlock(VECTOR initPos, VECTOR moveDir);
    ~MapModelMoveBlock();
    void Update(float deltaTime);
    void Draw();
private:
    const VECTOR setScale = VGet(1.0f, 0.2f, 1.0f);

    const float adjust = 50.0f;
    const float moveSpeed = 80.0f;

    VECTOR velocity;
    const float waitTime = 2.0f;
    float deltaWaitTime;// ‘Ò‹@ŽžŠÔ.
};

