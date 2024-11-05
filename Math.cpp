#include "Math.h"
#include <math.h>

//-------------------------------------------------------------------------------
	// @brief ベクトル同士の加算
	//-------------------------------------------------------------------------------
VECTOR operator+(const VECTOR& lhs, const VECTOR& rhs)
{
	VECTOR ret;
	ret.x = lhs.x + rhs.x;
	ret.y = lhs.y + rhs.y;
	ret.z = lhs.z + rhs.z;
	return ret;
}
//-------------------------------------------------------------------------------
// @brief ベクトル同士の減算
//-------------------------------------------------------------------------------
VECTOR operator-(const VECTOR& lhs, const VECTOR& rhs)
{
	VECTOR ret;
	ret.x = lhs.x - rhs.x;
	ret.y = lhs.y - rhs.y;
	ret.z = lhs.z - rhs.z;
	return ret;
}
//-------------------------------------------------------------------------------
// @brief ベクトルのスカラー倍
//-------------------------------------------------------------------------------
VECTOR operator*(float s, const VECTOR& rhs)
{
	VECTOR ret;
	ret.x = s * rhs.x;
	ret.y = s * rhs.y;
	ret.z = s * rhs.z;
	return ret;
}
//-------------------------------------------------------------------------------
// @brief ベクトルのスカラー倍
//-------------------------------------------------------------------------------
VECTOR operator*(const VECTOR& lhs, float s)
{
	VECTOR ret;
	ret.x = s * lhs.x;
	ret.y = s * lhs.y;
	ret.z = s * lhs.z;
	return ret;
}
//-------------------------------------------------------------------------------
// @brief += VECTORの演算子
//-------------------------------------------------------------------------------
VECTOR operator+=(VECTOR& lhs, const VECTOR& rhs)
{
	lhs = lhs + rhs;
	return lhs;
}

//-------------------------------------------------------------------------------
// @brief *= VECTORのスカラー倍の複合代入演算子
//-------------------------------------------------------------------------------
VECTOR operator*=(VECTOR& lhs, float rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

//-------------------------------------------------------------------------------
// @brief ２つのベクトルの角度はほぼ同じか
// @param[in] v1 調査したいベクトル1
// @param[in] v2 調査したいベクトル2
// @return 二つがほぼ同じ角度であれば true 異なればfalseを返す
//-------------------------------------------------------------------------------
bool IsNearAngle(const VECTOR& v1, const VECTOR& v2)
{
	float dot = VDot(v1, v2);
	if (dot > 0.999f)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------
// @param[in] nowVec 現在の方向ベクトル
// @param[in] dirVec 目標方向ベクトル
// @return float 時計回り方向の時は+1.0 反時計回り方向の時は -1.0を返す.
//-------------------------------------------------------------------------------
float CalcRotationDirectionYAxis(const VECTOR& nowVec, const VECTOR& dirVec)
{
	VECTOR axis;
	axis = VCross(nowVec, dirVec);
	if (axis.y < 0.0f)
	{
		return -1.0f;
	}
	return 1.0f;
}

//-------------------------------------------------------------------------------
// @brief nowVecから aimVecに向かってdegreeVerociyの速度でY回転する.
// @param[in] nowVec 現在の方向ベクトル
// @param[in] aimVec 目標方向ベクトル
//-------------------------------------------------------------------------------
VECTOR RotateForAimVecYAxis(const VECTOR& nowVec, const VECTOR& aimVec, float degreeVerocity)
{
	// 角速度（度数）をラジアン角に変換、右回りか左回りかを調べる
	float rotRadian = (DX_PI_F * degreeVerocity / 180.0f);
	rotRadian *= CalcRotationDirectionYAxis(nowVec, aimVec);

	// Y軸回転行列を作成する
	MATRIX yrotMat = MGetRotY(rotRadian);

	// Y軸回転する
	VECTOR rotVec;
	rotVec = VTransform(nowVec, yrotMat);

	return rotVec;
}

float Sigmoid(float x, float gain)
{
	return 1.0 / (1.0 + exp(-gain * x));
}

float ToRadian(float degree)
{
	return degree * DX_PI_F / 180.0f;
}

float VectorSize(VECTOR& vec)
{
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
