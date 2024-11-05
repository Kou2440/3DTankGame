#pragma once
#include <string>
#include "ObjectBase.h"
#include "PlayerBody.h"

using namespace std;
class UI 
{
public:
	UI();
	~UI();
	void Draw(float deltaTime);
	void Winner(const PlayerBody* player);
private:
	void Debug();
	void DrawStringInTheCenter(string str);
	
};