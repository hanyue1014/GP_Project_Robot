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

	bool operator==(Transform& o)
	{
		return transX == o.transX && transY == o.transY && transZ == o.transZ && rotX == o.rotX && rotY == o.rotY && rotZ == o.rotZ && rotAngle == o.rotAngle && scaleX == o.scaleX && scaleY == o.scaleY && scaleZ == o.scaleZ;
	}

	bool operator!=(Transform& o)
	{
		return !(*this == o);
	}
};
