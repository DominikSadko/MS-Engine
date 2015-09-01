#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"
#include "framework/physic/fixture.h"
#include "framework/physic/joint.h"

// todo destructor

namespace Lua
{
float Parser<float>::get(lua_State* L, int32_t idx)   { return (float)lua_tonumber(L, idx) / 100.f; }
void  Parser<float>::put(lua_State* L, const float v) { lua_pushnumber(L, std::floor((double)v * 100.f)); }
bool  Parser<float>::is(lua_State* L, int32_t idx)    { return lua_isnumber(L, idx); }

b2Vec2 Parser<b2Vec2>::get(lua_State* L, int32_t idx)
{
	b2Vec2 vec;
    Lua::Object object(L, idx);

    vec.x = object["x"].get<int32_t>() / BOX2D_SCALE;
    vec.y = object["y"].get<int32_t>() / BOX2D_SCALE;
    return vec;
}

void Parser<b2Vec2>::put(lua_State* L, const b2Vec2& vec)
{
    std::map<std::string, int32_t> pointMap;
    pointMap["x"] = std::floor(vec.x * BOX2D_SCALE);
    pointMap["y"] = std::floor(vec.y * BOX2D_SCALE);

    Lua::Parser<std::map<std::string, int32_t>>::put(L, pointMap);
}

bool Parser<b2Vec2>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }


const b2Vec2 Parser<const b2Vec2&>::get(lua_State* L, int32_t idx)
{
	b2Vec2 vec;
    Lua::Object object(L, idx);

    vec.x = object["x"].get<int32_t>() / BOX2D_SCALE;
    vec.y = object["y"].get<int32_t>() / BOX2D_SCALE;
    return vec;
}

void Parser<const b2Vec2&>::put(lua_State* L, const b2Vec2& vec)
{
    std::map<std::string, int32_t> pointMap;
    pointMap["x"] = std::floor(vec.x * BOX2D_SCALE);
    pointMap["y"] = std::floor(vec.y * BOX2D_SCALE);

    Lua::Parser<std::map<std::string, int32_t>>::put(L, pointMap);
}

bool Parser<const b2Vec2&>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }
}

RevoluteJoint* Joint::Revolute(Body* bodyA, Body* bodyB, bool collide, const Point& pointA, const Point& pointB)
{
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = collide;

	jointDef.localAnchorA.Set(pointA.x / BOX2D_SCALE, pointA.y / BOX2D_SCALE);
	jointDef.localAnchorB.Set(pointB.x / BOX2D_SCALE, pointB.y / BOX2D_SCALE);
	jointDef.referenceAngle = 0.f;
	jointDef.lowerAngle = 0.f;
	jointDef.upperAngle = 0.f;
	jointDef.maxMotorTorque = 0.f;
	jointDef.motorSpeed = 0.f;
	jointDef.enableLimit = false;
	jointDef.enableMotor = false;

    return (RevoluteJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

DistanceJoint* Joint::Distance(Body* bodyA, Body* bodyB, const Point& pointA, const Point& pointB)
{
	b2DistanceJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(pointA.x / BOX2D_SCALE, pointA.y / BOX2D_SCALE);
	jointDef.localAnchorB.Set(pointB.x / BOX2D_SCALE, pointB.y / BOX2D_SCALE);
	jointDef.length = 1.f;
	jointDef.frequencyHz = 0.f;
	jointDef.dampingRatio = 0.f;

    return (DistanceJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

PrismaticJoint* Joint::Prismatic(Body* bodyA, Body* bodyB, const Point& pointA, const Point& pointB, const Point& axis, float referenceAngle)
{
	b2PrismaticJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(pointA.x / BOX2D_SCALE, pointA.y / BOX2D_SCALE);
	jointDef.localAnchorB.Set(pointB.x / BOX2D_SCALE, pointB.y / BOX2D_SCALE);
	jointDef.localAxisA.Set(axis.x / BOX2D_SCALE, axis.y / BOX2D_SCALE);
	jointDef.referenceAngle = referenceAngle;
	jointDef.enableLimit = false;
	jointDef.lowerTranslation = 0.0f;
	jointDef.upperTranslation = 0.0f;
	jointDef.enableMotor = false;
	jointDef.maxMotorForce = 0.0f;
	jointDef.motorSpeed = 0.0f;

    return (PrismaticJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

FrictionJoint* Joint::Friction(Body* bodyA, Body* bodyB)
{
	b2FrictionJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.maxForce = 0.0f;
	jointDef.maxTorque = 0.0f;

    return (FrictionJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

GearJoint* Joint::Gear(Body* bodyA, Body* bodyB, b2Joint* jointA, b2Joint* jointB)
{
	b2GearJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.joint1 = jointA;
	jointDef.joint2 = jointB;
	jointDef.ratio = 1.0f;

    return (GearJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

MotorJoint* Joint::Motor(Body* bodyA, Body* bodyB)
{
	b2MotorJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.linearOffset.SetZero();
	jointDef.angularOffset = 0.0f;
	jointDef.maxForce = 1.0f;
	jointDef.maxTorque = 1.0f;
	jointDef.correctionFactor = 0.3f;

    return (MotorJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

MouseJoint* Joint::Mouse(Body* bodyA, Body* bodyB)
{
	b2MouseJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.target.Set(0.0f, 0.0f);
	jointDef.maxForce = 0.0f;
	jointDef.frequencyHz = 5.0f;
	jointDef.dampingRatio = 0.7f;

    return (MouseJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

PulleyJoint* Joint::Pulley(Body* bodyA, Body* bodyB)
{
	b2PulleyJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.groundAnchorA.Set(-1.0f, 1.0f);
	jointDef.groundAnchorB.Set(1.0f, 1.0f);
	jointDef.localAnchorA.Set(-1.0f, 0.0f);
	jointDef.localAnchorB.Set(1.0f, 0.0f);
	jointDef.lengthA = 0.0f;
	jointDef.lengthB = 0.0f;
	jointDef.ratio = 1.0f;
	jointDef.collideConnected = true;

    return (PulleyJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

RopeJoint* Joint::Rope(Body* bodyA, Body* bodyB)
{
	b2RopeJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(-1.0f, 0.0f);
	jointDef.localAnchorB.Set(1.0f, 0.0f);
	jointDef.maxLength = 0.0f;

    return (RopeJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

WeldJoint* Joint::Weld(Body* bodyA, Body* bodyB)
{
	b2WeldJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB.Set(0.0f, 0.0f);
	jointDef.referenceAngle = 0.0f;
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 0.0f;

    return (WeldJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}

WheelJoint* Joint::Wheel(Body* bodyA, Body* bodyB)
{
	b2WheelJointDef jointDef;
	jointDef.bodyA = bodyA->getB2Body();
	jointDef.bodyB = bodyB->getB2Body();
	jointDef.collideConnected = false;

	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB.Set(0.0f, 0.0f);
	jointDef.localAxisA.Set(1.0f, 0.0f);
	jointDef.enableMotor = false;
	jointDef.maxMotorTorque = 0.0f;
	jointDef.motorSpeed = 0.0f;
	jointDef.frequencyHz = 2.0f;
	jointDef.dampingRatio = 0.7f;

    return (WheelJoint*)bodyA->getB2World()->CreateJoint(&jointDef);
}
