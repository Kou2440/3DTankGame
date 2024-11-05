//-----------------------------------------------------------------------------
// @brief  メイン処理.
//-----------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <vector>
#include "Director.h"
#include "EffekseerForDXLib.h"
#include "ObjectManager.h"
#include "AssetManager.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "StartScene.h"
#include "Camera.h"
#include "DebugDraw.h"
#include "PlayScene.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリ初期化処理
	SetUseDirect3DVersion(DX_DIRECT3D_11);// 使用するDirectXバージョンを指定.

	// 画面モードセット.
	ChangeWindowMode(TRUE);// ウィンドウモードの変更.
	SetGraphMode(ScreenSizeX, ScreenSizeY, 16);
	SetBackgroundColor(70, 70, 70);// とりあえず灰色に.

	SetDrawScreen(DX_SCREEN_BACK);// 描画先を裏画面に変更する。

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ウインドウがアクティブではない状態でも処理を続行する。
	SetAlwaysRunFlag(true);

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// ライトを設定.
	SetLightDirection(VGet(0.0f, -1.0f, 0.0f));

	// マネージャー生成.
	AssetManager::Initalize();
	ObjectManager::Initialize();
	EffectManager::Initialize();

	SCENEINSTANCE.SetScene(new StartScene);

	
	//時間計測.
	LONGLONG nowTime = GetNowHiPerformanceCount();
	LONGLONG prevTime = nowTime;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// DXライブラリのカメラとEffekseerのカメラを同期する.
		Effekseer_Sync3DSetting();
		//フレーム時間計測.
		nowTime = GetNowHiPerformanceCount();
		float deltaTime = (nowTime - prevTime) / 1000000.0f;// マイクロ秒を秒にする.
	
		SCENEINSTANCE.Update(deltaTime);

		//画面の初期化.
		ClearDrawScreen();

		SCENEINSTANCE.Draw();
		//DrawGrid(3000, 100);
		//裏画面の内容を表画面に反映させる.
		ScreenFlip();
		prevTime = nowTime;
	}

	// オブジェクトの削除.
	ObjectManager::ReleseAllObj();
	ObjectManager::Finalize();

	// アセットの削除.
	AssetManager::DeleteAllAsset();
	AssetManager::Finalize();

	EffectManager::ReleseAllEffect();
	EffectManager::Finalize();

	// Effekseerを終了する。
	Effkseer_End();

	DxLib_End();

	return 0;
}
