#pragma once
#include "DxLib.h"

///--------------------------------------------------------------
/// システム定数ヘッダー. 
///--------------------------------------------------------------
const VECTOR MoveModelScale = VGet(0.135f, 0.135f, 0.135f);
const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);// ベクトル初期化変数.
const int PlayerNum = 1;

const float WindowSizeXMin = -873.0f;// 画面端X上座標.
const float windowSizeXMax = WindowSizeXMin * -1;// 画面端X下座標.
const float WindowSizeZMin = -495.0f;;// 画面端Y(Z)上座標.
const float WindowSizeZMax = WindowSizeZMin * -1;// 画面端Y(z)下.座標.

const int ScreenMagni = 120;// 画面比率にかける倍率.
const int ScreenSizeX = 16 * ScreenMagni;// 横画面サイズ.
const int ScreenSizeY = 9 * ScreenMagni;// 縦画面サイズ.

const int White = GetColor(255, 255, 255);
const int Black = GetColor(0, 0, 0);
const int Red = GetColor(255, 0, 0);
const int Green = GetColor(0, 255, 0);
const int Blue = GetColor(0, 0, 255);