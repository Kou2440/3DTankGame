#pragma once

class SceneBase
{
public:
	SceneBase() {};
	virtual ~SceneBase() {};

	virtual SceneBase* Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};

