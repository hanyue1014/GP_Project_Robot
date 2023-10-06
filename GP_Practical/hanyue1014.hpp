#pragma once

#include "Canvas.h"
#include "Transform.h"
#include "Util.h"
#include "soonchee.hpp"
#include "yikit.hpp"

namespace Robot
{
	// for hand and leg use, define position so place wont weird weird eh
	enum Position { POSITION_LEFT, POSITION_RIGHT };
	enum GripType { GRIP_FULL, GRIP_KAMEKAMEHA };
	enum SwordAnimStates { SWORD_UNEQUIP_FLYOUT, SWORD_UNEQUIP_FLYIN, SWORD_UNEQUIP_IDLE, SWORD_EQUIP_FLYOUT, SWORD_EQUIP_FLYIN, SWORD_EQUIPPED };
	enum SwordAttackTypes { SWORD_ATK_VER, SWORD_ATK_HOR };
	enum AttackWithSwordAnimState { SWORD_ATK_START_SWING, SWORD_ATK_FINISH_SWING, SWORD_ATK_SWING_OVERSHOOT, SWORD_ATK_IDLE }; // start swing is first half, finish swing is second half, idle means nothing is animating
	enum NowAnimating { WALK, SWORD_EQUIP_UNEQUIP, HAMMER_EQUIP_UNEQUIP, GUN_EQUIP_UNEQUIP, SHIELD, ATTACK_WITH_SWORD, ATTACK_WITH_HAMMER, ATTACK_WITH_GUN, CHARGE_KAMEKAMEHA, DEBUG_PLAYBACK, ANIMATING_NONE };
	enum EditModeEditTargets { EDIT_LEFT_HAND, EDIT_RIGHT_HAND, EDIT_LEFT_LEG, EDIT_RIGHT_LEG };
	enum KamekamehaChargeProgress { KKH_NONE, KKH_LOW, KKH_MEDIUM, KKH_HIGH, KKH_SHOOT, KKH_SHOOTING };
	enum HammerAnimStates { HAMMER_FLYOUT, HAMMER_UNGRIPPING, HAMMER_FLYIN, HAMMER_GRIPPING, HAMMER_EQUIPPED, HAMMER_UNEQUIPPED };
	enum AttackWithHammerAnimState { HAMMER_ATK_START_SWING, HAMMER_ATK_FINISH_SWING, HAMMER_ATK_SWING_OVERSHOOT, HAMMER_ATK_IDLE };
	enum GunAnimStates { GUN_EQUIPPING, GUN_UNEQUIPPING, GUN_EQUIPPED, GUN_UNEQUIPPED };
	enum GunShootingStates { GUN_RECOIL, GUN_READY };
	enum GunShootHand { LEFT_GUN, RIGHT_GUN };

	// no need create canvas everytime
	Canvas cv(20, 20, 20);
	Color primary = { 218, 219, 214 };
	Color lineOrJoint = { 80, 87, 132 };
	Color eyeC = { 39, 37, 55 };
	Color secondary = { 3, 210, 255 };
	Color accent = { 255, 165, 0 };
	Color outline = { 0, 0, 0 };

