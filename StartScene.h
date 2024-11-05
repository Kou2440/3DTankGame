#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <string>
class StartScene :
    public SceneBase
{
public:
    StartScene();
    SceneBase* Update(float deltaTime);
    void Draw();
private:
    float alpha;
    float addition = 1;
    const std::string Str = "Start with any key or pad start button";
    const int StrLne = strlen(Str.c_str());
    const int StrWidth = GetDrawStringWidth(Str.c_str(), StrLne);
};

