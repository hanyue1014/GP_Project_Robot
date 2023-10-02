#pragma once

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"

namespace Robot
{
	// no need create canvas everytime
	Canvas cv(20, 20, 20);

	class Hand
	{
	private:
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 0, rootYRotation = 0;
		float jointAngle = 180, jointYRotation = 0;
		const float upperArmLength = 5, botArmLength = 5;
	public:
		// creates the hand at the position
		Hand(Point3D p) : newX(p.x), newY(p.y), newZ(p.z) {}

		void draw()
		{
			cv.pushMatrix().translate(newX, newY, newZ);

			cv
				.pushMatrix()
				.rotate(rootYRotation, 0, 1, 0)
				.rotate(rootAngle, 1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1) // root bola
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, (0 + upperArmLength) }) // 0 cuz wanna factor in the bola
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				;

			cv
				.pushMatrix()
				.translate(0, 0, 6)
				.rotate(jointYRotation, 0, 1, 0)
				.rotate(jointAngle, 1, 0, 0) // elbow bola
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, -1, {255, 255, 0} }, { 1, -1, -(0 + botArmLength) }) // 0 cuz wanna factor in the bola as well
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
				;
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

			cv
				.pushMatrix()
				// knee joint
				.translate(0, 0, -6)
				.rotate(jointAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, 5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
				.popMatrix()
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

	Point3D leftHandRestTarget = { -5, -1.5, 1 };
	// TODO: add handTargets and when walking
	Point3D leftHandWalkTargets[] = {
		{ -5, -1.5, 0 },
		{ -5, -0.5, -1},
		{ -5, -0.5, -2 },
		{ -5, 0.5, -3 },
		{ -5, 1.5, -4 }, // max back point
		{ -5, 0.5, -3 },
		{ -5, -0.5, -2 },
		{ -5, -0.5, -1 },
		{ -5, -1.5, 0 },
		{ -5, -1.5, 1 },
		{ -5, -1.5, 2 },
		{ -5, -0.5, 3 },
		{ -5, -0.5, 4 },
		{ -5, 0.5, 5 },
		{ -5, 1.5, 6 }, // max point
		{ -5, 0.5, 5 },
		{ -5, -0.5, 4 },
		{ -5, -0.5, 3 },
		{ -5, -1.5, 2 },
		{ -5, -1.5, 1 }, // swing forward and back to rest position
	};
	Point3D leftHandCurrentTarget = leftHandRestTarget;

	float rightHandRootRestYRotation = 0;
	float rightHandJointRestYRotation = 0;
	float rightHandRootYRotation = rightHandRootRestYRotation;
	float rightHandJointYRotation = rightHandJointRestYRotation;
	Point3D handTargetDebug = { 5, -1.5, 1, {200, 255, 255} };

	Point3D rightHandRestTarget = { 5, -1.5, 1 };
	Point3D rightHandWalkTargets[] = {
		{ 5, -1.5, 2 },
		{ 5, -0.5, 3 },
		{ 5, -0.5, 4 },
		{ 5, 0.5, 5 },
		{ 5, 1.5, 6 }, // max point
		{ 5, 0.5, 5 },
		{ 5, -0.5, 4 },
		{ 5, -0.5, 3 },
		{ 5, -1.5, 2 },
		{ 5, -1.5, 1 }, // swing forward and back to rest position
		{ 5, -1.5, 0 },
		{ 5, -0.5, -1},
		{ 5, -0.5, -2 },
		{ 5, 0.5, -3 },
		{ 5, 1.5, -4 }, // max back point
		{ 5, 0.5, -3 },
		{ 5, -0.5, -2 },
		{ 5, -0.5, -1 },
		{ 5, -1.5, 0 },
		{ 5, -1.5, 1 },
	};
	Point3D rightHandCurrentTarget = rightHandRestTarget;

	bool isWalking = false;
	float walkingTweenProgress = 0;
	float stopWalkingTweenProgress = 0;
	float bodyCurrentWalkRotation = 0, maxBodyWalkRotation = 10, lastRotationBeforeStopWalking = 0;

	void main()
	{
		if (isWalking)
		{

			// walking leg animation
			leftLegCurrentTarget = tween(leftLegWalkTargets[walkingAnimationIndex], leftLegWalkTargets[walkingAnimationIndex + 1], walkingTweenProgress += 0.05);
			rightLegCurrentTarget = tween(rightLegWalkTargets[walkingAnimationIndex], rightLegWalkTargets[walkingAnimationIndex + 1], walkingTweenProgress);

			// walking hand animation
			rightHandCurrentTarget = tween(rightHandWalkTargets[walkingAnimationIndex], rightHandWalkTargets[walkingAnimationIndex + 1], walkingTweenProgress);
			leftHandCurrentTarget = tween(leftHandWalkTargets[walkingAnimationIndex], leftHandWalkTargets[walkingAnimationIndex + 1], walkingTweenProgress);

			int walkAnimCount = sizeof(leftLegWalkTargets) / sizeof(leftLegWalkTargets[0]);

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

		cv
			.pushMatrix() // upper body rotate when walking
			.rotate(bodyCurrentWalkRotation, 0, 1, 0)
			;
		// upper body
		cv
			.cube({ 0, 1.5, 0, {255, 255, 0} }, 0.5)
			.replotPrevBlocky3D(GL_LINE_LOOP, { 0, 0, 0 })
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
		//cv.pointSize(20).point(handTargetDebug).pointSize(1);
		//rightHand.solveIK(handTargetDebug);
		rightHand.forceYRotation(rightHandRootYRotation, rightHandJointYRotation);
		rightHand.draw();

		Hand leftHand({ -5, 8, 0 });
		leftHand.solveIK(leftHandCurrentTarget);
		leftHand.forceYRotation(leftHandRootYRotation, leftHandJointYRotation);
		leftHand.draw();

		cv.popMatrix();

		// lower body
		cv
			.cuboid({ -1, -1, 1, {0, 0, 255} }, { 1, -3, -1 })
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
		case VK_UP:
			handTargetDebug.y += 0.5;
			break;
		case VK_DOWN:
			handTargetDebug.y -= 0.5;
			break;
		case VK_LEFT:
			handTargetDebug.z += 0.5;
			break;
		case VK_RIGHT:
			handTargetDebug.z -= 0.5;
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
		default:
			break;
		}
	}
}
