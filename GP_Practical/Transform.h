#pragma once

struct Transform
{
	float
		transX, transY, transZ,
		rotX, rotY, rotZ, rotAngle,
		scaleX, scaleY, scaleZ;
	Transform()
	{
		transX = transY = transZ = rotX = rotY = rotZ = rotAngle = scaleX = scaleY = scaleZ = 0.0f;
	}
};
