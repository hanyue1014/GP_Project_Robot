#pragma once

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"

namespace Robot
{
	// for hand and leg use, define position so place wont weird weird eh
	enum Position { POSITION_LEFT, POSITION_RIGHT };

	// no need create canvas everytime
	Canvas cv(20, 20, 20);
	Color primary = { 218, 219, 214 };
	Color lineC = { 39, 37, 35 };
	Color secondary = { 0, 255, 0 };

	void Head()
	{
		cv
			// left half
			.pushMatrix()
			.rotate(90, 0, 0, 1)
			.cuboid({ -0.3, 1.25, 0.2, primary }, { 0.7, 1.25, -0.2 }, { -1, 0.75, 0.6 }, { 1, 0.75, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid({ -1, 0.75, 0.6, primary }, { 1, 0, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			// "eyes"
			.pushMatrix()
			.translate(0, 0, 0.601)
			.pushMatrix() // top one
			.rotate(30, 0, 0, -1)
			.rect({ -0.1, 0.65, lineC }, { 0.1, 0.25 })
			.popMatrix() // top one
			.rotate(30, 0, 0, 1)
			.rect({ -0.1, 0.65, lineC }, { 0.1, 0.25 })
			.popMatrix()
			.popMatrix()
			// right half
			.pushMatrix()
			.rotate(180, 0, 1, 0)
			.rotate(90, 0, 0, 1)
			.cuboid({ -0.3, 1.25, 0.2, primary }, { 0.7, 1.25, -0.2 }, { -1, 0.75, 0.6 }, { 1, 0.75, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid({ -1, 0.75, 0.6, primary }, { 1, 0, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			// "eyes"
			.pushMatrix()
			.translate(0, 0, -0.601)
			.pushMatrix() // top one
			.rotate(30, 0, 0, -1)
			.rect({ -0.1, 0.65, lineC }, { 0.1, 0.25 })
			.popMatrix() // top one
			.rotate(30, 0, 0, 1)
			.rect({ -0.1, 0.65, lineC }, { 0.1, 0.25 })
			.popMatrix()
			.popMatrix()
			// chin
			.pyramid({ -0.75, -1, 0.6, primary }, { 0.75, -1, -0.6 }, { 0, -1.4, 0.25 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			// tianlinggai
			.pyramid({ -0.75, 1, 0.6, primary }, { 0.75, 1, -0.6 }, { 0, 1.2, 0 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;
	}

	class Finger {
	private:
		float newX, newY, newZ;
		// fingers only have one axis rotation freedom
		float rootAngle = 0, jointAngle = 0;
		float rootRotateX = 0; // only will use tiok for thumb
		float topLength = 0.375, botLength = 0.75; // only will be diff for thumb
		Position pos; // cuz if left then rotate eh direction diff
	public:
		Finger(Point3D p, Position pos = POSITION_RIGHT) : newX(p.x), newY(p.y), newZ(p.z), pos(pos) {}
		// finger total length will be 1.5 only
		void draw()
		{
			// top part of a finger
			cv
				.pushMatrix()
				.translate(newX, newY, newZ)
				.rotate(rootRotateX, pos == POSITION_RIGHT ? -1 : 1, 0, 0)
				.rotate(rootAngle, 0, 0, pos == POSITION_RIGHT ? -1 : 1)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 0.125)
				.cuboid({ -0.125, -0.125, 0.125, {255, 255, 0} }, { 0.125, -(0.125f + topLength), -0.125 })
				;

			// bottom part
			cv
				.pushMatrix()
				.translate(0, -(0.25 + topLength), 0)
				.rotate(jointAngle, 0, 0, pos == POSITION_RIGHT ? -1 : 1)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 0.125)
				.cuboid({ -0.125, -0.125, 0.125, {255, 255, 0} }, { 0.125, -(0.125f + botLength), -0.125 })
				.popMatrix()
				;

			cv.popMatrix();
		}

		void setAngle(float root, float joint)
		{
			rootAngle = root;
			jointAngle = joint;
		}

		// all these normally only used for thumb
		void forceRotateX(float a)
		{
			rootRotateX = a;
		}

		void setLength(float top, float bot)
		{
			topLength = top;
			botLength = bot;
		}
	};

	// for grip animation
	float gripTweenProgress = 0;
	bool _isGripping = false;
	float fingerLastGripRootAngle = 0, fingerCurrentGripRootAngle = 0;
	float fingerLastGripJointAngle = 0, fingerCurrentGripJointAngle = 0;
	float thumbLastGripRootAngle = 0, thumbCurrentGripRootAngle = 0;
	float thumbLastGripJointAngle = 0, thumbCurrentGripJointAngle = 0;
	// tested - grip angles for the fingers
	float fingerGripRoot = 85, thumbGripRoot = 45,
		fingerGripJoint = 90, thumbGripJoint = 90;

	// to keep track whether gripping is animating in progress
	bool startGripping = false;
	bool startUnGripping = false;
	class Hand
	{
	private:
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 0, rootYRotation = 0;
		float jointAngle = 180, jointYRotation = 0;
		const float upperArmLength = 5, botArmLength = 5;
		Position pos;

	public:
		// creates the hand at the position, default to right
		Hand(Point3D p, Position pos = POSITION_RIGHT) : newX(p.x), newY(p.y), newZ(p.z), pos(pos) {}

		void draw()
		{
			cv.pushMatrix().translate(newX, newY, newZ);

			cv
				.pushMatrix()
				.rotate(rootYRotation, 0, 1, 0)
				.rotate(rootAngle, 1, 0, 0)
				// basic skeleton
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1) // root bola
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, (0 + upperArmLength) }) // 0 cuz wanna factor in the bola
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				;
				
			cv.reflect(REFLECT_Y, pos == POSITION_LEFT);
			// just styles for upper arm eh bola	
			cv
				.pushMatrix()
				.rotate(90, -1, 0, 0)
				.cuboid({ -1, 1, 1.3, primary }, { 0.3, -1, 1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ 0.3, 1, 1.3, primary }, { 1.2, 1, 1.1 }, { 0.3, 0.1, 1.3 }, { 0.6, 0.1, 1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ -1, 1.2, 1.3, primary }, { 1.2, 1, -1.3 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ -1, 1, -1.3, primary }, { 0.3, -1, -1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ 0.3, 1, -1.3, primary }, { 1.2, 1, -1.1 }, { 0.3, 0.1, -1.3 }, { 0.6, 0.1, -1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ 1.4, 2, 1.3, primary }, { 2, 2, -1.3 }, { 0.3, 1.2, 1.3 }, { 1.4, 1.2, -1.3 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				;
			// styles for upper arm
			cv
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.popMatrix()
				;

			cv
				.pushMatrix() // lower arm rotations
				.translate(0, 0, 6)
				.rotate(jointYRotation, 0, 1, 0)
				.rotate(jointAngle, 1, 0, 0) // elbow bola
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, -1, {255, 255, 0} }, { 1, -1, -(0 + botArmLength) }) // 0 cuz wanna factor in the bola as well
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				;

			// lower arm styles
			cv
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.popMatrix()
				;

			// reset reflections
			cv.reflect();

			// blade like structure
			cv
				.pushMatrix()
				.rotate(90, 0, 0, pos == POSITION_LEFT ? 1 : -1)
				.cuboid({ -0.5, 1.5, -(botArmLength - 1), primary }, { 0.5, 1.5, -1 }, { -0.75, 2.5, -(botArmLength - 3) }, { 0.75, 2.5, botArmLength - 2 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(27.5, 0, 0, 1)
				.cuboid({ 0, 1.5, -(botArmLength - 1), secondary }, { 0.5, 1.5, -1 }, { -0.75, 2.5, -(botArmLength - 3) }, { 0.5, 2.5, botArmLength - 2 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(-27.5, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -(botArmLength - 1), secondary }, { 0, 1.5, -1 }, { -0.75, 2.5, -(botArmLength - 3) }, { 0.5, 2.5, botArmLength - 2 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.popMatrix()
				;

			// le palm + fingers
			cv
				.pushMatrix()
				.rotate(90, 1, 0, 0)
				.translate(0, -5, 0) // cuz above alrd translate 6 d
				.cuboid({ -0.25, 0, 0.5, primary }, { 0.25, 0, -0.5 }, { -0.25, -0.5, 0.75 }, { 0.25, -0.5, -0.75 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ -0.25, -0.5, 0.75, primary }, { 0.25, -1.5, -0.75 }) // real palm
				;

			// fingers
			//Finger index({  });
			// index finger -> pinky
			for (int i = 0; i < 4; i++)
			{
				Finger f({ 0, -(1.5 + 0.125), -(0.75f - ((i+1) * 0.225f) - (i * 0.125f)) }, pos);
				f.setAngle(fingerCurrentGripRootAngle, fingerCurrentGripJointAngle);
				f.draw();
			}

			// thumb
			Finger thumb({ 0, -0.75, 0.875 }, pos);
			thumb.setLength(0.375, 0.375);
			thumb.forceRotateX(pos == POSITION_RIGHT ? 90 : -90);
			thumb.setAngle(thumbCurrentGripRootAngle, thumbCurrentGripJointAngle);
			thumb.draw();

			cv
				.popMatrix()
				;

			cv.popMatrix(); // lower arm rotations
				
			cv.popMatrix(); // from root

			cv.popMatrix();
		}

		// only works if x = 0 after normalized
		void solveIK(Point3D target)
		{
			// normalize target
			float
				x = target.x - newX,
				y = target.y - newY,
				z = target.z - newZ;

			Point3D v1 = { x, y, z }; // the root -> target vector
			Point3D v2 = { 0, 0, 1 };  // the ori vector which z shouldn't actually matter

			float magV1 = magnitude(v1);
			float magV2 = magnitude(v2);


			float dotProd = dot(v1, v2);
			float cosAlpha = dotProd / (magV1 * magV2);
			float alpha;
			if (magV1 * magV2 == 0)
				alpha = 0;
			else
				alpha = acosf(cosAlpha);

			rootAngle = alpha / PI * 180;
			if (y > 0)
				rootAngle *= -1;
			// if longer than leg no need solve d, direct rotate 
			if (magV1 > upperArmLength + botArmLength)
			{
				return;
			}

			float cosPhi = magV1 / 2;
			float phi = acosf(cosPhi / upperArmLength);
			// if magV1 is 0 then mean same as origin point, then should not rotate anything
			if (magV1 == 0)
			{
				phi = 0;
			}
			phi = phi / PI * 180;
			rootAngle += phi;

			float theta = 180 - 90 - phi;
			// if magV1 is 0 then mean same as origin point, then should not rotate anything
			if (magV1 == 0)
			{
				theta = 0;
			}
			jointAngle = theta * 2;
		}

		// tells the hand to start gripping
		void grip(bool shouldGrip)
		{
			if (shouldGrip && !_isGripping)
			{
				if (!startGripping)
				{
					startGripping = true;
					gripTweenProgress = 0;
					startUnGripping = false;
				}
				// rest angle for all the fingers are 0
				fingerLastGripRootAngle = fingerCurrentGripRootAngle = tween(0, fingerGripRoot, gripTweenProgress += 0.01);
				thumbLastGripRootAngle = thumbCurrentGripRootAngle = tween(0, thumbGripRoot, gripTweenProgress);
				fingerLastGripJointAngle = fingerCurrentGripJointAngle = tween(0, fingerGripJoint, gripTweenProgress);
				thumbLastGripJointAngle = thumbCurrentGripJointAngle = tween(0, thumbGripJoint, gripTweenProgress);
				if (gripTweenProgress >= 1)
				{
					_isGripping = true;
					startGripping = false;
					gripTweenProgress = 0;
					fingerLastGripRootAngle = fingerCurrentGripRootAngle = fingerGripRoot;
					thumbLastGripRootAngle = thumbCurrentGripRootAngle = thumbGripRoot;
					fingerLastGripJointAngle = fingerCurrentGripJointAngle = fingerGripJoint;
					thumbLastGripJointAngle = thumbCurrentGripJointAngle = thumbGripJoint;
				}
			}
			else if (!shouldGrip && (_isGripping || fingerCurrentGripJointAngle != 0 || thumbCurrentGripJointAngle != 0 || fingerCurrentGripRootAngle != 0 || thumbCurrentGripJointAngle != 0))
			{
				if (!startUnGripping)
				{
					startUnGripping = true;
					gripTweenProgress = 0;
					startGripping = false;
				}
				fingerCurrentGripRootAngle = tween(fingerLastGripRootAngle, 0, gripTweenProgress += 0.01);
				thumbCurrentGripRootAngle = tween(thumbLastGripRootAngle, 0, gripTweenProgress);
				fingerCurrentGripJointAngle = tween(fingerLastGripJointAngle, 0, gripTweenProgress);
				thumbCurrentGripJointAngle = tween(thumbLastGripJointAngle, 0, gripTweenProgress);
				if (gripTweenProgress >= 1)
				{
					_isGripping = false;
					startGripping = false;
					gripTweenProgress = 0;
					fingerLastGripRootAngle = fingerCurrentGripRootAngle = thumbLastGripRootAngle = thumbCurrentGripRootAngle = fingerLastGripJointAngle = fingerCurrentGripJointAngle = thumbLastGripJointAngle = thumbCurrentGripJointAngle = 0;
				}
			}
		}

		bool isGripping()
		{
			return _isGripping;
		}

		void forceYRotation(float rootYRot, float jointYRot)
		{
			rootYRotation = rootYRot;
			jointYRotation = jointYRot;
		}
	};

	class Leg
	{
	private:
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 90;
		float jointAngle = 180;
		const float upperLegLength = 5, botLegLength = 5;

	public:
		// creates the hand at position
		Leg(Point3D p) : newX(p.x), newY(p.y), newZ(p.z) {}

		void draw()
		{
			cv.pushMatrix().translate(newX, newY, newZ);

			cv
				.pushMatrix()
				// root joint
				.rotate(rootAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, -1, {255, 255, 0} }, { 1, -1, -5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				;

			// upper leg style
			cv
				.cuboid({ -0.75, -1.3, 1.5, primary }, { 0.75, -1.5, -6 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.cuboid({ 0.75, -1.5, 1.5, primary }, { 0.75, -1.3, -6 }, { 1.5, -1, 1 }, { 1.5, -0.75, -5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.reflect(REFLECT_Y, true) // direct use reflect, lazy  rewrite d
				.replotPrevBlocky3D(GL_QUADS, primary)
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.reflect()
				;
				
			// "armor"
			// side way and back no need got "floating shield"
			cv
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				;

			cv
				.pushMatrix()
				// knee joint
				.translate(0, 0, -6)
				.rotate(jointAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, 5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				;

			cv
				.pushMatrix()
				.rotate(180, 1, 0, 0)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(270, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				.popMatrix()
				;
				
			cv
				.popMatrix() // knee
				;
			cv.popMatrix();

			cv.popMatrix();
		}

		// x of the points should be 0 after normalized with the origin
		void solveIK(Point3D target)
		{
			// normalize target
			float
				x = target.x - newX,
				y = target.y - newY,
				z = target.z - newZ;

			Point3D v1 = { x, y, z }; // the root -> target vector
			Point3D v2 = { 0, 0, -1 };  // the ori vector which z shouldn't actually matter

			float magV1 = magnitude(v1);
			float magV2 = magnitude(v2);


			float dotProd = dot(v1, v2);
			float cosAlpha = dotProd / (magV1 * magV2);
			float alpha = acosf(cosAlpha);

			rootAngle = alpha / PI * 180;
			// if longer than leg no need solve d, direct rotate 
			if (magV1 > upperLegLength + botLegLength)
			{
				return;
			}

			float cosPhi = magV1 / 2;
			float phi = acosf(cosPhi / upperLegLength);
			phi = phi / PI * 180;
			rootAngle += phi;

			float theta = 180 - 90 - phi;
			jointAngle = theta * 2;
		}
	};

	// leftLeg would go upwards first
	Point3D leftLegRestTarget = { -2, -13, 0 };
	Point3D leftLegWalkTargets[] = {
		{ -2, -9, 0 }, // go upwards 
		{ -2, -9, 1 }, // make a parabola when walking towards front
		{ -2, -9, 2 },
		{ -2, -10, 3 },
		{ -2, -11, 4 },
		{ -2, -12, 4 },
		{ -2, -13, 4 }, // touched ground
		{ -2, -13, 3 }, // start glide backwards
		{ -2, -13, 2 },
		{ -2, -13, 1 },
		{ -2, -13, 0 },
		{ -2, -13, -1 },
		{ -2, -13, -2 },
		{ -2, -13, -3 },
		{ -2, -13, -4 }, // reached max backwards
		{ -2, -12, -4 }, // start moving upwards in a parabola
		{ -2, -11, -4 },
		{ -2, -10, -3 },
		{ -2, -9, -2 },
		{ -2, -9, -1 } // next will be upwards center d (wrap around back to top)
	};
	Point3D leftLegCurrentTarget = leftLegRestTarget;

	// rightLeg would go backwards first
	Point3D rightLegRestTarget = { 2, -13, 0 };
	Point3D rightLegWalkTargets[] = {
		{ 2, -13, -1 }, // go backwards 
		{ -2, -13, -2 }, // continue glide backwards
		{ -2, -13, -3 },
		{ -2, -13, -4 }, // reached max backwards
		{ -2, -12, -4 }, // start moving upwards in a parabola
		{ -2, -11, -4 },
		{ -2, -10, -3 },
		{ -2, -9, -2 },
		{ -2, -9, -1 },
		{ -2, -9, 0 }, // up center
		{ -2, -9, 1 }, // make a parabola when walking towards front
		{ -2, -9, 2 },
		{ -2, -10, 3 },
		{ -2, -11, 4 },
		{ -2, -12, 4 },
		{ -2, -13, 4 }, // touched ground
		{ -2, -13, 3 }, // start glide backwards
		{ -2, -13, 2 },
		{ -2, -13, 1 },
		{ -2, -13, 0 }, // next should wrap around
	};
	Point3D rightLegCurrentTarget = rightLegRestTarget;
	// due to array arrangement only need one to keep track both
	int walkingAnimationIndex = 0;

	float leftHandRootRestYRotation = 0;
	float leftHandJointRestYRotation = 0;
	float leftHandRootYRotation = leftHandRootRestYRotation;
	float leftHandJointYRotation = leftHandJointRestYRotation;

	Point3D leftHandRestTarget = { -5, -3, 0 };
	Point3D leftHandWalkTargets[] = {
		{ -5, -1.5, 1 }, // swing backward
		{ -5, -2, 0 },
		{ -5, -1.5, -1},
		{ -5, -1, -2 },
		{ -5, -0.5, -3 },
		{ -5, 0.5, -4 }, // max back point
		{ -5, -0.5, -3 },
		{ -5, -1, -2 },
		{ -5, -1.5, -1 },
		{ -5, -2, 0 },
		{ -5, -1.5, 1 },	// resting position, swing back forward
		{ -5, -1.5, 1 },
		{ -5, -0.5, 3 },
		{ -5, 0, 4 },
		{ -5, 0.5, 5 },
		{ -5, 1.5, 6 }, // max point
		{ -5, 0.5, 5 },
		{ -5, 0, 4 },
		{ -5, -0.5, 3 },
		{ -5, -1, 2 },

	};
	Point3D leftHandCurrentTarget = leftHandRestTarget;

	float rightHandRootRestYRotation = 0;
	float rightHandJointRestYRotation = 0;
	float rightHandRootYRotation = rightHandRootRestYRotation;
	float rightHandJointYRotation = rightHandJointRestYRotation;

	Point3D rightHandRestTarget = { 5, -3, 0 };
	Point3D rightHandWalkTargets[] = {
		{ 5, -1.5, 1 },
		{ 5, -0.5, 3 },
		{ 5, 0, 4 },
		{ 5, 0.5, 5 },
		{ 5, 1.5, 6 }, // max point
		{ 5, 0.5, 5 },
		{ 5, 0, 4 },
		{ 5, -0.5, 3 },
		{ 5, -1, 2 },
		{ 5, -1.5, 1 }, // swing backward
		{ 5, -2, 0 },
		{ 5, -1.5, -1},
		{ 5, -1, -2 },
		{ 5, -0.5, -3 },
		{ 5, 0.5, -4 }, // max back point
		{ 5, -0.5, -3 },
		{ 5, -1, -2 },
		{ 5, -1.5, -1 },
		{ 5, -2, 0 },
		{ 5, -1.5, 1 },
	};
	Point3D rightHandCurrentTarget = rightHandRestTarget;

	bool isWalking = false;
	float walkingTweenProgress = 0;
	float stopWalkingTweenProgress = 0;
	float bodyCurrentWalkRotation = 0, maxBodyWalkRotation = 10, lastRotationBeforeStopWalking = 0;

	// debug purpose (test grip)
	bool shouldGrip = false;

	void main()
	{
		if (isWalking)
		{
			int walkAnimCount = sizeof(leftLegWalkTargets) / sizeof(leftLegWalkTargets[0]);

			// walking leg animation
			leftLegCurrentTarget = tween(leftLegWalkTargets[walkingAnimationIndex], leftLegWalkTargets[walkingAnimationIndex + 1 >= walkAnimCount ? 0 : walkingAnimationIndex + 1], walkingTweenProgress += 0.05);
			rightLegCurrentTarget = tween(rightLegWalkTargets[walkingAnimationIndex], rightLegWalkTargets[walkingAnimationIndex + 1 >= walkAnimCount ? 0 : walkingAnimationIndex + 1], walkingTweenProgress);

			// walking hand animation
			rightHandCurrentTarget = tween(rightHandWalkTargets[walkingAnimationIndex], rightHandWalkTargets[walkingAnimationIndex + 1 >= walkAnimCount ? 0 : walkingAnimationIndex + 1], walkingTweenProgress);
			leftHandCurrentTarget = tween(leftHandWalkTargets[walkingAnimationIndex], leftHandWalkTargets[walkingAnimationIndex + 1 >= walkAnimCount ? 0 : walkingAnimationIndex + 1], walkingTweenProgress);


			// first half of the 
			// at index == 10 and 20 eh time rotation should be 0
			// at 10 eh time percent = 0.5
			// at 20 eh time percent = 1
			// if split
			if (walkingAnimationIndex <= 4)
			{
				lastRotationBeforeStopWalking = bodyCurrentWalkRotation = tween(0, maxBodyWalkRotation, walkingAnimationIndex / 4.0);
			}
			else if (walkingAnimationIndex > 4 && walkingAnimationIndex <= 9)
			{
				lastRotationBeforeStopWalking = bodyCurrentWalkRotation = tween(maxBodyWalkRotation, 0, (walkingAnimationIndex - 4) / 5.0);
			}
			else if (walkingAnimationIndex > 9 && walkingAnimationIndex <= 14)
			{
				lastRotationBeforeStopWalking = bodyCurrentWalkRotation = tween(0, -maxBodyWalkRotation, (walkingAnimationIndex - 9) / 5.0);
			}
			else 
			{
				lastRotationBeforeStopWalking = bodyCurrentWalkRotation = tween(-maxBodyWalkRotation, 0, (walkingAnimationIndex - 14) / 5.0);
			}

			if (walkingTweenProgress >= 1)
			{
				walkingTweenProgress = 0;
				if (++walkingAnimationIndex >= walkAnimCount)
					walkingAnimationIndex = 0;
			}
		}
		// if stopped walking but still not in leg resting position, tween it back
		if (!isWalking 
			&& (leftLegCurrentTarget != leftLegRestTarget || rightLegCurrentTarget != rightLegRestTarget)
			&& (leftHandCurrentTarget != leftHandRestTarget || rightHandCurrentTarget != rightHandRestTarget)
			)
		{
			// tween back
			leftLegCurrentTarget = tween(leftLegWalkTargets[walkingAnimationIndex], leftLegRestTarget, stopWalkingTweenProgress += 0.005);
			rightLegCurrentTarget = tween(rightLegWalkTargets[walkingAnimationIndex], rightLegRestTarget, stopWalkingTweenProgress);

			// tween back hand
			rightHandCurrentTarget = tween(rightHandWalkTargets[walkingAnimationIndex], rightHandRestTarget, stopWalkingTweenProgress);
			leftHandCurrentTarget = tween(leftHandWalkTargets[walkingAnimationIndex], leftHandRestTarget, stopWalkingTweenProgress);

			bodyCurrentWalkRotation = tween(lastRotationBeforeStopWalking, 0, stopWalkingTweenProgress);

			if (stopWalkingTweenProgress >= 1)
			{
				walkingAnimationIndex = 0;
				walkingTweenProgress = 0;
				stopWalkingTweenProgress = 0;
				// for some reason cpp eh 1 can become 1.00000001 eh at that time my tween function wont exactly tween it to the same numbers, so set it back manually to escape this condition
				leftLegCurrentTarget = leftLegRestTarget;
				rightLegCurrentTarget = rightLegRestTarget;
				leftHandCurrentTarget = leftHandRestTarget;
				rightHandCurrentTarget = rightHandRestTarget;
			}
		}

		cv.pushMatrix().translate(0, 10, 0);
		Head();
		cv.popMatrix();

		cv
			.pushMatrix() // upper body rotate when walking
			.rotate(bodyCurrentWalkRotation, 0, 1, 0)
			;
		// upper body
		cv
			.cuboid(
				{ -1.5, 3, 1.5, {255, 0, 0} },
				{ 1.5, 3, -1.5 },
				{ -0.5, 2, 0.5 },
				{ 0.5, 2, -0.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -2.5, 4, 2, {255, 0, 0} },
				{ 2.5, 4, -2 },
				{ -1.5, 3, 1.5 },
				{ 1.5, 3, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid({ -2.5, 6, 2, {255, 0, 0} }, { 2.5, 4, -2 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -2, 7, 1.5, {255, 0, 0} },
				{ 2, 7, -1.5 },
				{ -2.5, 6, 2 },
				{ 2.5, 6, -2 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -1, 8, 1, {255, 0, 0} },
				{ 1, 8, -1 },
				{ -2, 7, 1.5 },
				{ 2, 7, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;

		// arm joint to body
		// right
		cv
			.pushMatrix()
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, {0, 255, 255} }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		// left, lazy rethink coords d
		cv
			.pushMatrix()
			.rotate(180, 0, 1, 0)
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, {0, 255, 255} }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		Hand rightHand({ 5, 8, 0 });
		rightHand.solveIK(rightHandCurrentTarget);
		rightHand.forceYRotation(rightHandRootYRotation, rightHandJointYRotation);
		rightHand.grip(shouldGrip);
		rightHand.draw();

		Hand leftHand({ -5, 8, 0 }, POSITION_LEFT);
		leftHand.solveIK(leftHandCurrentTarget);
		leftHand.forceYRotation(leftHandRootYRotation, leftHandJointYRotation);
		leftHand.grip(shouldGrip);
		leftHand.draw();

		cv.popMatrix();

		cv
			.cube({ 0, 1.5, 0, {255, 255, 0} }, 0.5) // upper body join lower body eh part
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;
		// lower body
		cv
			.cuboid({ -1, -1, 1, {0, 0, 255} }, { 1, -1, -1 }, { -0.5, -2, 0.5 }, { 0.5, -2, -0.5 })
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -1.5, 0, 1.5, {0, 255, 255} },
				{ 1.5, 0, -1.5 },
				{ -1, -1, 1 },
				{ 1, -1, -1 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			.cuboid(
				{ -0.5, 1, 0.5, {0, 255, 255} },
				{ 0.5, 1, -0.5 },
				{ -1.5, 0, 1.5 },
				{ 1.5, 0, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
			;

		Leg leftLeg({ 2, -2, 0 });
		leftLeg.solveIK(leftLegCurrentTarget);
		leftLeg.draw();
		Leg rightLeg({ -2, -2, 0 });
		rightLeg.solveIK(rightLegCurrentTarget);
		rightLeg.draw();
	}

	void handleKeyDownEvent(WPARAM key)
	{
		switch (key)
		{
		case 'W': 
			isWalking = true;
			break;
		case 'G':
			shouldGrip = !shouldGrip;
			break;
		}
	}

	void handleKeyUpEvent(WPARAM key)
	{
		switch (key)
		{
		case 'W':
			isWalking = false;
			break;
		}
	}
}
