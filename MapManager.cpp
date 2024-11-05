#include "MapManager.h"
#include "ObjectManager.h"
#include "MapModelBlock.h"
#include "MapModelCylinder.h"
#include "MapModelMoveBlock.h"
#include <fstream>
#include <sstream>
#include "EnemyBody.h"
#include "PlayerBody.h"
using namespace std;

MapManager::MapManager(int mapNum)
{

	vector<ObjectBase*> obj;

	struct MAPOBJECT
	{
		string objectNum;
		ObjectBase* mapObj;
	};
	const string Path1 = "data/map/map";
	const string Extension = ".csv";
	char mapNumber = mapNum + '0';// int型からchar型に変換.
	ifstream file{ Path1 + mapNumber + Extension ,ios::in };// 読み込みのみ.
	string s;
	int row = 0;
	while (getline(file, s, '\n'))
	{
		int column = 0;
		istringstream istm(s);
		string num;
		while (getline(istm, num, ','))
		{
			float x = WindowSizeXMin + objLen * column;
			float z = WindowSizeZMax - objLen * row;
			// プレイヤーの生成.
			if (num == PlayerSpawnPos)
			{
				
				int padInput = DX_INPUT_PAD1;
				PlayerBody* player = new PlayerBody(VGet(x, 0, z), LookRight*-1, padInput, ObjectTag::Player1, "data/player1/");
				ObjectManager::Entry(player);
			}
			// 敵の生成.
			else if (num[0] == EnemySpawnPos[0])
			{
				
				for (int i = 0; i < 10; ++i)
				{
					if (num[1] == '1' + i)
					{
						EnemyBody* enemy = new EnemyBody(VGet(x, 0, z), LookLeft, i + 1);
						ObjectManager::Entry(enemy);
					}
				}
				
				
			}
			// マップオブジェクトの生成.
			else
			{
				MAPOBJECT mapObj[] =
				{
					{Block, new MapModelBlock(VGet(x, 0, z)) },
					{Cylinder, new MapModelCylinder(VGet(x + adjustCylinder, 0, z - adjustCylinder))},
					{MoveBlockVirtical, new MapModelMoveBlock(VGet(x, 0.0f, z), moveBlockDirVirtical)},
					{MoveBlockHolizon,new MapModelMoveBlock(VGet(x, 0.0f, z), moveBlockDirHorizon)}
				};
				for (int l = 0; l < sizeof mapObj / sizeof mapObj[0]; ++l)
				{
					if (num == mapObj[l].objectNum)
					{
						obj.push_back(mapObj[l].mapObj);
					}
				}
			}
			column++;// 列の加算.
		}
		row++;//行の加算.
	}
	
	

	// オブジェクトマネージャーに登録.
	for (auto& i : obj)
	{
		ObjectManager::Entry(i);
	}

	enemySpawnNum = enemySpawnPos.size();
}