	void Head()
	{
		cv
			// left half
			.pushMatrix()
			.rotate(90, 0, 0, 1)
			// "eyes"
			.pushMatrix()
			.translate(0, 0, 0.601)
			.pushMatrix() // top one
			.rotate(30, 0, 0, -1)
			.rect({ -0.1, 0.65, eyeC }, { 0.1, 0.25 })
			.popMatrix() // top one
			.rotate(30, 0, 0, 1)
			.rect({ -0.1, 0.65, eyeC }, { 0.1, 0.25 })
			.popMatrix()
			.cuboid({ -0.3, 1.25, 0.2, primary }, { 0.7, 1.25, -0.2 }, { -1, 0.75, 0.6 }, { 1, 0.75, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid({ -1, 0.75, 0.6, primary }, { 1, 0, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.popMatrix()
			// right half
			.pushMatrix()
			.rotate(180, 0, 1, 0)
			.rotate(90, 0, 0, 1)
			// "eyes"
			.pushMatrix()
			.translate(0, 0, -0.601)
			.pushMatrix() // top one
			.rotate(30, 0, 0, -1)
			.rect({ -0.1, 0.65, eyeC }, { 0.1, 0.25 })
			.popMatrix() // top one
			.rotate(30, 0, 0, 1)
			.rect({ -0.1, 0.65, eyeC }, { 0.1, 0.25 })
			.popMatrix()
			.cuboid({ -0.3, 1.25, 0.2, primary }, { 0.7, 1.25, -0.2 }, { -1, 0.75, 0.6 }, { 1, 0.75, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid({ -1, 0.75, 0.6, primary }, { 1, 0, -0.6 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.popMatrix()
			// chin
			.pyramid({ -0.75, -1, 0.6, primary }, { 0.75, -1, -0.6 }, { 0, -1.4, 0.25 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			// tianlinggai
			.pyramid({ -0.75, 1, 0.6, primary }, { 0.75, 1, -0.6 }, { 0, 1.2, 0 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
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
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				;

			// bottom part
			cv
				.pushMatrix()
				.translate(0, -(0.25 + topLength), 0)
				.rotate(jointAngle, 0, 0, pos == POSITION_RIGHT ? -1 : 1)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 0.125)
				.cuboid({ -0.125, -0.125, 0.125, {255, 255, 0} }, { 0.125, -(0.125f + botLength), -0.125 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
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
	struct GripAnimationVars
	{
		float fingerGripTweenProgress = 0;
		bool _isGripping = false;
		float fingerLastGripRootAngle = 0, fingerCurrentGripRootAngle = 0;
		float fingerLastGripJointAngle = 0, fingerCurrentGripJointAngle = 0;
		float thumbLastGripRootAngle = 0, thumbCurrentGripRootAngle = 0;
		float thumbLastGripJointAngle = 0, thumbCurrentGripJointAngle = 0;
		// to keep track whether gripping is animating in progress
		bool startGripping = false;
		bool startUnGripping = false;
	};
	GripAnimationVars left;
	GripAnimationVars right;
	// tested - grip angles for the fingers
	struct GripAngles
	{
		float fingerGripRoot = 0, thumbGripRoot = 5,
			fingerGripJoint = 0, thumbGripJoint = 0;
		GripAngles(float fgr, float tgr, float fgj, float tgj)
			: fingerGripRoot(fgr), fingerGripJoint(fgj), thumbGripRoot(tgr), thumbGripJoint(tgj) {}
	};

	GripAngles fullGrip(85, 45, 90, 90);
	GripAngles kamekamehaGrip(0, 0, 0, 0);

	SwordAnimStates swordState = SWORD_UNEQUIP_IDLE;
	HammerAnimStates hammerState = HAMMER_UNEQUIPPED;
	float hammerAnimTranslateZ = 0, hammerAnimFlyInFromFlyOutTo = 15; // add this value to make it fly out
	GunAnimStates gunState = GUN_UNEQUIPPED;
	float gunEquipRotation = 0;

	class Hand
	{
	private:
		float newX, newY, newZ; // marks the newX and newY of the top side's sphere
		float rootAngle = 0, rootYRotation = 0;
		float jointAngle = 180, jointYRotation = 0;
		float handYRotation = 0, handZRotation = 30;
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
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				;

			cv.reflect(REFLECT_Y, pos == POSITION_LEFT);
			// just styles for upper arm eh bola	
			cv
				.pushMatrix()
				.rotate(90, -1, 0, 0)
				.cuboid({ -1, 1, 1.3, primary }, { 0.3, -1, 1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ 0.3, 1, 1.3, primary }, { 1.2, 1, 1.1 }, { 0.3, 0.1, 1.3 }, { 0.6, 0.1, 1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ -1, 1.2, 1.3, primary }, { 1.2, 1, -1.3 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ -1, 1, -1.3, primary }, { 0.3, -1, -1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ 0.3, 1, -1.3, primary }, { 1.2, 1, -1.1 }, { 0.3, 0.1, -1.3 }, { 0.6, 0.1, -1.1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ 1.4, 2, 1.3, primary }, { 2, 2, -1.3 }, { 0.3, 1.2, 1.3 }, { 1.4, 1.2, -1.3 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				;
			// styles for upper arm
			cv
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, upperArmLength - 1, primary }, { 0.5, 1.5, 2 }, { -1, 1, upperArmLength }, { 1, 1, 1 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
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
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				;

			// lower arm styles
			cv
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -2, primary }, { 0.5, 1.5, -(botArmLength - 1) }, { -1, 1, -1 }, { 1, 1, -botArmLength })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
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
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(27.5, 0, 0, 1)
				.cuboid({ 0, 1.5, -(botArmLength - 1), accent }, { 0.5, 1.5, -1 }, { -0.75, 2.5, -(botArmLength - 3) }, { 0.5, 2.5, botArmLength - 2 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(-27.5, 0, 0, 1)
				.cuboid({ -0.5, 1.5, -(botArmLength - 1), accent }, { 0, 1.5, -1 }, { -0.75, 2.5, -(botArmLength - 3) }, { 0.5, 2.5, botArmLength - 2 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.popMatrix()
				;

			// le palm + fingers
			cv
				.pushMatrix()
				.rotate(90, 1, 0, 0)
				.translate(0, -5, 0) // cuz above alrd translate 6 d
				.rotate(handZRotation, 0, 0, pos == POSITION_RIGHT ? 1 : -1)
				.rotate(handYRotation, 0, pos == POSITION_RIGHT ? -1 : 1, 0)
				.cuboid({ -0.25, 0, 0.5, primary }, { 0.25, 0, -0.5 }, { -0.25, -0.5, 0.75 }, { 0.25, -0.5, -0.75 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ -0.25, -0.5, 0.75, primary }, { 0.25, -1.5, -0.75 }) // real palm
				;

			// fingers
			//Finger index({  });
			// index finger -> pinky
			// the set of animation vars that is using
			GripAnimationVars gav = pos == POSITION_LEFT ? left : right;
			for (int i = 0; i < 4; i++)
			{
				Finger f({ 0, -(1.5 + 0.125), -(0.75f - ((i + 1) * 0.225f) - (i * 0.125f)) }, pos);
				f.setAngle(gav.fingerCurrentGripRootAngle, gav.fingerCurrentGripJointAngle);
				f.draw();
			}

			// thumb
			Finger thumb({ 0, -0.75, 0.875 }, pos);
			thumb.setLength(0.375, 0.375);
			thumb.forceRotateX(pos == POSITION_RIGHT ? 90 : -90);
			thumb.setAngle(gav.thumbCurrentGripRootAngle, gav.thumbCurrentGripJointAngle);
			thumb.draw();

			// will only be equipped on right hand
			if (swordState == SWORD_EQUIPPED && pos == POSITION_RIGHT)
			{
				cv.pushMatrix();
				cv
					.translate(-0.25, -1.1, 0)
					.rotate(90, 0, 0, 1)
					.rotate(90, 1, 0, 0)
					;
				WeaponProjectionBackground::sword();
				cv.popMatrix();
			}

			// hammer only equipped on left hand
			if ((hammerState == HAMMER_EQUIPPED || hammerState == HAMMER_FLYIN || hammerState == HAMMER_FLYOUT) && pos == POSITION_LEFT)
			{
				cv.pushMatrix();
				cv
					.translate(0, 0, hammerAnimTranslateZ)
					.translate(0, -1.1, 2)
					.rotate(90, 0, 0, 1)
					.rotate(90, 1, 0, 0)
					.scale(0.25, 0.5, 0.3)
					;
				WeaponProjectionBackground::hammer();
				cv.popMatrix();
			}

			if (gunState == GUN_EQUIPPED || gunState == GUN_EQUIPPING)
			{
				cv.pushMatrix();
				cv
					.translate(pos == POSITION_RIGHT ? -0.3 : 0.3, -1.2, 0)
					.rotate(90, 1, 0, 0)
					.rotate(90, 0, 1, 0)
					.rotate(gunEquipRotation, 0, 0, 1)
					.scale(0.2, 0.2, 0.1)
					;

				WeaponGun::display();

				cv.popMatrix();
			}

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
		void grip(bool shouldGrip, GripType gt = GRIP_FULL)
		{
			// cuz need change value, need to use reference
			GripAnimationVars& gav = pos == POSITION_LEFT ? left : right;
			GripAngles ga = gt == GRIP_FULL ? fullGrip : kamekamehaGrip;
			if (shouldGrip && !gav._isGripping)
			{
				if (!gav.startGripping)
				{
					gav.startGripping = true;
					gav.fingerGripTweenProgress = 0;
					gav.startUnGripping = false;
				}
				// rest angle for all the fingers are 0
				gav.fingerLastGripRootAngle = gav.fingerCurrentGripRootAngle = tween(0, ga.fingerGripRoot, gav.fingerGripTweenProgress += 0.01);
				gav.thumbLastGripRootAngle = gav.thumbCurrentGripRootAngle = tween(0, ga.thumbGripRoot, gav.fingerGripTweenProgress);
				gav.fingerLastGripJointAngle = gav.fingerCurrentGripJointAngle = tween(0, ga.fingerGripJoint, gav.fingerGripTweenProgress);
				gav.thumbLastGripJointAngle = gav.thumbCurrentGripJointAngle = tween(0, ga.thumbGripJoint, gav.fingerGripTweenProgress);
				if (gav.fingerGripTweenProgress >= 1)
				{
					gav._isGripping = true;
					gav.startGripping = false;
					gav.fingerGripTweenProgress = 0;
					gav.fingerLastGripRootAngle = gav.fingerCurrentGripRootAngle = ga.fingerGripRoot;
					gav.thumbLastGripRootAngle = gav.thumbCurrentGripRootAngle = ga.thumbGripRoot;
					gav.fingerLastGripJointAngle = gav.fingerCurrentGripJointAngle = ga.fingerGripJoint;
					gav.thumbLastGripJointAngle = gav.thumbCurrentGripJointAngle = ga.thumbGripJoint;
				}
			}
			else if (!shouldGrip && (gav._isGripping || gav.fingerCurrentGripJointAngle != 0 || gav.thumbCurrentGripJointAngle != 0 || gav.fingerCurrentGripRootAngle != 0 || gav.thumbCurrentGripJointAngle != 0))
			{
				if (!gav.startUnGripping)
				{
					gav.startUnGripping = true;
					gav.fingerGripTweenProgress = 0;
					gav.startGripping = false;
				}
				gav.fingerCurrentGripRootAngle = tween(gav.fingerLastGripRootAngle, 0, gav.fingerGripTweenProgress += 0.01);
				gav.thumbCurrentGripRootAngle = tween(gav.thumbLastGripRootAngle, 0, gav.fingerGripTweenProgress);
				gav.fingerCurrentGripJointAngle = tween(gav.fingerLastGripJointAngle, 0, gav.fingerGripTweenProgress);
				gav.thumbCurrentGripJointAngle = tween(gav.thumbLastGripJointAngle, 0, gav.fingerGripTweenProgress);
				if (gav.fingerGripTweenProgress >= 1)
				{
					gav._isGripping = false;
					gav.startGripping = false;
					gav.fingerGripTweenProgress = 0;
					gav.fingerLastGripRootAngle = gav.fingerCurrentGripRootAngle = gav.thumbLastGripRootAngle = gav.thumbCurrentGripRootAngle = gav.fingerLastGripJointAngle = gav.fingerCurrentGripJointAngle = gav.thumbLastGripJointAngle = gav.thumbCurrentGripJointAngle = 0;
				}
			}
		}

		bool isGripping()
		{
			GripAnimationVars gav = pos == POSITION_LEFT ? left : right;
			return gav._isGripping;
		}

		void forceYRotation(float rootYRot, float jointYRot, float handYRot, float handZRot)
		{
			rootYRotation = rootYRot;
			jointYRotation = jointYRot;
			handYRotation = handYRot;
			handZRotation = handZRot;
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
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				;

			// upper leg style
			cv
				.cuboid({ -0.75, -1.3, 1.5, primary }, { 0.75, -1.5, -6 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.cuboid({ 0.75, -1.5, 1.5, primary }, { 0.75, -1.3, -6 }, { 1.5, -1, 1 }, { 1.5, -0.75, -5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.reflect(REFLECT_Y, true) // direct use reflect, lazy  rewrite d
				.replotPrevBlocky3D(GL_QUADS, primary)
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.reflect()
				;

			// "armor"
			// side way and back no need got "floating shield"
			cv
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				;

			cv
				.pushMatrix()
				// knee joint
				.translate(0, 0, -6)
				.rotate(jointAngle, -1, 0, 0)
				.sphere({ 0, 0, 0, {255, 0, 0} }, 1)
				.cuboid({ -1, 1, 1, {255, 255, 0} }, { 1, -1, 5 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				;

			cv
				.pushMatrix()
				.rotate(180, 1, 0, 0)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(270, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.cuboid({ 1, 1, -1, primary }, { 1, -1, -5 }, { 1.25, 0.5, -1.25 }, { 1.25, -0.5, -4.25 })
				.replotPrevBlocky3D(GL_LINE_LOOP, outline)
				.popMatrix()
				.popMatrix()
				;

			// leg (or shoe)
			cv
				.pushMatrix()
				.translate(0, 0, 6)
				.pushMatrix()
				.rotate(90, 0, 1, 0)
				.translate(0, 0, -1)
				.cylinder({ 0, 0, 0, lineOrJoint }, 1, 1, 2, true)
				.cylinder({ 0, 0, 0, outline }, 1, 1, 2, false, GLU_LINE, 20, 5)
				.pushMatrix() // le cross with center empty front
				.translate(0, 0, 2.1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.pushMatrix()
				.rotate(270, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.popMatrix() // le cross with center empty front
				.pushMatrix() // le cross with center empty back
				.translate(0, 0, -0.1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.pushMatrix()
				.rotate(90, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.pushMatrix()
				.rotate(180, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.pushMatrix()
				.rotate(270, 0, 0, 1)
				.rect({ -0.1, 0.8, secondary }, { 0.1, 0.2 })
				.popMatrix()
				.popMatrix() // le cross with center empty back
				.popMatrix()
				.pushMatrix()
				.rotate(45, -1, 0, 0)
				.cuboid({ -1, 1, 4, primary }, { 1, 1, 1 }, { -1, -1, 2 }, { 1, -1, 1 })
				.pushMatrix()
				.translate(0, 1.1, 2.5)
				.rotate(90, 1, 0, 0)
				.circle({ 0, 0, secondary }, 0.75)
				.popMatrix()
				.pushMatrix()
				.translate(0, 1.15, 2.5)
				.rotate(90, 1, 0, 0)
				.circle({ 0, 0, primary }, 0.5)
				.popMatrix()
				.popMatrix()
				.cuboid({ -1, 3.5215, 2, lineOrJoint }, { 1, 0.7, 3 })
				.cuboid({ -1, 1, 1, primary }, { 1, -1, 2 })
				.cuboid({ -1, -1, -1, lineOrJoint }, { 1, -2, 3 })
				.cuboid({ -1, -2, -1, primary }, { 1, -2, 1 }, { -1, -4, -4 }, { 1, -4, -4 })
				.pushMatrix()
				.rotate(20, 0, 0, 1)
				.cuboid({ -1, -2, -1, accent }, { 0.5, -2, 1 }, { -1, -4, -4 }, { 0.5, -4, -4 })
				.popMatrix()
				.pushMatrix()
				.rotate(20, 0, 0, -1)
				.cuboid({ -0.5, -2, -1, accent }, { 1, -2, 1 }, { -0.5, -4, -4 }, { 1, -4, -4 })
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

	// these restTargets all terbalik d, bear with me, for leg targets -> right = left, left = right
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
	float leftHandPalmRestYRotation = 0;
	float leftHandPalmRestZRotation = 0;
	float leftHandRootYRotation = leftHandRootRestYRotation;
	float leftHandJointYRotation = leftHandJointRestYRotation;
	float leftHandPalmYRotation = leftHandPalmRestYRotation;
	float leftHandPalmZRotation = leftHandPalmRestZRotation;

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
	float rightHandPalmRestYRotation = 0;
	float rightHandPalmRestZRotation = 0;
	float rightHandRootYRotation = rightHandRootRestYRotation;
	float rightHandJointYRotation = rightHandJointRestYRotation;
	float rightHandPalmYRotation = rightHandPalmRestYRotation;
	float rightHandPalmZRotation = rightHandPalmZRotation;

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
	bool rightHandShouldGrip = false;
	bool leftHandShouldGrip = false;
	GripType rightHandGripType = GRIP_KAMEKAMEHA;
	GripType leftHandGripType = GRIP_KAMEKAMEHA;

	// the "customization feature" to allow the "director" to customize every hand and leg movement
	bool inEditMode = false; // extreme customization, TAB to toggle, FUTURE_TODO: add finger grip angle customization
	EditModeEditTargets editModeTarget = EDIT_RIGHT_HAND;
	// for inverse kinematic customizations
	Point3D rightHandTargetDebug = { rightHandRestTarget.x, rightHandRestTarget.y, rightHandRestTarget.z, { 255, 255, 255} };
	Point3D leftHandTargetDebug = { leftHandRestTarget.x, leftHandRestTarget.y, leftHandRestTarget.z, { 255, 255, 255} };
	Point3D rightLegTargetDebug = { rightLegRestTarget.x, rightLegRestTarget.y, rightLegRestTarget.z, { 255, 255, 255} };
	Point3D leftLegTargetDebug = { leftLegRestTarget.x, leftLegRestTarget.y, leftLegRestTarget.z, { 255, 255, 255} };
	float rightHandJointYDebug = 0, rightHandRootYDebug = 0, rightHandPalmYDebug = 0, rightHandPalmZDebug = 0;
	float leftHandJointYDebug = 0, leftHandRootYDebug = 0, leftHandPalmYDebug = 0, leftHandPalmZDebug = 0;
	Point3D rightHandTargetDebugRecord = rightHandTargetDebug;
	Point3D leftHandTargetDebugRecord = leftHandTargetDebug;
	Point3D rightLegTargetDebugRecord = rightLegTargetDebug;
	Point3D leftLegTargetDebugRecord = leftLegTargetDebug;
	Point3D rightHandPrevTargetDebug = rightHandTargetDebug;
	Point3D leftHandPrevTargetDebug = leftHandTargetDebug;
	Point3D rightLegPrevTargetDebug = rightLegTargetDebug;
	Point3D leftLegPrevTargetDebug = leftLegTargetDebug;
	float rightHandJointYDebugRecord = 0, rightHandRootYDebugRecord = 0, rightHandPalmYDebugRecord = 0, rightHandPalmZDebugRecord = 0;
	float leftHandJointYDebugRecord = 0, leftHandRootYDebugRecord = 0, leftHandPalmYDebugRecord = 0, leftHandPalmZDebugRecord = 0;
	float
		rightHandPrevJointYDebug = rightHandJointYDebug = 0,
		rightHandPrevRootYDebug = rightHandRootYDebug = 0,
		rightHandPrevPalmYDebug = rightHandPalmYDebug = 0,
		rightHandPrevPalmZDebug = rightHandPalmZDebug = 0;
	float
		leftHandPrevJointYDebug = leftHandJointYDebug = 0,
		leftHandPrevRootYDebug = leftHandRootYDebug = 0,
		leftHandPrevPalmYDebug = leftHandPalmYDebug = 0,
		leftHandPrevPalmZDebug = leftHandPalmZDebug = 0;

	bool debugPlayBack = false;
	float targetDebugTween = 0;

	bool isWalking = false;
	float walkingTweenProgress = 0;
	float stopWalkingTweenProgress = 0;
	float bodyCurrentWalkRotation = 0, maxBodyWalkRotation = 10, lastRotationBeforeStopWalking = 0;

	Transform shieldActiveState;
	float shieldActiveRotateY = 0; // both shield should be same

	// cuz cannot execute set value outside of a function
	void initWeaponActiveStates()
	{
		shieldActiveState.rotAngle = 0;
		shieldActiveState.transY = 0;
		shieldActiveState.transZ = 7;
		shieldActiveState.transX = 4;
		shieldActiveState.scaleX = shieldActiveState.scaleY = shieldActiveState.scaleZ = 1;
	}

	Transform shieldRestState;
	float shieldRestRotateY = 90; // both shield should be same

	Transform shieldCurrentState = shieldRestState;
	Transform shieldLastState;
	float shieldCurrentRotateY = shieldRestRotateY;
	float shieldLastRotateY = shieldCurrentRotateY;

	Transform swordUnequipFlyOutDest;
	Transform swordUnequipAnim; // also used for idle states
	Transform swordUnequipFlyInFrom;
	Point3D hammerEquipUnequipHandTarget = { leftHandRestTarget.x, 10, 7 };

	// cuz cannot execute set value outside of a function
	void initWeaponRestAndOriStates()
	{
		shieldCurrentState.rotAngle = shieldRestState.rotAngle = 45;
		shieldCurrentState.transY = shieldRestState.transY = 1;
		shieldCurrentState.transZ = shieldRestState.transZ = 14;
		shieldCurrentState.transX = shieldRestState.transX = 0;
		shieldCurrentState.scaleX = shieldRestState.scaleX = 0.5;
		shieldCurrentState.scaleY = shieldCurrentState.scaleZ = shieldRestState.scaleY = shieldRestState.scaleZ = 0.75;

		// sword states
		swordUnequipFlyOutDest.transX = -20;
		swordUnequipFlyOutDest.transY = 20;
		swordUnequipFlyInFrom.transX = -8;
		swordUnequipFlyInFrom.transY = 25;
	}

	bool setShieldActive = false;
	bool shieldActivating = false;
	bool shieldActivated = false;
	float shieldActivateTweenProgress = 0;
	float shieldUnactivateTweenProgress = 0;

	Point3D swordReleaseHandTarget = { rightHandRestTarget.x, rightHandRestTarget.y + 4, rightHandRestTarget.z + 5, { 255, 255, 255} };
	float swordReleasePalmRotation = 30;

	bool hideUnequippedWeapon = false;

	bool setSwordEquip = false;
	float swordTween = 0;

	bool setHammerEquip = false;
	float hammerTween = 0;

	bool setGunEquip = false;
	float gunEquipUnequipTween = 0;
	float gunEquipPrevRotation = 0;
	Point3D rightHandGunEquipUnequipTarget = { rightHandRestTarget.x, 3, 5 }; // temp
	Point3D leftHandGunEquipUnequipTarget = { leftHandRestTarget.x, 3, 5 };

	Point3D attackWithSwordVerStartSwingTarget = { rightHandRestTarget.x, rightHandRestTarget.y + 9.0f, rightHandRestTarget.z + 8.0f }; // start swinging target
	Point3D attackWithSwordVerSwingTillTarget = { rightHandRestTarget.x, rightHandRestTarget.y + 2.5f, rightHandRestTarget.z + 8.0f }; // max forward swing action
	Point3D attackWithSwordVerSwingOvershootTarget = { rightHandRestTarget.x, rightHandRestTarget.y - 1.0f, rightHandRestTarget.z - 6.0f }; // max forward swing action

	Point3D attackWithSwordHorStartSwingTarget = { rightHandRestTarget.x, 2, 6 };
	float attackWithSwordHorStartSwingJointAngle = 50;
	Point3D attackWithSwordHorSwingTillTarget = { rightHandRestTarget.x, -1, 4 };
	float attackWithSwordHorSwingTillJointAngle = 45;
	// when overshoot joint no rotate
	Point3D attackWithSwordHorSwingOvershootTarget = { rightHandRestTarget.x, -4, -5 };

	bool attackWithSword = false;
	SwordAttackTypes attackWithSwordType = SWORD_ATK_HOR; // alternate attacking styles
	AttackWithSwordAnimState currentAttackSwordAnimState = SWORD_ATK_IDLE;
	float attackWithSwordTween = 0;
	NowAnimating animating = ANIMATING_NONE; // if is animating other stuff, cannot walk

	Point3D rightkkhChargeTarget = { rightHandRestTarget.x, 1, 5 };
	Point3D leftkkhChargeTarget = { leftHandRestTarget.x, 1, 5 };
	// for use tween back
	Point3D rightkkhLastChargeTarget = rightHandRestTarget;
	Point3D leftkkhLastChargeTarget = leftHandRestTarget;
	KamekamehaChargeProgress kkhChargeState = KKH_NONE;
	Color kkhOriBola = { 0, 0, 0 };
	Color kkhLowBola = { 0, 255, 0};
	Color kkhMedBola = { 255, 255, 0 };
	Color kkhHighBola = { 255, 0, 0 };
	Color kkhPrevColor;
	Color kkhBolaCurrentColor;
	float rightkkhPrevRootRotation = 0, leftkkhPrevRootRotation = 0;
	float kkhOriRotateRoot = 30, kkhLowRotateRoot = 25, kkhMedRotateRoot = 20, kkhHighRotateRoot = 15;
	Transform kkhBolaScale; // for use when make the bola slowly get bigger
	Transform kkhBolaPrevScale;
	float kkhBeamLength = 10, kkhPrevBeamLength = 0;
	float kkhTopRad = 3, kkhPrevTopRad = 0;
	bool kamekamehaCharging = false;
	float kkhTween = 0;

	Point3D attackWithHammerStartSwingTarget = { leftHandRestTarget.x, 8, 4 };
	Point3D attackWithHammerSwingTillTarget = { leftHandRestTarget.x, 1, 6 };
	Point3D attackWithHammerOvershootTarget = { leftHandRestTarget.x, -3, -4 };

	bool attackWithHammer = false;
	float attackWithHammerTween = 0;
	AttackWithHammerAnimState attackHammerState = HAMMER_ATK_IDLE;

	bool gunShoot = false;
	float leftGunShootTween = 0;
	float leftGunRecoilTween = 0;
	float rightGunShootTween = 0;
	float rightGunRecoilTween = 0;
	float stopGunShootTween = 0;
	GunShootingStates leftGunShootingState = GUN_READY;
	GunShootingStates rightGunShootingState = GUN_READY;
	GunShootHand currentGun = LEFT_GUN;
	Point3D leftGunRecoilTarget = { leftHandRestTarget.x, 6, 4 };
	Point3D rightGunRecoilTarget = { rightHandRestTarget.x, 6, 4 };
	Point3D leftGunPrevTarget = leftHandRestTarget;
	Point3D rightGunPrevTarget = rightHandRestTarget;
	Transform leftGunBullet;
	Transform rightGunBullet;

	void main()
	{
		if (debugPlayBack && inEditMode && animating == DEBUG_PLAYBACK)
		{
			rightHandTargetDebug = tween(rightHandPrevTargetDebug, rightHandTargetDebugRecord, targetDebugTween += 0.05);
			leftHandTargetDebug = tween(leftHandPrevTargetDebug, leftHandTargetDebugRecord, targetDebugTween);
			rightLegTargetDebug = tween(rightLegPrevTargetDebug, rightLegTargetDebugRecord, targetDebugTween);
			leftLegTargetDebug = tween(leftLegPrevTargetDebug, leftLegTargetDebugRecord, targetDebugTween);

			rightHandJointYDebug = tween(rightHandPrevJointYDebug, rightHandJointYDebugRecord, targetDebugTween);
			rightHandRootYDebug = tween(rightHandPrevRootYDebug, rightHandRootYDebugRecord, targetDebugTween);
			rightHandPalmYDebug = tween(rightHandPrevPalmYDebug, rightHandPalmYDebugRecord, targetDebugTween);
			rightHandPalmZDebug = tween(rightHandPrevPalmZDebug, rightHandPalmZDebugRecord, targetDebugTween);
			
			leftHandJointYDebug = tween(leftHandPrevJointYDebug, leftHandJointYDebugRecord, targetDebugTween);
			leftHandRootYDebug = tween(leftHandPrevRootYDebug, leftHandRootYDebugRecord, targetDebugTween);
			leftHandPalmYDebug = tween(leftHandPrevPalmYDebug, leftHandPalmYDebugRecord, targetDebugTween);
			leftHandPalmZDebug = tween(leftHandPrevPalmZDebug, leftHandPalmZDebugRecord, targetDebugTween);

			if (targetDebugTween >= 1)
			{
				targetDebugTween = 0;
				rightHandTargetDebug = rightHandTargetDebugRecord;
				leftHandTargetDebug = leftHandTargetDebugRecord;
				rightLegTargetDebug = rightLegTargetDebugRecord;
				leftLegTargetDebug = leftLegTargetDebugRecord;

				rightHandJointYDebug = rightHandJointYDebugRecord;
				rightHandRootYDebug = rightHandRootYDebugRecord;
				rightHandPalmYDebug = rightHandPalmYDebugRecord;
				rightHandPalmZDebug = rightHandPalmZDebugRecord;
				
				leftHandJointYDebug = leftHandJointYDebugRecord;
				leftHandRootYDebug = leftHandRootYDebugRecord;
				leftHandPalmYDebug = leftHandPalmYDebugRecord;
				leftHandPalmZDebug = leftHandPalmZDebugRecord;

				animating = ANIMATING_NONE;
				debugPlayBack = false;
			}
		}

		if (isWalking && animating == WALK)
		{
			// hmmmmmm maybe when walking hands should be gripped
			leftHandShouldGrip = rightHandShouldGrip = true;

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
			&& (animating == WALK)
			)
		{
			// ungrip
			leftHandShouldGrip = false;
			// right hand ungrip or not should be decided by whether is holding a sword
			rightHandShouldGrip = swordState == SWORD_EQUIPPED;
			
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
				animating = ANIMATING_NONE;
			}
		}

		// when equipping sword cannot activate otherwise will crash (sword too long, thanks soon chee ;))
		// when hiding unequipped weapon, should not be able to do anything with shields
		if (setShieldActive && !shieldActivated) 
		{
			if (!shieldActivating)
			{
				shieldActivateTweenProgress = 0;
				shieldActivating = true;
			}
			shieldLastState.rotAngle = shieldCurrentState.rotAngle = tween(shieldRestState.rotAngle, shieldActiveState.rotAngle, shieldActivateTweenProgress += 0.01);
			shieldLastState.transX = shieldCurrentState.transX = tween(shieldRestState.transX, shieldActiveState.transX, shieldActivateTweenProgress);
			shieldLastState.transY = shieldCurrentState.transY = tween(shieldRestState.transY, shieldActiveState.transY, shieldActivateTweenProgress);
			shieldLastState.transZ = shieldCurrentState.transZ = tween(shieldRestState.transZ, shieldActiveState.transZ, shieldActivateTweenProgress);
			shieldCurrentState.scaleX = shieldLastState.scaleX = tween(shieldRestState.scaleX, shieldActiveState.scaleX, shieldActivateTweenProgress);
			shieldCurrentState.scaleY = shieldCurrentState.scaleZ
				= shieldLastState.scaleY = shieldLastState.scaleZ 
				= tween(shieldRestState.scaleY, shieldActiveState.scaleY, shieldActivateTweenProgress); // luckily all side scale same
			shieldLastRotateY = shieldCurrentRotateY = tween(shieldRestRotateY, shieldActiveRotateY, shieldActivateTweenProgress);

			if (shieldActivateTweenProgress >= 1)
			{
				shieldActivated = true;
				shieldActivating = false;
				shieldActivateTweenProgress = 0;
				shieldUnactivateTweenProgress = 0;
				shieldLastState = shieldCurrentState = shieldActiveState;
				shieldLastRotateY = shieldCurrentRotateY = shieldActiveRotateY;
				// ehh? why no set animating back to none??? cuz when shield is activated, ntg else should be able to run
			}
		}
		if (!setShieldActive && (shieldActivated || shieldCurrentState != shieldRestState || shieldCurrentRotateY != shieldRestRotateY))
		{
			if (shieldActivating)
			{
				shieldActivateTweenProgress = 0;
				shieldUnactivateTweenProgress = 0;
				shieldActivating = false;
			}
			shieldCurrentState.rotAngle = tween(shieldLastState.rotAngle, shieldRestState.rotAngle, shieldUnactivateTweenProgress += 0.01);
			shieldCurrentState.transX = tween(shieldLastState.transX, shieldRestState.transX, shieldUnactivateTweenProgress);
			shieldCurrentState.transY = tween(shieldLastState.transY, shieldRestState.transY, shieldUnactivateTweenProgress);
			shieldCurrentState.transZ = tween(shieldLastState.transZ, shieldRestState.transZ, shieldUnactivateTweenProgress);
			shieldCurrentState.scaleX = tween(shieldLastState.scaleX, shieldRestState.scaleX, shieldUnactivateTweenProgress); 
			shieldCurrentState.scaleY = shieldCurrentState.scaleZ
				= tween(shieldLastState.scaleY, shieldRestState.scaleY, shieldUnactivateTweenProgress); // luckily all side scale same
			shieldCurrentRotateY = tween(shieldLastRotateY, shieldRestRotateY, shieldUnactivateTweenProgress);

			if (shieldUnactivateTweenProgress >= 1)
			{
				shieldActivated = false;
				shieldActivateTweenProgress = 0;
				shieldUnactivateTweenProgress = 0;
				shieldLastState = shieldCurrentState = shieldRestState;
				shieldLastRotateY = shieldCurrentRotateY = shieldRestRotateY;
				animating = ANIMATING_NONE;
			}
		}

		cv.pushMatrix().translate(0, 10, 0);
		Head();
		cv.popMatrix();

		// create the hands, but don't draw yet cuz transform matrix diff
		// idk what my brain thinking when i code but apparently my right hand is left hand d now
		Hand rightHand({ 5, 8, 0 });
		Hand leftHand({ -5, 8, 0 }, POSITION_LEFT);

		// equipping a sword (cannot equip when gun equipped)
		if (setSwordEquip && gunState != GUN_EQUIPPED)
		{
			if (swordState == SWORD_UNEQUIP_IDLE)
			{
				// ntg special, proceed to next state
				swordState = SWORD_EQUIP_FLYOUT;
			}
			if (swordState == SWORD_EQUIP_FLYOUT)
			{
				// sword flyout from idle position to the "flyin" position
				// when fly out eh time hand can be ready to fetch the sword d
				swordUnequipAnim.transX = tween(0, swordUnequipFlyInFrom.transX, swordTween += 0.005);
				swordUnequipAnim.transY = tween(0, swordUnequipFlyInFrom.transY, swordTween);
				swordUnequipAnim.transZ = tween(0, swordUnequipFlyInFrom.transZ, swordTween);

				rightHandCurrentTarget = tween(rightHandRestTarget, swordReleaseHandTarget, swordTween); 
				rightHandPalmYRotation = tween(rightHandPalmRestYRotation, swordReleasePalmRotation, swordTween);
				if (swordTween >= 1)
				{
					swordTween = 0;
					swordUnequipAnim.transX = swordUnequipFlyInFrom.transX;
					swordUnequipAnim.transY = swordUnequipFlyInFrom.transY;
					swordUnequipAnim.transZ = swordUnequipFlyInFrom.transZ;

					rightHandCurrentTarget = swordReleaseHandTarget;
					rightHandPalmYRotation = swordReleasePalmRotation;
					swordState = SWORD_EQUIP_FLYIN;
				}
			}
			if (swordState == SWORD_EQUIP_FLYIN)
			{
				swordUnequipAnim.transX = tween(swordUnequipFlyOutDest.transX, 0, swordTween += 0.005);
				swordUnequipAnim.transY = tween(swordUnequipFlyOutDest.transY, 0, swordTween);
				swordUnequipAnim.transZ = tween(swordUnequipFlyOutDest.transZ, 0, swordTween);

				if (swordTween >= 1)
				{
					swordTween = 0;
					swordUnequipAnim.transX = swordUnequipAnim.transY = swordUnequipAnim.transZ = 0;
					// go to next state
					swordState = SWORD_EQUIPPED;
				}
			}
			if (swordState == SWORD_EQUIPPED && animating == SWORD_EQUIP_UNEQUIP)
			{
				// grip the sword then move back to rest position
				if (!rightHand.isGripping())
				{
					rightHandShouldGrip = true;
					rightHandGripType = GRIP_FULL;
				}
				else if (rightHand.isGripping() && (rightHandCurrentTarget != rightHandRestTarget || rightHandPalmYRotation != rightHandPalmRestYRotation)) // if finally gripped then can move back to rest position
				{
					rightHandCurrentTarget = tween(swordReleaseHandTarget, rightHandRestTarget, swordTween += 0.005); 
					rightHandPalmYRotation = tween(swordReleasePalmRotation, rightHandPalmRestYRotation, swordTween);
					if (swordTween >= 1)
					{
						swordTween = 0;
						rightHandCurrentTarget = rightHandRestTarget;
						rightHandPalmYRotation = rightHandPalmRestYRotation;
						animating = ANIMATING_NONE; // officially finished
					}
				}
			}
		}
		// unequipping a sword
		if (!setSwordEquip)
		{
			if (swordState == SWORD_EQUIPPED)
			{
				if (swordTween >= 1 && rightHand.isGripping())
				{
					swordTween = 1;
					rightHandCurrentTarget = swordReleaseHandTarget;
					rightHandPalmYRotation = swordReleasePalmRotation;
					// once reach, then start ungrip
					// call for ungrip
					rightHandShouldGrip = false;
				}
				// once right hand target at position and ungrip finish
				else if (swordTween >= 1 && !rightHand.isGripping())
				{
					swordTween = 0;
					swordState = SWORD_UNEQUIP_FLYOUT;
				}
				else 
				{
					// shud be in rest position for right hand when want unequip
					rightHandCurrentTarget = tween(rightHandRestTarget, swordReleaseHandTarget, swordTween);
					rightHandPalmYRotation = tween(rightHandPalmRestYRotation, swordReleasePalmRotation, swordTween);
					swordTween += 0.005;
				}
			}
			if (swordState == SWORD_UNEQUIP_FLYOUT)
			{
				swordUnequipAnim.transX = tween(0, swordUnequipFlyOutDest.transX, swordTween += 0.005);
				swordUnequipAnim.transY = tween(0, swordUnequipFlyOutDest.transY, swordTween);
				swordUnequipAnim.transZ = tween(0, swordUnequipFlyOutDest.transZ, swordTween);
				// when flying out the hand can return to ori rest position d
				rightHandCurrentTarget = tween(swordReleaseHandTarget, rightHandRestTarget, swordTween); 
				rightHandPalmYRotation = tween(swordReleasePalmRotation, rightHandPalmRestYRotation, swordTween);
				if (swordTween >= 1)
				{
					swordTween = 0;
					swordUnequipAnim.transX = swordUnequipFlyOutDest.transX;
					swordUnequipAnim.transY = swordUnequipFlyOutDest.transY;
					swordUnequipAnim.transZ = swordUnequipFlyOutDest.transZ;

					rightHandCurrentTarget = rightHandRestTarget;
					rightHandPalmYRotation = rightHandPalmRestYRotation;
					swordState = SWORD_UNEQUIP_FLYIN;
				}	
			}
			if (swordState == SWORD_UNEQUIP_FLYIN)
			{
				swordUnequipAnim.transX = tween(swordUnequipFlyInFrom.transX, 0, swordTween += 0.005);
				swordUnequipAnim.transY = tween(swordUnequipFlyInFrom.transY, 0, swordTween);
				swordUnequipAnim.transZ = tween(swordUnequipFlyInFrom.transZ, 0, swordTween);
				if (swordTween >= 1)
				{
					swordTween = 0;
					swordUnequipAnim.transX = 0;
					swordUnequipAnim.transY = 0;
					swordUnequipAnim.transZ = 0;
					swordState = SWORD_UNEQUIP_IDLE;
					// finish whole sequence d
					animating = ANIMATING_NONE;
				}
			}
		}

		// hammer won't float around when unequipped, so can still equip/unequip when unequipped weapons are hidden
		// but only can equip if guns are not equipped
		if (setHammerEquip && gunState != GUN_EQUIPPED)
		{
			if (hammerState == HAMMER_UNEQUIPPED)
			{
				// move left hand to target first
				leftHandCurrentTarget = tween(leftHandRestTarget, hammerEquipUnequipHandTarget, hammerTween += 0.05);
				if (hammerTween >= 1)
				{
					hammerTween = 0;
					leftHandCurrentTarget = hammerEquipUnequipHandTarget;
					hammerState = HAMMER_FLYIN;
				}
			}
			if (hammerState == HAMMER_FLYIN)
			{
				hammerAnimTranslateZ = tween(hammerAnimFlyInFromFlyOutTo, 0, hammerTween += 0.05);
				if (hammerTween >= 1)
				{
					hammerTween = 0;
					hammerAnimTranslateZ = 0;
					hammerState = HAMMER_EQUIPPED;
				}
			}
			if (hammerState == HAMMER_EQUIPPED && animating == HAMMER_EQUIP_UNEQUIP)
			{
				if (!leftHand.isGripping())
				{
					leftHandShouldGrip = true;
					leftHandGripType = GRIP_FULL;
				}
				else if (leftHand.isGripping() && leftHandCurrentTarget != leftHandRestTarget)
				{
					// go back rest target
					leftHandCurrentTarget = tween(hammerEquipUnequipHandTarget, leftHandRestTarget, hammerTween += 0.05);
					if (hammerTween >= 1)
					{
						hammerTween = 0;
						leftHandCurrentTarget = leftHandRestTarget;
						animating = ANIMATING_NONE;
					}
				}
			}
		}
		// unequip hammer
		if (!setHammerEquip)
		{
			if (hammerState == HAMMER_EQUIPPED)
			{
				// move to release position and ungrip
				if (leftHandCurrentTarget != hammerEquipUnequipHandTarget)
				{
					leftHandCurrentTarget = tween(leftHandRestTarget, hammerEquipUnequipHandTarget, hammerTween += 0.05);
					if (hammerTween >= 1)
					{
						hammerTween = 0;
						leftHandCurrentTarget = hammerEquipUnequipHandTarget;
						hammerState = HAMMER_FLYOUT;
					}
				}
			}

			if (hammerState == HAMMER_FLYOUT)
			{
				if (leftHand.isGripping())
				{
					leftHandShouldGrip = false;
				}
				else
				{
					hammerAnimTranslateZ = tween(0, hammerAnimFlyInFromFlyOutTo, hammerTween += 0.05);
					if (hammerTween >= 1)
					{
						hammerTween = 0;
						hammerAnimTranslateZ = hammerAnimFlyInFromFlyOutTo;
						hammerState = HAMMER_UNEQUIPPED;
					}
				}
			}

			if (hammerState == HAMMER_UNEQUIPPED && animating == HAMMER_EQUIP_UNEQUIP)
			{
				// go back rest target
				leftHandCurrentTarget = tween(hammerEquipUnequipHandTarget, leftHandRestTarget, hammerTween += 0.05);
				if (hammerTween >= 1)
				{
					hammerTween = 0;
					leftHandCurrentTarget = leftHandRestTarget;
					animating = ANIMATING_NONE;
				}
			}
		}

		// only not equipping sword AND hammer ka can equip gun
		// no need test for shield becuz when shield activated nothing else can run
		if (setGunEquip && swordState != SWORD_EQUIPPED && hammerState != HAMMER_EQUIPPED)
		{
			if (gunState == GUN_UNEQUIPPED)
			{
				// move both hand to target
				rightHandCurrentTarget = tween(rightHandRestTarget, rightHandGunEquipUnequipTarget, gunEquipUnequipTween += 0.05);
				leftHandCurrentTarget = tween(leftHandRestTarget, leftHandGunEquipUnequipTarget, gunEquipUnequipTween);
				if (gunEquipUnequipTween >= 1)
				{
					gunEquipUnequipTween = 0;
					rightHandCurrentTarget = rightHandGunEquipUnequipTarget;
					leftHandCurrentTarget = leftHandGunEquipUnequipTarget;
					gunState = GUN_EQUIPPING;
				}
			}

			if (gunState == GUN_EQUIPPING)
			{
				// while not finished gripping keep spinning the gun
				if (!leftHand.isGripping() || !rightHand.isGripping())
				{
					leftHandShouldGrip = rightHandShouldGrip = true;
					leftHandGripType = rightHandGripType = GRIP_FULL;
					gunEquipPrevRotation = gunEquipRotation += 10;
					if (gunEquipRotation >= 360)
						gunEquipPrevRotation = gunEquipRotation = 0;
				}
				else if (leftHand.isGripping() && rightHand.isGripping())
				{
					gunEquipRotation = tween(gunEquipPrevRotation, 0, gunEquipUnequipTween += 0.075);
					if (gunEquipUnequipTween >= 1)
					{
						gunEquipRotation = 0;
						gunEquipUnequipTween = 0;
						gunState = GUN_EQUIPPED;
						animating = ANIMATING_NONE;
					}
				}
			}
		}
		else if (setGunEquip && (swordState == SWORD_EQUIPPED || hammerState == HAMMER_EQUIPPED))
		{
			// free up the animation slot
			animating = ANIMATING_NONE;
			setGunEquip = false;
		}
		// unequip gun
		if (!setGunEquip)
		{
			if (gunState == GUN_EQUIPPED)
			{
				// rotate the gun while ungripping it
				if (leftHand.isGripping() || rightHand.isGripping())
				{
					leftHandShouldGrip = false;
					rightHandShouldGrip = false;
					gunEquipPrevRotation = gunEquipRotation += 10;
					if (gunEquipRotation >= 360)
						gunEquipPrevRotation = gunEquipRotation = 0;
				}
				else if (!leftHand.isGripping() && !rightHand.isGripping())
				{
					// tween gun to face down and make it disappear
					gunEquipRotation = tween(gunEquipPrevRotation, -90, gunEquipUnequipTween += 0.075);
					if (gunEquipUnequipTween >= 1)
					{
						gunEquipUnequipTween = 0;
						gunEquipRotation = -90;
						gunState = GUN_UNEQUIPPING;
					}
				}
			}

			if (gunState == GUN_UNEQUIPPING)
			{
				// tween hands back to rest and consider whole animation done
				rightHandCurrentTarget = tween(rightHandGunEquipUnequipTarget, rightHandRestTarget, gunEquipUnequipTween += 0.05);
				leftHandCurrentTarget = tween(leftHandGunEquipUnequipTarget, leftHandRestTarget, gunEquipUnequipTween += 0.05);
				if (gunEquipUnequipTween >= 1)
				{
					gunEquipUnequipTween = 0;
					rightHandCurrentTarget = rightHandRestTarget;
					leftHandCurrentTarget = leftHandRestTarget;
					gunState = GUN_UNEQUIPPED;
					animating = ANIMATING_NONE;
				}
			}
		}

		// when equipping flying in, also need to show this
		if (swordState == SWORD_UNEQUIP_FLYOUT || swordState == SWORD_EQUIP_FLYIN)
		{
			// when hand gripping wan release back
			cv
				.pushMatrix()
				.translate(swordUnequipAnim.transX, swordUnequipAnim.transY, swordUnequipAnim.transZ) // when animate change this
				.translate(4.75, 0, 0)
				.rotate(23, 1, 0, 0)
				.translate(0, 2, 6)
				.rotate(90, 0, 1, 0)
				.rotate(30, -1, 0, 0)
				;
			WeaponProjectionBackground::sword();
			cv.popMatrix();
		}

		cv
			.pushMatrix() // upper body rotate when walking
			.rotate(bodyCurrentWalkRotation, 0, 1, 0)
			;
		// upper body
		cv
			.cuboid(
				{ -1.5, 3, 1.5, primary },
				{ 1.5, 3, -1.5 },
				{ -0.5, 2, 0.5 },
				{ 0.5, 2, -0.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid(
				{ -2.5, 4, 2, primary },
				{ 2.5, 4, -2 },
				{ -1.5, 3, 1.5 },
				{ 1.5, 3, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid({ -2.5, 6, 2, primary }, { 2.5, 4, -2 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid(
				{ -2, 7, 1.5, primary },
				{ 2, 7, -1.5 },
				{ -2.5, 6, 2 },
				{ 2.5, 6, -2 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid(
				{ -1, 8, 1, primary },
				{ 1, 8, -1 },
				{ -2, 7, 1.5 },
				{ 2, 7, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			;

		// sword by default float behind body, when fly out to become equipped also need show
		if (!hideUnequippedWeapon)
		{
			if (swordState == SWORD_UNEQUIP_FLYIN || swordState == SWORD_UNEQUIP_IDLE || swordState == SWORD_EQUIP_FLYOUT)
		{
			cv
				.pushMatrix()
				.translate(swordUnequipAnim.transX, swordUnequipAnim.transY, swordUnequipAnim.transZ) // when animate change this
				.rotate(15, 0, 0, 1)
				.translate(0, 9, -4)
				.rotate(180, 0, 0, 1)
				;
			WeaponProjectionBackground::sword();
			cv.popMatrix();
		}
		}

		// only when equipped with sword can activate this
		if (attackWithSword && swordState == SWORD_EQUIPPED && animating == ATTACK_WITH_SWORD)
		{
			// when attacking vertically
			if (attackWithSwordType == SWORD_ATK_VER)
			{
				// if IDLE then hand ***SHOULD*** be in rest position
				if (currentAttackSwordAnimState == SWORD_ATK_IDLE)
				{
					// move hand to the pre-swing target
					rightHandCurrentTarget = tween(rightHandRestTarget, attackWithSwordVerStartSwingTarget, attackWithSwordTween += 0.005);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordVerStartSwingTarget;
						currentAttackSwordAnimState = SWORD_ATK_START_SWING; // move to next state
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_START_SWING)
				{
					// swing until max in front
					rightHandCurrentTarget = tween(attackWithSwordVerStartSwingTarget, attackWithSwordVerSwingTillTarget, attackWithSwordTween += 0.05);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordVerSwingTillTarget;
						currentAttackSwordAnimState = SWORD_ATK_FINISH_SWING;
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_FINISH_SWING)
				{
					rightHandCurrentTarget = tween(attackWithSwordVerSwingTillTarget, attackWithSwordVerSwingOvershootTarget, attackWithSwordTween += 0.05);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordVerSwingOvershootTarget;
						currentAttackSwordAnimState = SWORD_ATK_SWING_OVERSHOOT;
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_SWING_OVERSHOOT)
				{
					// swing back to rest target
					rightHandCurrentTarget = tween(attackWithSwordVerSwingOvershootTarget, rightHandRestTarget, attackWithSwordTween += 0.01);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = rightHandRestTarget;
						currentAttackSwordAnimState = SWORD_ATK_IDLE;
						animating = ANIMATING_NONE;
					}
				}
			}
		
			if (attackWithSwordType == SWORD_ATK_HOR)
			{
				// if IDLE then hand ***SHOULD*** be in rest position
				if (currentAttackSwordAnimState == SWORD_ATK_IDLE)
				{
					// move hand to the pre-swing target
					rightHandCurrentTarget = tween(rightHandRestTarget, attackWithSwordHorStartSwingTarget, attackWithSwordTween += 0.005);
					rightHandJointYRotation = tween(rightHandJointRestYRotation, attackWithSwordHorStartSwingJointAngle, attackWithSwordTween);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordVerStartSwingTarget;
						rightHandJointYRotation = attackWithSwordHorStartSwingJointAngle;
						currentAttackSwordAnimState = SWORD_ATK_START_SWING; // move to next state
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_START_SWING)
				{
					// swing until max in front
					rightHandCurrentTarget = tween(attackWithSwordHorStartSwingTarget, attackWithSwordHorSwingTillTarget, attackWithSwordTween += 0.05);
					rightHandJointYRotation = tween(attackWithSwordHorStartSwingJointAngle, attackWithSwordHorSwingTillJointAngle, attackWithSwordTween);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordHorSwingTillTarget;
						rightHandJointYRotation = attackWithSwordHorSwingTillJointAngle;
						currentAttackSwordAnimState = SWORD_ATK_FINISH_SWING;
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_FINISH_SWING)
				{
					rightHandCurrentTarget = tween(attackWithSwordHorSwingTillTarget, attackWithSwordHorSwingOvershootTarget, attackWithSwordTween += 0.05);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = attackWithSwordHorSwingOvershootTarget;
						currentAttackSwordAnimState = SWORD_ATK_SWING_OVERSHOOT;
					}
				}

				if (currentAttackSwordAnimState == SWORD_ATK_SWING_OVERSHOOT)
				{
					// swing back to rest target
					rightHandCurrentTarget = tween(attackWithSwordHorSwingOvershootTarget, rightHandRestTarget, attackWithSwordTween += 0.01);
					rightHandJointYRotation = tween(attackWithSwordHorSwingTillJointAngle, rightHandJointRestYRotation, attackWithSwordTween);

					if (attackWithSwordTween >= 1)
					{
						attackWithSwordTween = 0;
						rightHandCurrentTarget = rightHandRestTarget;
						rightHandJointYRotation = rightHandJointRestYRotation;
						currentAttackSwordAnimState = SWORD_ATK_IDLE;
						animating = ANIMATING_NONE;
					}
				}
			}
		}
		else if (attackWithSword && swordState != SWORD_EQUIPPED) // prob sword not equipped, free up the animating space to let others animate
		{
			attackWithSword = false;
			animating = ANIMATING_NONE;
		}

		if (attackWithHammer && hammerState == HAMMER_EQUIPPED && animating == ATTACK_WITH_HAMMER)
		{
			if (attackHammerState == HAMMER_ATK_IDLE)
			{
				// move hand to pre swing target
				leftHandCurrentTarget = tween(leftHandRestTarget, attackWithHammerStartSwingTarget, attackWithHammerTween += 0.05);
				if (attackWithHammerTween >= 1)
				{
					attackWithHammerTween = 0;
					leftHandCurrentTarget = attackWithHammerStartSwingTarget;
					attackHammerState = HAMMER_ATK_START_SWING;
				}
			}

			if (attackHammerState == HAMMER_ATK_START_SWING)
			{
				// move hand to swing till target
				leftHandCurrentTarget = tween(attackWithHammerStartSwingTarget, attackWithHammerSwingTillTarget, attackWithHammerTween += 0.09);
				if (attackWithHammerTween >= 1)
				{
					attackWithHammerTween = 0;
					leftHandCurrentTarget = attackWithHammerSwingTillTarget;
					attackHammerState = HAMMER_ATK_FINISH_SWING;
				}
			}

			if (attackHammerState == HAMMER_ATK_FINISH_SWING)
			{
				// move hand to overshoot
				leftHandCurrentTarget = tween(attackWithHammerSwingTillTarget, attackWithHammerOvershootTarget, attackWithHammerTween += 0.09);
				if (attackWithHammerTween >= 1)
				{
					attackWithHammerTween = 0;
					leftHandCurrentTarget = attackWithHammerOvershootTarget;
					attackHammerState = HAMMER_ATK_SWING_OVERSHOOT;
				}
			}

			if (attackHammerState == HAMMER_ATK_SWING_OVERSHOOT)
			{
				leftHandCurrentTarget = tween(attackWithHammerOvershootTarget, leftHandRestTarget, attackWithHammerTween += 0.05);
				if (attackWithHammerTween >= 1)
				{
					attackWithHammerTween = 0;
					leftHandCurrentTarget = leftHandRestTarget;
					attackHammerState = HAMMER_ATK_IDLE;
					animating = ANIMATING_NONE;
					attackWithHammer = false;
				}
			}
		}
		else if (attackWithHammer && hammerState != HAMMER_EQUIPPED)
		{
			attackWithHammer = false;
			animating = ANIMATING_NONE;
		}

		// kamekameha, only sword not equipped can activate
		if (kamekamehaCharging && swordState != SWORD_EQUIPPED)
		{
			if (kkhChargeState == KKH_NONE)
			{
				// move hands to target position
				rightkkhLastChargeTarget = rightHandCurrentTarget = tween(rightHandRestTarget, rightkkhChargeTarget, kkhTween += 0.05);
				leftkkhLastChargeTarget = leftHandCurrentTarget = tween(leftHandRestTarget, leftkkhChargeTarget, kkhTween);
				rightkkhPrevRootRotation = rightHandRootYRotation = tween(rightHandRootRestYRotation, -kkhOriRotateRoot, kkhTween);
				leftkkhPrevRootRotation = leftHandRootYRotation = tween(leftHandRootRestYRotation, kkhOriRotateRoot, kkhTween);
				if (kkhTween >= 1)
				{
					kkhTween = 0;
					rightHandCurrentTarget = rightkkhChargeTarget;
					leftHandCurrentTarget = leftkkhChargeTarget;
					rightkkhPrevRootRotation = rightHandRootYRotation = -kkhOriRotateRoot;
					leftkkhPrevRootRotation = leftHandRootYRotation = kkhOriRotateRoot;
					kkhChargeState = KKH_LOW;
				}
			}

			// charge from ori to low
			if (kkhChargeState == KKH_LOW)
			{
				kkhPrevColor = kkhBolaCurrentColor = tween(kkhOriBola, kkhLowBola, kkhTween += 0.005);
				kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ = 
					kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = tween(0.2, 0.7, kkhTween); // noneed additional open var gua
				rightkkhPrevRootRotation = rightHandRootYRotation = tween(-kkhOriRotateRoot, -kkhLowRotateRoot, kkhTween);
				leftkkhPrevRootRotation = leftHandRootYRotation = tween(kkhOriRotateRoot, kkhLowRotateRoot, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhPrevColor = kkhBolaCurrentColor = kkhLowBola;
					kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ = 
						kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = 0.7; // noneed additional open var gua
					rightkkhPrevRootRotation = rightHandRootYRotation = -kkhLowRotateRoot;
					leftkkhPrevRootRotation = leftHandRootYRotation = kkhLowRotateRoot;
					kkhChargeState = KKH_MEDIUM;
				}
			}

			// charge from low to med
			if (kkhChargeState == KKH_MEDIUM)
			{
				kkhPrevColor = kkhBolaCurrentColor = tween(kkhLowBola, kkhMedBola, kkhTween += 0.005);
				kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ =
					kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = tween(0.7, 1.0, kkhTween); // noneed additional open var gua
				rightkkhPrevRootRotation = rightHandRootYRotation = tween(-kkhLowRotateRoot, -kkhMedRotateRoot, kkhTween);
				leftkkhPrevRootRotation = leftHandRootYRotation = tween(kkhLowRotateRoot, kkhMedRotateRoot, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhPrevColor = kkhBolaCurrentColor = kkhMedBola;
					kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ =
						kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = 1.0; // noneed additional open var gua
					rightkkhPrevRootRotation = rightHandRootYRotation = -kkhMedRotateRoot;
					leftkkhPrevRootRotation = leftHandRootYRotation = kkhMedRotateRoot;
					kkhChargeState = KKH_HIGH;
				}
			}

			// charge ffrom med to high
			if (kkhChargeState == KKH_HIGH)
			{
				kkhPrevColor = kkhBolaCurrentColor = tween(kkhMedBola, kkhHighBola, kkhTween += 0.005);
				kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ =
					kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = tween(1.0, 1.2, kkhTween); // noneed additional open var gua
				rightkkhPrevRootRotation = rightHandRootYRotation = tween(-kkhMedRotateRoot, -kkhHighRotateRoot, kkhTween);
				leftkkhPrevRootRotation = leftHandRootYRotation = tween(kkhMedRotateRoot, kkhHighRotateRoot, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhPrevColor = kkhBolaCurrentColor = kkhHighBola;
					kkhBolaPrevScale.scaleX = kkhBolaPrevScale.scaleY = kkhBolaPrevScale.scaleZ =
						kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = 1.2; // noneed additional open var gua
					rightkkhPrevRootRotation = rightHandRootYRotation = -kkhHighRotateRoot;
					leftkkhPrevRootRotation = leftHandRootYRotation = kkhHighRotateRoot;
					kkhChargeState = KKH_SHOOT;
				}
			}

			if (kkhChargeState == KKH_SHOOT)
			{
				kkhPrevBeamLength = kkhBeamLength = tween(0, 15, kkhTween += 0.005);
				kkhPrevTopRad = kkhTopRad = tween(0, 4, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhPrevBeamLength = kkhBeamLength = 15;
					kkhPrevTopRad = kkhTopRad = 4;
					kkhChargeState = KKH_SHOOTING; // need one additional state so this condition won't repeat
				}
			}
		}
		else if (kamekamehaCharging && swordState == SWORD_EQUIPPED)
		{
			kamekamehaCharging = false;
			animating = ANIMATING_NONE;
		}

		// stop charging kamekameha
		if (!kamekamehaCharging && animating == CHARGE_KAMEKAMEHA)
		{
			if (kkhChargeState == KKH_SHOOTING)
			{
				// go to other state from the additional state
				kkhChargeState = KKH_SHOOT;
			}

			if (kkhChargeState == KKH_SHOOT)
			{
				kkhBeamLength = tween(kkhPrevBeamLength, 0, kkhTween += 0.005);
				kkhTopRad = tween(kkhPrevTopRad, 0, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhBeamLength = 0;
					kkhTopRad = 0;
					kkhChargeState = KKH_HIGH; // need one additional state so this condition won't repeat
				}
			}

			if (kkhChargeState != KKH_NONE && kkhChargeState != KKH_SHOOT && kkhChargeState != KKH_SHOOTING)
			{
				// xian make it no bola
				kkhBolaCurrentColor = tween(kkhPrevColor, kkhOriBola, kkhTween += 0.005);
				if (kkhBolaPrevScale.scaleX > 0.2)
					kkhBolaScale.scaleX = kkhBolaScale.scaleY = kkhBolaScale.scaleZ = tween(kkhBolaPrevScale.scaleX, 0.2, kkhTween); // luckily all same scale
				// rotate in first, when wan rest ka rotate back out
				rightHandRootYRotation = tween(rightkkhPrevRootRotation, -kkhOriRotateRoot, kkhTween);
				leftHandRootYRotation = tween(leftkkhPrevRootRotation, kkhOriRotateRoot, kkhTween);

				if (kkhTween >= 1)
				{
					kkhTween = 0;
					kkhChargeState = KKH_NONE;
					kkhBolaCurrentColor = kkhOriBola;
					rightHandRootYRotation = rightHandRootRestYRotation;
					leftHandRootYRotation = leftHandRootRestYRotation;
					kkhBolaScale = Transform(); // auto set back all scale = 0 ki
				}
			}
			if (kkhChargeState == KKH_NONE)
			{
				// move hands to target position
				rightHandCurrentTarget = tween(rightkkhLastChargeTarget, rightHandRestTarget, kkhTween += 0.05);
				leftHandCurrentTarget = tween(leftkkhLastChargeTarget, leftHandRestTarget, kkhTween);
				rightHandRootYRotation = tween(rightkkhPrevRootRotation, rightHandRootRestYRotation, kkhTween);
				leftHandRootYRotation = tween(leftkkhPrevRootRotation, leftHandRootRestYRotation, kkhTween);
				if (kkhTween >= 1)
				{
					kkhTween = 0;
					rightHandCurrentTarget = rightHandRestTarget;
					leftHandCurrentTarget = leftHandRestTarget;
					rightHandRootYRotation = rightHandRootRestYRotation;
					leftHandRootYRotation = leftHandRootRestYRotation;
					animating = ANIMATING_NONE;
				}
			}
		}

		// kkh bolas
		if (kkhChargeState != KKH_NONE && animating == CHARGE_KAMEKAMEHA)
		{
			cv
				.pushMatrix()
				.translate(0, 0, 5)
				.scale(kkhBolaScale.scaleX, kkhBolaScale.scaleY, kkhBolaScale.scaleZ)
				.sphere({ 0, 0, 0, kkhBolaCurrentColor }, 1)
				.popMatrix()
				;
		}

		// beam to shoot when kkh charge complete
		if (kkhChargeState == KKH_SHOOT || kkhChargeState == KKH_SHOOTING)
		{
			// beam same color as bola
			cv
				.pushMatrix()
				.translate(0, 0, 4)
				.cylinder({0, 0, 0}, 1, kkhTopRad, kkhBeamLength)
				.popMatrix()
				;
		}

		if (gunShoot && gunState == GUN_EQUIPPED)
		{
			// even though both gun can technically be ready at the same time, only one gun should shoot
			if (leftGunShootingState == GUN_READY)
			{
				leftGunPrevTarget = leftHandCurrentTarget = tween(leftHandGunEquipUnequipTarget, leftGunRecoilTarget, leftGunShootTween += 0.05);

				currentGun = LEFT_GUN;

				if (leftGunShootTween >= 1)
				{
					leftGunShootTween = 0;
					leftGunPrevTarget = leftHandCurrentTarget = leftGunRecoilTarget;
					leftGunShootingState = GUN_RECOIL;
				}
			}
			
			if (rightGunShootingState == GUN_READY && leftGunShootingState != GUN_READY) 
			{
				rightGunPrevTarget = rightHandCurrentTarget = tween(rightHandGunEquipUnequipTarget, rightGunRecoilTarget, rightGunShootTween += 0.05);

				currentGun = RIGHT_GUN;

				if (rightGunShootTween >= 1)
				{
					rightGunShootTween = 0;
					rightGunPrevTarget = rightHandCurrentTarget = rightGunRecoilTarget;
					rightGunShootingState = GUN_RECOIL;
				}
			}

			if (leftGunShootingState == GUN_RECOIL)
			{
				leftGunPrevTarget = leftHandCurrentTarget = tween(leftGunRecoilTarget, leftHandGunEquipUnequipTarget, leftGunRecoilTween += 0.05);

				if (leftGunRecoilTween >= 1)
				{
					// reset gun shoot tween
					leftGunRecoilTween = 0;
					leftGunPrevTarget = leftHandCurrentTarget = leftHandGunEquipUnequipTarget;
					leftGunShootingState = GUN_READY;
				}
			} 
			if (rightGunShootingState == GUN_RECOIL)
			{
				rightGunPrevTarget = rightHandCurrentTarget = tween(rightGunRecoilTarget, rightHandGunEquipUnequipTarget, rightGunRecoilTween += 0.05);

				if (rightGunRecoilTween >= 1)
				{
					// reset gun shoot tween
					rightGunRecoilTween = 0;
					rightGunPrevTarget = rightHandCurrentTarget = rightHandGunEquipUnequipTarget;
					rightGunShootingState = GUN_READY;
				}
			}

		}
		else if (gunShoot && gunState != GUN_EQUIPPED)
		{
			animating = ANIMATING_NONE;
			gunShoot = false;
		}

		if (!gunShoot && animating == ATTACK_WITH_GUN)
		{
			// tween back the hands to normal equipped state and set animating to none to free up animation slot
			leftHandCurrentTarget = tween(leftGunPrevTarget, leftHandGunEquipUnequipTarget, stopGunShootTween += 0.05);
			rightHandCurrentTarget = tween(rightGunPrevTarget, rightHandGunEquipUnequipTarget, stopGunShootTween);
			if (stopGunShootTween >= 1)
			{
				leftHandCurrentTarget = leftHandGunEquipUnequipTarget;
				rightHandCurrentTarget = rightHandGunEquipUnequipTarget;
				stopGunShootTween = 0;
				animating = ANIMATING_NONE;
			}
		}

		// arm joint to body
		// right
		cv
			.pushMatrix()
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, lineOrJoint }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		// left, lazy rethink coords d
		cv
			.pushMatrix()
			.rotate(180, 0, 1, 0)
			.translate(2, 7, 0)
			.rotate(20, 0, 0, 1)
			.cuboid({ -0.5, 0.5, 0.5, lineOrJoint }, { 3.5, -0.5, -0.5 })
			.popMatrix()
			;

		// duli eh float above hand, flw body orientation
		if (!hideUnequippedWeapon)
		{
			// right shield
			cv.pushMatrix();
			cv.rotate(shieldCurrentState.rotAngle, 0, 0, 1);
			cv.rotate(shieldCurrentRotateY, 0, 1, 0);
			cv.translate(shieldCurrentState.transX, shieldCurrentState.transY, shieldCurrentState.transZ);
			cv.scale(shieldCurrentState.scaleX, shieldCurrentState.scaleY, shieldCurrentState.scaleZ);

			WeaponProjectionBackground::shield(shieldActivated, false);

			cv.popMatrix();

			// left shield
			cv.pushMatrix();
			cv.rotate(shieldCurrentState.rotAngle, 0, 0, -1);
			cv.rotate(shieldCurrentRotateY, 0, -1, 0);
			cv.translate(-shieldCurrentState.transX, shieldCurrentState.transY, shieldCurrentState.transZ);
			cv.scale(shieldCurrentState.scaleX, shieldCurrentState.scaleY, shieldCurrentState.scaleZ);

			WeaponProjectionBackground::shield(shieldActivated, true);

			cv.popMatrix();
		}

		// solve the transformations and draw
		if (inEditMode) // rn edit mode only affects inverse kinematic targets
		{
			rightHand.solveIK(rightHandTargetDebug);
			cv.pointSize(20).point({ rightHandTargetDebug.x + 2, rightHandTargetDebug.y, rightHandTargetDebug.z, rightHandTargetDebug.c }); // move the point out abit for easier visualisation
			rightHand.forceYRotation(rightHandRootYDebug, rightHandJointYDebug, rightHandPalmYDebug, rightHandPalmZDebug);
		}
		else 
		{
			rightHand.solveIK(rightHandCurrentTarget);
			rightHand.forceYRotation(rightHandRootYRotation, rightHandJointYRotation, rightHandPalmYRotation, rightHandPalmZRotation);
		}
		//rightHand.forceYRotation(debugRootY, debugJointY);
		rightHand.grip(rightHandShouldGrip, rightHandGripType);
		rightHand.draw();

		if (inEditMode) // rn edit mode only affects inverse kinematic targets
		{
			leftHand.solveIK(leftHandTargetDebug);
			cv.pointSize(20).point({ leftHandTargetDebug.x - 2, leftHandTargetDebug.y, leftHandTargetDebug.z, leftHandTargetDebug.c }); // move the point out abit for easier visualisation
			leftHand.forceYRotation(leftHandRootYDebug, leftHandJointYDebug, leftHandPalmYDebug, leftHandPalmZDebug);
		}
		else 
		{
			leftHand.solveIK(leftHandCurrentTarget);
			leftHand.forceYRotation(leftHandRootYRotation, leftHandJointYRotation, leftHandPalmYRotation, leftHandPalmZRotation);
		}
		leftHand.grip(leftHandShouldGrip, leftHandGripType);
		leftHand.draw();

		cv.popMatrix();

		cv
			.cube({ 0, 1.5, 0, lineOrJoint }, 0.5) // upper body join lower body eh part
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			;
		// lower body
		cv
			.cuboid(
				{ -0.5, 1, 0.5, primary },
				{ 0.5, 1, -0.5 },
				{ -1.5, 0, 1.5 },
				{ 1.5, 0, -1.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid(
				{ -1.5, 0, 1.5, primary },
				{ 1.5, 0, -1.5 },
				{ -1, -1, 1 },
				{ 1, -1, -1 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid({ -1, -1, 1, primary }, { 1, -1, -1 }, { -0.5, -2, 0.5 }, { 0.5, -2, -0.5 })
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cuboid(
				{ -0.5, -2, 0.5, primary },
				{ 0.5, -3, -0.5 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			.cylinder({ 0, -2.5, -0.6, lineOrJoint }, 0.375, 0.375, 1.2, true)
			.cylinder({ 0, -2.5, -0.625, secondary }, 0.25, 0.25, 1.25, true)
			.cuboid(
				{ -0.5, -3, 0.5, lineOrJoint },
				{ 0.5, -3, -0.5 },
				{ -0.5, -4, 0.7 },
				{ 0.5, -4, -0.3 }
			)
			.replotPrevBlocky3D(GL_LINE_LOOP, outline)
			;

		Leg leftLeg({ 2, -2, 0 });
		
		// dk when my fking brain was coding the targets, left target affects right leg right target affects left leg
		// yes, my nice brain coded the inverse stuff, but just deal with it mkay?
		if (inEditMode) // rn edit mode only affects inverse kinematic targets
		{
			leftLeg.solveIK(rightLegTargetDebug);
			// dk why is - but it works, it works
			cv.pointSize(20).point({ rightLegTargetDebug.x + 2, rightLegTargetDebug.y, rightLegTargetDebug.z, rightLegTargetDebug.c }); // move the point out abit for easier visualisation
		}
		else 
		{
			leftLeg.solveIK(rightLegCurrentTarget);
		}
		leftLeg.draw();
		Leg rightLeg({ -2, -2, 0 });
		if (inEditMode) // rn edit mode only affects inverse kinematic targets
		{
			rightLeg.solveIK(leftLegTargetDebug);
			cv.pointSize(20).point({ leftLegTargetDebug.x - 2, leftLegTargetDebug.y, leftLegTargetDebug.z, leftLegTargetDebug.c }); // move the point out abit for easier visualisation
		}
		else
		{
			rightLeg.solveIK(leftLegCurrentTarget);
		}
		rightLeg.draw();

		// gun bullets, not concerned with other transforms
		if (currentGun == LEFT_GUN && gunShoot)
		{
			// when left gun shooting, sneakily reset right gun bullet's transformation
			rightGunBullet.transZ = 0;
			cv
				.pushMatrix()
				.translate(0, 0, leftGunBullet.transZ += 0.55)
				.sphere({ -5, 3.5, 9, {255, 255, 255} }, 0.5)
				;

			cv.popMatrix();
		}
		if (currentGun == RIGHT_GUN && gunShoot)
		{
			// when left gun shooting, sneakily reset right gun bullet's transformation
			leftGunBullet.transZ = 0;
			cv
				.pushMatrix()
				.translate(0, 0, rightGunBullet.transZ += 0.55)
				.sphere({ 5, 3.5, 9, {255, 255, 255} }, 0.5)
				;

			cv.popMatrix();
		}
	}

	void handleKeyDownEvent(WPARAM key)
	{
		// only no animation ongoing ka can enter debug mode
		if (key == VK_TAB && animating == ANIMATING_NONE) // editor power ACTIVATE
		{
			inEditMode = !inEditMode;
			return;
		}

		// FUTURE ENHANCE maybe can extend on this edit capabilities
		// only no animation ongoing ka can adjust le targets
		if (inEditMode && animating == ANIMATING_NONE)
		{
			// all the keys are different set d
			// F2 -> edit leftHandTarget, F3 -> edit rightHandTarget, F4 -> edit leftLegTarget, F5 -> edit rightLegTarget
			switch (key)
			{
			case VK_F2:
				editModeTarget = EDIT_LEFT_HAND;
				break;
			case VK_F3:
				editModeTarget = EDIT_RIGHT_HAND;
				break;
			case VK_F4:
				editModeTarget = EDIT_LEFT_LEG;
				break;
			case VK_F5:
				editModeTarget = EDIT_RIGHT_LEG;
				break;
			}

			// taking pointer cuz wanna edit the values, not taking reference cuz we dk which one yet
			Point3D* editingIKTarget;
			Point3D* editingIKTargetPrev;
			bool isLeft = false; // if isLeft we need to flip the z movement
			switch (editModeTarget)
			{
			case Robot::EDIT_LEFT_HAND:
				editingIKTarget = &leftHandTargetDebug;
				editingIKTargetPrev = &leftHandPrevTargetDebug;
				isLeft = true;
				break;
			case Robot::EDIT_RIGHT_HAND:
				editingIKTarget = &rightHandTargetDebug;
				editingIKTargetPrev = &rightHandPrevTargetDebug;
				break;
			case Robot::EDIT_LEFT_LEG:
				editingIKTarget = &leftLegTargetDebug;
				editingIKTargetPrev = &leftLegPrevTargetDebug;
				isLeft = true;
				break;
			case Robot::EDIT_RIGHT_LEG:
				editingIKTarget = &rightLegTargetDebug;
				editingIKTargetPrev = &rightLegPrevTargetDebug;
				break;
			default:
				return; // impossible reach here
			}

			switch (key)
			{
			case 'W':
				editingIKTargetPrev->y = editingIKTarget->y += 0.5;
				break;
			case 'S':
				editingIKTargetPrev->y = editingIKTarget->y -= 0.5;
				break;
			case 'A':
				editingIKTargetPrev->z = editingIKTarget -> z += isLeft ? -0.5 : 0.5;
				break;
			case 'D':
				editingIKTargetPrev-> z = editingIKTarget->z -= isLeft ? -0.5 : 0.5;
				break;
			}

			// for leg targets, cannot go more than y == -2, so y must be < -2
			if (leftLegTargetDebug.y >= -2)
			{
				leftLegPrevTargetDebug.y = leftLegTargetDebug.y = -2.5; // max -2.5 la
			}

			if (rightLegTargetDebug.y >= -2)
			{
				rightLegPrevTargetDebug.y = rightLegTargetDebug.y = -2.5;
			}

			// only for hand, rotate the rootY or jointY or the palm
			if (key == 'L') // joint up
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevJointYDebug = rightHandJointYDebug += 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevJointYDebug = leftHandJointYDebug += 1;
				}
			}

			if (key == 'K') // joint down
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevJointYDebug = rightHandJointYDebug -= 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevJointYDebug = leftHandJointYDebug -= 1;
				}
			}

			if (key == 'J') // root up
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevRootYDebug = rightHandRootYDebug += 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevRootYDebug = leftHandRootYDebug += 1;
				}
			}

			if (key == 'H') // root down
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevRootYDebug = rightHandRootYDebug -= 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevRootYDebug = leftHandRootYDebug -= 1;
				}
			}

			if (key == 'M') // palm Y up
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevPalmYDebug = rightHandPalmYDebug += 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevPalmYDebug = leftHandPalmYDebug += 1;
				}
			}

			if (key == 'N') // palm Y down
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevPalmYDebug = rightHandPalmYDebug -= 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevPalmYDebug = leftHandPalmYDebug -= 1;
				}
			}

			if (key == 'B') // palm Z up
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevPalmZDebug = rightHandPalmZDebug += 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevPalmZDebug = leftHandPalmZDebug += 1;
				}
			}

			if (key == 'V') // palm Z down
			{
				if (editModeTarget == EDIT_RIGHT_HAND)
				{
					rightHandPrevPalmZDebug = rightHandPalmZDebug -= 1;
				}
				else if (editModeTarget == EDIT_LEFT_HAND)
				{
					leftHandPrevPalmZDebug = leftHandPalmZDebug -= 1;
				}
			}

			// space reset all
			if (key == VK_SPACE)
			{

				rightHandPrevTargetDebug = rightHandTargetDebug = { rightHandRestTarget.x, rightHandRestTarget.y, rightHandRestTarget.z, { 255, 255, 255} };
				leftHandPrevTargetDebug = leftHandTargetDebug = { leftHandRestTarget.x, leftHandRestTarget.y, leftHandRestTarget.z, { 255, 255, 255} };
				rightLegPrevTargetDebug = rightLegTargetDebug = { rightLegRestTarget.x, rightLegRestTarget.y, rightLegRestTarget.z, { 255, 255, 255} };
				leftLegPrevTargetDebug = leftLegTargetDebug = { leftLegRestTarget.x, leftLegRestTarget.y, leftLegRestTarget.z, { 255, 255, 255} };
				rightHandPrevJointYDebug = rightHandPrevRootYDebug = rightHandPrevPalmYDebug = rightHandPrevPalmZDebug = 
					rightHandJointYDebug = rightHandRootYDebug = rightHandPalmYDebug = rightHandPalmZDebug = 0;
				leftHandPrevJointYDebug = leftHandPrevRootYDebug = leftHandPrevPalmYDebug = leftHandPrevPalmZDebug = 
					leftHandJointYDebug = leftHandRootYDebug = leftHandPalmYDebug = leftHandPalmZDebug = 0;
			}

			// recording and playback (rn only support record one point)
			switch (key)
			{
			case 'R':
				rightHandTargetDebugRecord = rightHandTargetDebug;
				leftHandTargetDebugRecord = leftHandTargetDebug;
				rightLegTargetDebugRecord = rightLegTargetDebug;
				leftLegTargetDebugRecord = leftLegTargetDebug;

				rightHandJointYDebugRecord = rightHandJointYDebug;
				rightHandRootYDebugRecord = rightHandRootYDebug;
				rightHandPalmYDebugRecord = rightHandPalmYDebug;
				rightHandPalmZDebugRecord = rightHandPalmZDebug;

				leftHandJointYDebugRecord = leftHandJointYDebug;
				leftHandRootYDebugRecord = leftHandRootYDebug;
				leftHandPalmYDebugRecord = leftHandPalmYDebug;
				leftHandPalmZDebugRecord = leftHandPalmZDebug;
				break;
			case VK_OEM_PERIOD: // play back (tween from prev target to recorded target) the full stop key
				debugPlayBack = true;
				animating = DEBUG_PLAYBACK;
				break;
			}

			return;
		}

		switch (key)
		{
		case 'W': 
			if (animating != ANIMATING_NONE && animating != WALK)
				break;
			animating = WALK;
			isWalking = true;
			break;
		case '1': // defense
			// only not hiding unequipped weapon baru can activate defense
			if ((animating != ANIMATING_NONE && animating != SHIELD) || hideUnequippedWeapon || swordState == SWORD_EQUIPPED) // shudnt hold and shudnt be able to be toggled when sword is equipped
				break;
			animating = SHIELD;
			setShieldActive = !setShieldActive;
			break;
		case '2':
			// only can equip / unequip sword when the sword is shown
			if (animating != ANIMATING_NONE || hideUnequippedWeapon) // only finish equip/unequipn ka can execute again
				break;
			animating = SWORD_EQUIP_UNEQUIP;
			setSwordEquip = !setSwordEquip;
			break;
		case '3': // attack with sword
			if (animating != ANIMATING_NONE) // only ntg animating only can attack with sword
				break;
			attackWithSword = true;
			animating = ATTACK_WITH_SWORD;
			attackWithSwordType = attackWithSwordType == SWORD_ATK_HOR ? SWORD_ATK_VER : SWORD_ATK_HOR;
			break;
		case '4':
			if (animating != ANIMATING_NONE)
				break;
			animating = HAMMER_EQUIP_UNEQUIP;
			setHammerEquip = !setHammerEquip;
			break;
		case '5':
			if (animating != ANIMATING_NONE)
				break;
			animating = ATTACK_WITH_HAMMER;
			attackWithHammer = true;
			break;
		case '6':
			if (animating != ANIMATING_NONE)
				break;
			animating = GUN_EQUIP_UNEQUIP;
			setGunEquip = !setGunEquip;
			break;
		case '7':
			if (animating != ANIMATING_NONE && animating != ATTACK_WITH_GUN)
				break;
			animating = ATTACK_WITH_GUN;
			gunShoot = !gunShoot;
			break;
		case 'C':
			if (animating != ANIMATING_NONE) // only ntg animating only can attack with sword
				break;
			kamekamehaCharging = true;
			animating = CHARGE_KAMEKAMEHA;
			break;
		case 'H':
			hideUnequippedWeapon = !hideUnequippedWeapon;
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
		case 'C':
			kamekamehaCharging = false;
			// here reset the tween better
			kkhTween = 0;
			break;
		}
	}
}
