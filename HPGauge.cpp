#include "HPGauge.h"
#include "SystemConstant.h"
HPGauge::HPGauge(float maxHP, float DamagePoint)
{
	this->maxHP = maxHP;
	damagePoint = DamagePoint;
}

void HPGauge::Initialize(float maxHP)
{
	this->maxHP = maxHP;
	color = White;
	alpha = 255.0f;
}

void HPGauge::Update(VECTOR& pos, float& HP, float deltaTime)
{
	screenPos = ConvWorldPosToScreenPos(pos);
	screenPos = VAdd(screenPos, VGet(0.0f, -50.0f, 0.0f));
	float max = screenPos.x + Width / 2;
	float min = screenPos.x - Width / 2;
	HPRatio = HP / maxHP;
	valiableWidth = min + (max - min) * HPRatio;
	float a = 1 / maxHP;
	
	if (HP <= damagePoint)
	{
		color = Red;
		alpha += addition;
		if (alpha > 255)
		{
			addition *= -1;
		}
		if (alpha < 0)
		{
			addition *= -1;
		}

	}
}
void HPGauge::Draw()
{

	if (HPRatio > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBoxAA(screenPos.x - Width / 2, screenPos.y - Hight / 2, valiableWidth, screenPos.y + Hight / 2, color, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	DrawBoxAA(screenPos.x - Width / 2, screenPos.y - Hight / 2, screenPos.x + Width / 2, screenPos.y + Hight / 2, White, false);// ÉQÅ[ÉWòg

}
