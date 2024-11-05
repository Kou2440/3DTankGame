#include "Collision.h"
#include "SystemConstant.h"
#include <cassert>
#include <algorithm>
#include <functional>
//-----------------------------------------------------------------------------
// @brief �����R���X�g���N�^
//-----------------------------------------------------------------------------
LineSegment::LineSegment()
	:localStart()
	,localEnd()
	,worldStart()
	,worldEnd()
{
}
//-----------------------------------------------------------------------------
// @brief �����R���X�g���N�^�����ʒu�Z�b�g
//-----------------------------------------------------------------------------

LineSegment::LineSegment(const VECTOR& start, const VECTOR& end)
	:localStart(start)
	,localEnd(end)
	,worldStart(start)
	,worldEnd(end)
{
}

//-----------------------------------------------------------------------------
// @brief �����̃��[���h�ʒu���ړ�����
//-----------------------------------------------------------------------------
void LineSegment::Move(const VECTOR& pos)
{
	worldStart = localStart + pos;
	worldEnd = localEnd + pos;
}


//-----------------------------------------------------------------------------
// @brief ���̃R���X�g���N�^
//-----------------------------------------------------------------------------
Sphere::Sphere()
	:localCenter()
	, worldCenter()
	, radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief ���̃R���X�g���N�^
//-----------------------------------------------------------------------------
Sphere::Sphere(const VECTOR& center, float radius)
	:localCenter(center)
	,worldCenter(center)
	,radius(radius)
{
}

//-----------------------------------------------------------------------------
// @brief ���̂̃��[���h���W���ړ�����
//-----------------------------------------------------------------------------
void Sphere::Move(const VECTOR& pos)
{
	worldCenter = localCenter + pos;
}

//-----------------------------------------------------------------------------
// @brief �J�v�Z�������蔻��R���X�g���N�^
//-----------------------------------------------------------------------------
Capsule::Capsule()
	:localStart()
	,localEnd()
	,worldStart()
	,worldEnd()
	,radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief �J�v�Z�������蔻��R���X�g���N�^
//-----------------------------------------------------------------------------
Capsule::Capsule(const VECTOR& start, const VECTOR& end, float radius)
	:localStart(start)
	,localEnd(end)
	,worldStart(start)
	,worldEnd(end)
	,radius(0.0f)
{
}

