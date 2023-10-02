#include "Util.h"
#include <cmath>

float tween(float from, float to, float percent)
{
	return from + (percent * (to - from));
}

Color tween(Color from, Color to, float percent)
{
	return Color(
		tween(from.r, to.r, percent),
		tween(from.g, to.g, percent),
		tween(from.b, to.b, percent),
		tween(from.a, to.a, percent)
	);
}

Point3D tween(Point3D from, Point3D to, float percent)
{
	return Point3D(
		tween(from.x, to.x, percent),
		tween(from.y, to.y, percent),
		tween(from.z, to.z, percent)
	);
}

float magnitude(Point3D v1)
{
	return sqrtf(powf(v1.x, 2) + powf(v1.y, 2) + powf(v1.z, 2));
}

float dot(Point3D v1, Point3D v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
