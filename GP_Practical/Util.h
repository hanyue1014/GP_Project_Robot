#pragma once

#include "Canvas.h"

float tween(float from, float to, float percent);
Color tween(Color from, Color to, float percent);
Point3D tween(Point3D from, Point3D to, float percent);

// assuming v1 is the vector to the point from origin
float magnitude(Point3D v1);

// assuming v1 and v2 both is vector from origin to the both point
float dot(Point3D v1, Point3D v2);
