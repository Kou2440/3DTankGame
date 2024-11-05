#include "DebugDraw.h"


void DrawGrid(float groundSize, int divideNum)
{
	VECTOR start, end;
	int color = GetColor(0, 255, 0);
	start = end = VGet(0, 0, 0);

	float halfWidth = groundSize * 0.5f;
	float addWidth = groundSize / (float)divideNum;
	// cü‚ğˆø‚­ 
	start.z = -halfWidth;
	end.z = +halfWidth;
	for (int ix = 0; ix < divideNum + 1; ix++)
	{
		start.x = end.x = -halfWidth + ix * addWidth;
		DrawLine3D(start, end, color);
	}

	// ‰¡ü
	start.x = -halfWidth;
	end.x = +halfWidth;
	for (int iz = 0; iz < divideNum + 1; iz++)
	{
		start.z = end.z = -halfWidth + iz * addWidth;
		DrawLine3D(start, end, color);
	}
}
