#pragma once
#include "SceneBase.h"

#include "camera.h"
#include "PlayerBody.h"
#include "EnemyBody.h"
#include "MapManager.h"
#include <string>
#include "SystemConstant.h"
class PlayScene :
    public SceneBase
{
public:
    PlayScene(int mapNum);
    //PlayScene(bool mode, int mapNum);
    ~PlayScene();
    SceneBase* Update(float deltaTime);
    void Draw();
private:
    const VECTOR LookLeft = VGet(-1.0f, 0.0f, 0.0f);
    const VECTOR LookRight = VGet(1.0f, 0.0f, 0.0f);
    int imgHandle;
    const float OnShootingDownWaitTime = 30.0f;// 被撃墜時待機時間.
    const float WaitingTimeBeforStart = 4.0f;// 開始前待機時間.
    float deltaWaitTime;// 待機時間.
    int winnerNum;
    const float WaitingTimeToSwitchToTheNextScene = 0.85f;//次のシーンに切り替わるまでの待機時間.
    float TimerOfWaitingTimeNextScene;
    
    std::string str;
    const float StringDrawTime = 2.0f;// 文字を描画しておく時間.
    int fontHandle;
    const int fontSize = 60;// 大き
    const int fontThick = 3;// 太さ.
    std::string comment;
    int mapNum;
    
};

