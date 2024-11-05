#pragma once
#include "ObjectBase.h"

class MapModelBlock :
	public ObjectBase
{
public:
	MapModelBlock(VECTOR initPos);
	~MapModelBlock();

	void Update(float deltaTime);
	void Draw();

private:
	const VECTOR setScale = VGet(1.0f, 0.2f, 1.0f);


};

