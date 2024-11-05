#pragma once
#include "DxLib.h"
#include "ObjectTag.h"
class Bullet;
class BulletManager
{
public:
	BulletManager(ObjectTag userTag);
	void Generate(VECTOR& pos, VECTOR& dir);
private:
	Bullet* bullet;
};

