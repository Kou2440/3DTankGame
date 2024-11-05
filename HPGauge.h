#pragma once
#include "DxLib.h"

class HPGauge
{
public:
	HPGauge(float maxHP, float DamagePoint);
	void Initialize(float maxHP);
	void Update(VECTOR& pos, float& HP, float deltaTime);
	void Draw();
private:
	VECTOR screenPos;
	float maxHP;
	float HPRatio;// HP割合.
	const float Width = 40.0f;// HPバーの幅.
	const float Hight = 10.0f;;// HPバーの高さ.
	float valiableWidth;// HPバーの可変長.
	int color;
	float damagePoint;
	int alpha;
	float addition = 3.0f;
};

