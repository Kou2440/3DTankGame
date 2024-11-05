#include "EffectBase.h"

EffectBase::EffectBase() :
	handle(-1)
	,valid(true)
	,isPlayNow(true)
{
}

void EffectBase::DeletionPermission()
{
	
	valid = false;
}

