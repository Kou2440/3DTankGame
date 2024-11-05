#pragma once
#include "DxLib.h"
#include <string>
#include <unordered_map>
//#include <atlstr.h>
using namespace std;
class AssetManager final
{
public:
	static void Initalize();// 初期化.
	static int GetMesh(string meshFileName);// メッシュ（モデル）の取得.
	static void DeleteMesh(int meshID);
	static void DeleteAllAsset();
	// メッシュ（モデル）の削除.
	static void Finalize();// アセットマネージャーの終了処理.

private:
	AssetManager();
	~AssetManager();
	static AssetManager* Instance;

	unordered_map<string, int> meshMap;// メッシュ原本.
	vector<int> duplicateMesh;// メッシュを複製したもの
};

