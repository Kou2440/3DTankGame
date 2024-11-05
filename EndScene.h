#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <string>
class EndScene :
    public SceneBase
{
public:
    EndScene(std::string comment);
    ~EndScene();
    SceneBase* Update(float deltaTime);
    void Draw();

private:
    float alpha;
    float addition = 1;
    int winnerNum;
    int fontHandle;
    const int fontSize = 60;// ‘å‚«‚³.
    const int fontThick = 3;// ‘¾‚³.
    const std::string Str = "Rematch with any key or pad any button";
    const std::string Str2 = "Exit with Esc button";
    const int str2Pos = 50;
    int WaitTime = 300;
    std::string winOrlose;
};

