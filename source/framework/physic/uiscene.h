#ifndef __UISCENE_H__
#define __UISCENE_H__

#include "framework/ui/uiwidget.h"
#include "framework/physic/camera.h"

#include <stdex/flag.h>
#include <Box2D/Box2D.h>

#define BOX2D_SCALE 128.f

class Body;
class Fixture;
class UIScene : public UIWidget, public b2DestructionListener, public b2ContactListener, public b2QueryCallback, public b2RayCastCallback
{
	public:
		UIScene();
		virtual ~UIScene();
		virtual void remove();

		void init();
		void terminate();
		virtual void draw(const Point& offset, float scale);

		void sleep() { m_sleep = true; }
		void awake() { m_sleep = false; }
		bool isSleeping() { return m_sleep; }

		void setStep(int32_t timeStep, int32_t velocityIterations, int32_t positionIterations);
		int32_t getTimeStep() const { return m_timeStep; };
		int32_t getVelocityIterations() const { return m_velocityIterations; };
		int32_t getPositionIterations() const { return m_positionIterations; };
		void setScale(uint16_t scale) { m_scale = scale; };
		uint16_t getScale() const { return m_scale; };

		b2World* getWorld() const { return m_world; }
		Body* getFirstBody() const;
		void removeBody(Body* body);
		void removeJoint(b2Joint* joint);
		int32_t getBodyCount() const;
		int32_t getJointCount() const;

		void setGravity(const Point& gravity);
		Point getGravity() const;

	private:
		b2World* m_world;

		bool m_sleep;
		int32_t m_timeStep;
	    int32_t m_velocityIterations;
	    int32_t m_positionIterations;
	    uint16_t m_scale;

	    std::vector<Body*> m_bodiesToRemove;

	public:
		std::vector<Fixture*> getFixtureList() const { return m_fixtures; };
		std::vector<Body*> getBodyList() const { return m_bodies; };
		void queryAABB(const Point& from, const Point& to);
		void rayCast(const Point& from, const Point& to);

	private:
	    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		bool ReportFixture(b2Fixture* fixture);
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		void SayGoodbye(b2Joint* joint);
		void SayGoodbye(b2Fixture* fixture);

	protected:
	    std::vector<Fixture*> m_fixtures;
	    std::vector<Body*> m_bodies;

	public:
	    void setCamera(Body* body) { m_camera.set(body); }
	    Body* getCamera() const { return m_camera.get(); }
	    void setCameraOffset(const Point& offset) { m_camera.setOffset(offset); }
	    Point getCameraOffset() const { return m_camera.getOffset(); }

	private:
	    CameraManager m_camera;
	    friend class CameraManager;
	    friend class UICondition;
};

#endif // __UISCENE_H__ //
