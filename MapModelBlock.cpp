#include "MapModelBlock.h"
#include "AssetManager.h"

MapModelBlock::MapModelBlock(VECTOR initPos) :
	ObjectBase(ObjectTag::BackGround)
{

	modelHandle = AssetManager::GetMesh("data/map/block.mv1");
	MV1SetScale(modelHandle, setScale);// ÉTÉCÉYÇ100*100Ç…Ç∑ÇÈ.
	colModel = modelHandle;
	pos = initPos;
	MV1SetPosition(modelHandle, pos);
	CollisionUpdate();
}

MapModelBlock::~MapModelBlock()
{
	AssetManager::DeleteMesh(modelHandle);
}

void MapModelBlock::Update(float deltaTime)
{
	// èàóùÇ»Çµ.
}

void MapModelBlock::Draw()
{
	MV1DrawModel(modelHandle);
	DrawCollider();
}
