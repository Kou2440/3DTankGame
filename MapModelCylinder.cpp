#include "MapModelCylinder.h"
#include "AssetManager.h"

MapModelCylinder::MapModelCylinder(VECTOR initPos):
	ObjectBase(ObjectTag::BackGround)
{
	modelHandle = AssetManager::GetMesh("data/map/cylinder.mv1");
	MV1SetScale(modelHandle, setScale);// ÉTÉCÉYÇ100*100Ç…Ç∑ÇÈ.
	colModel = modelHandle;
	pos = initPos;
	MV1SetPosition(modelHandle, initPos);
	CollisionUpdate();
}

MapModelCylinder::~MapModelCylinder()
{
	AssetManager::DeleteMesh(modelHandle);
}

void MapModelCylinder::Update(float deltaTime)
{
	// èàóùÇ»Çµ.
}

void MapModelCylinder::Draw()
{
	MV1DrawModel(modelHandle);
}
