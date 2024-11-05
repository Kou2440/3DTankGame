#pragma once
#include "DxLib.h"
#include <vector>
#include <map>
#include <string>

class MapManager
{
public:
	MapManager(int mapNum);
private:
	const float objLen = 100.0f;//オブジェクトの距離

	const float adjustCylinder = 50.0f;// 円柱オブジェクトの調整.
	const VECTOR moveBlockDirHorizon = VGet(1.0f, 0.0f, 0.0f);// 水平方向.
	const VECTOR moveBlockDirVirtical = VGet(0.0f, 0.0f, 1.0f);// 垂直方向.
	std::vector<VECTOR>spawnPos;
	std::vector<VECTOR>enemySpawnPos;
	int enemySpawnNum;// 敵スポーン数.
	const VECTOR LookRight = VGet(-1.0f, 0.0f, 0.0f);
	const VECTOR LookLeft = VGet(1.0f, 0.0f, 0.0f);
};
static const int mapObjectNumX = 19;
static const int mapObjectNumZ = 10;

static const std::string Block = "1";// ブロックの位置番号.
static const std::string Cylinder = "2";// 円柱マップオブジェクトの位置番号.
static const std::string MoveBlockVirtical = "3";// 水平に動くブロックマップオブジェクトの位置番号.
static const std::string MoveBlockHolizon = "4";// 水平に動くブロックマップオブジェクトの位置番号.
static const std::string PlayerSpawnPos = "P";// プレイヤースポーン位置番号.
static const std::string EnemySpawnPos = "E";// 敵スポーン位置番号.
static const std::vector<std::string>MapDataPath = 
{ "map0.csv","map1.scv","map2.scv","map3.csv"};

