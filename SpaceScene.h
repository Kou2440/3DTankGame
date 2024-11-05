#pragma once
#include "SceneBase.h"
class SpaceScene :
    public SceneBase
{
public :
    SpaceScene(int stage);
    ~SpaceScene();
    SceneBase* Update(float deltaTime);
    void Draw();
private:
    int fontHandle;
    const int fontSize = 65;// �傫
    const int fontThick = 4;// ����.

    const float WaitTime = 2.0f;
    float Timer;
    int stageNum;
};

