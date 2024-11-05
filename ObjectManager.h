#pragma once
#include <unordered_map>

#include "ObjectBase.h"
#include "ObjectTag.h"
#include "Math.h"
#include "SystemConstant.h"

class ObjectManager final
{
public:
	static void Initialize();

	static void Entry(ObjectBase* newObj);
	static void Relese(ObjectBase* releseObj);
	static void ReleseAllObj();

	static void Update(float deltaTime);
	static void Draw();

	static void Collition();

	static size_t TagObjectSize(ObjectTag tag);
	static ObjectBase* GetFirstObject(ObjectTag tag);
	static ObjectBase* GetAllObjectsInTag(ObjectTag tag, int index);
	static void Finalize();

private:
	ObjectManager();
	~ObjectManager();
	static ObjectManager* Instance;
	std::vector<ObjectBase*> pendingObjects;// ˆêŽž•Û‘¶.
	std::unordered_map<ObjectTag, std::vector<ObjectBase*>> objects;
};

