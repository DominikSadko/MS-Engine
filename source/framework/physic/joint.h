#ifndef __JOINT_H__
#define __JOINT_H__

#include "framework/core/platform.h"

#include "framework/lua/lua.h"
#include <Box2D/Box2D.h>

class RevoluteJoint : public b2RevoluteJoint { };
class DistanceJoint : public b2DistanceJoint { };
class PrismaticJoint : public b2PrismaticJoint { };
class FrictionJoint : public b2FrictionJoint { };
class GearJoint : public b2GearJoint { };
class MotorJoint : public b2MotorJoint { };
class MouseJoint : public b2MouseJoint { };
class PulleyJoint : public b2PulleyJoint { };
class RopeJoint : public b2RopeJoint { };
class WeldJoint : public b2WeldJoint { };
class WheelJoint : public b2WheelJoint { };

struct Joint
{
	static RevoluteJoint*  Revolute(Body* bodyA, Body* bodyB, bool collide, const Point& pointA, const Point& pointB);
	static DistanceJoint*  Distance(Body* bodyA, Body* bodyB, const Point& pointA, const Point& pointB);
	static PrismaticJoint* Prismatic(Body* bodyA, Body* bodyB, const Point& pointA, const Point& pointB, const Point& axis, float referenceAngle);
	static FrictionJoint*  Friction(Body* bodyA, Body* bodyB);
	static GearJoint*      Gear(Body* bodyA, Body* bodyB, b2Joint* jointA, b2Joint* jointB);
	static MotorJoint*     Motor(Body* bodyA, Body* bodyB);
	static MouseJoint*     Mouse(Body* bodyA, Body* bodyB);
	static PulleyJoint*    Pulley(Body* bodyA, Body* bodyB);
	static RopeJoint*      Rope(Body* bodyA, Body* bodyB);
	static WeldJoint*      Weld(Body* bodyA, Body* bodyB);
	static WheelJoint*     Wheel(Body* bodyA, Body* bodyB);
};


#endif // __JOINT_H__ //
