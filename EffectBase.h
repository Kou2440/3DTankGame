#pragma once
#include "DxLib.h"
#include "EffekseerForDXLib.h"

class EffectBase
{
public:
	EffectBase();

	virtual void Update(float deltaTime) = 0;
	virtual void Play() {};

	bool GetValid() const { return valid; }
	bool IsPlayNow() const { return isPlayNow; }
	void DeletionPermission();
protected:
	int handle;
	bool valid;// 有効かどうか.
	bool isPlayNow;// 再生中かどうか.
	VECTOR pos;
	VECTOR dir;
	int playingEffectHandle;// 再生中のハンドル.



};

