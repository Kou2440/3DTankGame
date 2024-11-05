#pragma once
#include "ObjectBase.h"

class MapModelCylinder :
	public ObjectBase
{
public:
	MapModelCylinder(VECTOR initPos);
	~MapModelCylinder();

	void Update(float deltaTime);
	void Draw();


private:
	const VECTOR setScale = VGet(0.24f, 0.3f, 0.24f);


};