//-----------------------------------------------------------------------------
// @brief �J�v�Z���̃��[���h���W���ړ�����
//-----------------------------------------------------------------------------
void Capsule::Move(const VECTOR& pos)
{
	worldStart = localStart + pos;
	worldEnd = localEnd + pos;
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� ���E��
// @param[in] sphere1 ��
// @param[in] sphere2 ��
// @return bool �����m���Ԃ����Ă���ꍇ��true ��Ԃ�
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere1, const Sphere& sphere2)
{
	return HitCheck_Sphere_Sphere(sphere1.worldCenter, sphere1.radius, sphere2.worldCenter, sphere2.radius);
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� �����E��
// @param[in] line1 ����
// @param[in] sphere1 ��
// @return bool �����Ƌ����Ԃ����Ă���ꍇ��true ��Ԃ�
//-----------------------------------------------------------------------------
bool CollisionPair(const LineSegment& line, const Sphere& sphere)
{
	return HitCheck_Line_Sphere(line.worldStart, line.worldEnd, sphere.worldCenter, sphere.radius);
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� �����E��
// @param[in] sphere ��
// @param[in] line ����
// @return bool ���Ɛ������Ԃ����Ă���ꍇ��true ��Ԃ�
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere, const LineSegment& line)
{
	return HitCheck_Line_Sphere(line.worldStart, line.worldEnd, sphere.worldCenter, sphere.radius);
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� �����E���b�V��
// @param[in]  line          ����
// @param[in]  modelHandle   ���b�V���̃��f��ID
// @param[out] collisionInfo ���������ꍇ�̃|���S�����A�|���S�����X�g
// @return     �����ƃ��f�����Փ˂��Ă���ꍇ��true��Ԃ��BcollisionInfo�ɓ����蔻��ڍ׏�񂪕Ԃ�
//-----------------------------------------------------------------------------
bool CollisionPair(const LineSegment& line, const int modelHandle, MV1_COLL_RESULT_POLY& collisionInfo)
{
	collisionInfo = MV1CollCheck_Line(modelHandle, -1, line.worldStart, line.worldEnd);
	return collisionInfo.HitFlag;
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� ���b�V���E����
// @param[in]  modelHandle   ���b�V���̃��f��ID
// @param[in]  line          ����
// @param[out] collisionInfo ���������ꍇ�̃|���S�����A�|���S�����X�g
// @return     �����ƃ��f�����Փ˂��Ă���ꍇ��true��Ԃ��BcollisionInfo�ɓ����蔻��ڍ׏�񂪕Ԃ�
// @detail     �����ƃ��f���Փ˂̓����蔻��������t�o�[�W����
//-----------------------------------------------------------------------------
bool CollisionPair(const int modelHandle, const LineSegment& line, MV1_COLL_RESULT_POLY& collisionInfo)
{
	return CollisionPair(line, modelHandle, collisionInfo);
}

//-----------------------------------------------------------------------------
// @brief �����蔻�� ���E���b�V��
// @param[in]  sphere        ��
// @param[in]  modelHandle   ���b�V���̃��f��ID
// @param[out] collisionInfo ���������ꍇ�̃|���S�����A�|���S�����X�g
// @return     ���ƃ��f�����Փ˂��Ă���ꍇ��true��Ԃ�
//-----------------------------------------------------------------------------
bool CollisionPair(const Sphere& sphere, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	collisionInfo = MV1CollCheck_Sphere(modelHandle, -1, sphere.worldCenter, sphere.radius);
	if (collisionInfo.HitNum == 0)
	{
		return false;
	}
	return true;
}

bool CollisionPair(const Capsule& capsule, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	collisionInfo = MV1CollCheck_Capsule(modelHandle, -1, capsule.worldStart, capsule.worldEnd, capsule.radius);
	if (collisionInfo.HitNum == 0)
	{
		return false;
	}
	return true;
}

bool CollisionPair(const Capsule& capsule1, const Sphere& sphere)
{
	VECTOR startToSphere = capsule1.worldStart - sphere.worldCenter;// startToSphere   
	VECTOR startToEnd = capsule1.worldEnd - sphere.worldCenter;// startToEnd      
	VECTOR nAB = VNorm(startToSphere);//
	float dot = VDot(nAB, startToEnd);//nearLength      
	VECTOR n = capsule1.worldStart + (nAB * dot);
	
	float ret = dot / VSize(startToSphere);//nearLengthRate  
	VECTOR a = startToSphere * min(max(ret, 0), 1);
	VECTOR mNear = capsule1.worldStart + a;//near            

	float distance = 0.0f;
	VECTOR endToSphere = sphere.worldCenter - capsule1.worldEnd;
	VECTOR nearToSphere = sphere.worldCenter - n;
	if (ret < 0)
	{
		distance = VSize(capsule1.worldStart) * VSize(capsule1.worldStart);
	}
	else if (ret > 1)
	{
		distance = VSize(capsule1.worldEnd) * VSize(capsule1.worldEnd);
	}
	else
	{
		distance = VSize(sphere.worldCenter - mNear) * VSize(sphere.worldCenter - mNear);
	}
	return distance - (sphere.radius + capsule1.radius) * (sphere.radius + capsule1.radius) <= 0;

}
//-----------------------------------------------------------------------------
// �����߂��ʂ�Ԃ�.
//-----------------------------------------------------------------------------
VECTOR CalcSpherePushBackVecFromMesh(const Sphere& sphere, const MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	// �Փˋ�
	std::vector<VECTOR> moveCandidate; // �����S��� 
	float  radius = sphere.radius; // �����a
	VECTOR planeNormal;                    // �|���S�����ʖ@��
	VECTOR moveVec = VGet(0, 0, 0);    // �ړ��x�N�g��
	float  moveLen = 0.0f;           // �ړ���
	VECTOR newCenter = sphere.worldCenter; // �ړ����  
	int i = 0;

	int   minindex = 0;                  // �ŒZ�������C���f�b�N�X
	float minLen = sphere.radius * 2.0f; // �����߂��ŒZ�����i���a�ȏ�͉����߂��͂Ȃ��͂��j

	// �Փ˃|���S�������ׂĉ���āA���̂߂荞�݂���������
	for (i = 0; i < collisionInfo.HitNum; ++i)
	{
		moveCandidate.push_back(sphere.worldCenter);
		// �Փ˃|���S���̕� 
		VECTOR edge1, edge2;
		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];

		// �Փ˃|���S���̕ӂ��A�|���S���ʂ̖@���x�N�g�������߂�
		planeNormal = VCross(edge1, edge2);
		planeNormal = VNorm(planeNormal);

		// �����S�ɍł��߂��|���S�����ʂ̓_�����߂�
		VECTOR tmp = moveCandidate[i] - collisionInfo.Dim[i].Position[0];
		float  dot = VDot(planeNormal, tmp);

		// �Փ˓_
		VECTOR hitPos = moveCandidate[i] - planeNormal * dot;

		// �����ǂꂭ�炢�߂荞��ł��邩���Z�o
		VECTOR tmp2 = moveCandidate[i] - hitPos;
		float  len = VSize(tmp2);

		// �߂荞��ł���ꍇ�͋��̒��S�������߂�
		if (HitCheck_Sphere_Triangle(moveCandidate[i], radius,
			collisionInfo.Dim[i].Position[0],
			collisionInfo.Dim[i].Position[1],
			collisionInfo.Dim[i].Position[2]) == TRUE)
		{
			// �߂荞�݉�������ʒu�܂ňړ�
			VECTOR moveVec;
			
			minLen = radius - len;
			//len += 0.0001f;
			moveVec = planeNormal * minLen;
			moveCandidate[i] = moveVec;

			// �������߂��������͂���܂ł̌������Z��������
			if (minLen < len)
			{
				minLen = len;
				minindex = i;
			}
		}
	}

	//moveCandidate[minindex]�ɂ͍ŒZ�ړ������������Ă���A�ړ���̐V�������̒��S�ʒu�Ƃ���
	return moveCandidate[minindex];
}

//VECTOR CalCapsulePushBackVecFromMesh(const Capsule& capsule, int modelHandle, MV1_COLL_RESULT_POLY_DIM& collisionInfo)
//{
//	// �Փˋ�
//	std::vector<VECTOR> moveCandidate; // �����S��� 
//	float  radius = capsule.radius; // �����a
//	VECTOR planeNormal;                    // �|���S�����ʖ@��
//	VECTOR moveVec = VGet(0, 0, 0);    // �ړ��x�N�g��
//	float  moveLen = 0.0f;           // �ړ���
//	
//	//VECTOR newCenter = sphere.worldCenter; // �ړ���� 
//	int i = 0;
//
//	int   minindex = 0;                  // �ŒZ�������C���f�b�N�X
//	float minLen = sphere.radius * 2.0f; // �����߂��ŒZ�����i���a�ȏ�͉����߂��͂Ȃ��͂��j
//
//	// �Փ˃|���S�������ׂĉ���āA���̂߂荞�݂���������
//	for (i = 0; i < collisionInfo.HitNum; ++i)
//	{
//		moveCandidate.push_back(sphere.worldCenter);
//		// �Փ˃|���S���̕� 
//		VECTOR edge1, edge2;
//		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
//		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];
//
//		// �Փ˃|���S���̕ӂ��A�|���S���ʂ̖@���x�N�g�������߂�
//		planeNormal = VCross(edge1, edge2);
//		planeNormal = VNorm(planeNormal);
//
//		// �����S�ɍł��߂��|���S�����ʂ̓_�����߂�
//		VECTOR tmp = moveCandidate[i] - collisionInfo.Dim[i].Position[0];
//		float  dot = VDot(planeNormal, tmp);
//
//		// �Փ˓_
//		VECTOR hitPos = moveCandidate[i] - planeNormal * dot;
//
//		// �����ǂꂭ�炢�߂荞��ł��邩���Z�o
//		VECTOR tmp2 = moveCandidate[i] - hitPos;
//		float  len = VSize(tmp2);
//
//		// �߂荞��ł���ꍇ�͋��̒��S�������߂�
//		if (HitCheck_Sphere_Triangle(moveCandidate[i], radius,
//			collisionInfo.Dim[i].Position[0],
//			collisionInfo.Dim[i].Position[1],
//			collisionInfo.Dim[i].Position[2]) == TRUE)
//		{
//			// �߂荞�݉�������ʒu�܂ňړ�
//			VECTOR moveVec;
//
//			minLen = radius - len;
//			//len += 0.0001f;
//			moveVec = planeNormal * minLen;
//			moveCandidate[i] = moveVec;
//
//			// �������߂��������͂���܂ł̌������Z��������
//			if (minLen < len)
//			{
//				minLen = len;
//				minindex = i;
//			}
//		}
//	}
//
//	//moveCandidate[minindex]�ɂ͍ŒZ�ړ������������Ă���A�ړ���̐V�������̒��S�ʒu�Ƃ���
//	return moveCandidate[minindex];
//}

VECTOR PlaneNormal(const MV1_COLL_RESULT_POLY_DIM& collisionInfo)
{
	VECTOR planeNormal;                    // �|���S�����ʖ@��
	for (int i = 0; i < collisionInfo.HitNum; ++i)
	{
		// �Փ˃|���S���̕� 
		VECTOR edge1, edge2;
		edge1 = collisionInfo.Dim[i].Position[1] - collisionInfo.Dim[i].Position[0];
		edge2 = collisionInfo.Dim[i].Position[2] - collisionInfo.Dim[i].Position[0];

		// �Փ˃|���S���̕ӂ��A�|���S���ʂ̖@���x�N�g�������߂�
		planeNormal = VCross(edge1, edge2);
		planeNormal = VNorm(planeNormal);
	}
	return VECTOR();
}


bool offscreenDicision(VECTOR pos, float radius)
{
	if (ConvWorldPosToScreenPos(pos).x - radius < 0 || ConvWorldPosToScreenPos(pos).x + radius > ScreenSizeX ||
		ConvWorldPosToScreenPos(pos).y - radius < 0 || ConvWorldPosToScreenPos(pos).y + radius > ScreenSizeY)
	{
		return true;
	}
	return false;
}