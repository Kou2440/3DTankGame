#include "Camera.h"
#include "ObjectTag.h"
#include "ObjectManager.h"
#include "PlayerBody.h"


MainCamera::MainCamera()
	:ObjectBase(ObjectTag::Camera)
{
	
	SetCameraNearFar(CameraRangeNear, CameraRangeFar);
	aimTargetPos = VGet(0.0f, 0.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(InitPos, aimTargetPos);

}


void MainCamera::Update(float deltaTime)
{
	// ˆ—‚È‚µ.
}
