#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"
#include "framework/physic/fixture.h"
#include "framework/physic/joint.h"

/*
   Density - gêstoœæ
   Friction - tarcie
   Restitution - Odbicie
   Velocity - Prêdkoœæ
   Inertia - bezw³adnoœæ
   Damping - t³umienie

   torque = moment obrotowy
   fraction = u³amek
*/

UIScene::UIScene()
{
    setMetatable("UIScene");
    setVirtualArea(true);

    m_sleep = false;
    m_timeStep = 60;
    m_velocityIterations = 8;
    m_positionIterations = 8;
    m_scale = 100;

    m_world = nullptr;

    init();
}

UIScene::~UIScene()
{
	if(m_world)
		delete m_world;
}

void UIScene::remove()
{
	terminate();
	UIWidget::remove();
	delete this;
}

void UIScene::init()
{
	if(m_world)
	{
		LOGE("Already initialized.");
		return;
	}

    m_world = new b2World(b2Vec2(0.f, 0.f));
    m_world->SetDestructionListener(this);
    m_world->SetContactListener(this);
    m_world->SetAutoClearForces(false);
}

void UIScene::terminate()
{
	m_bodiesToRemove.clear();
	m_fixtures.clear();
	m_bodies.clear();
	m_camera.set(nullptr);
	m_camera.setOffset({0, 0});

	if(m_world)
	{
		delete m_world;
		m_world = nullptr;
	}
}

void UIScene::draw(const Point& offset, float scale)
{
	if(!isVisible() || !m_rect.w || !m_rect.h)
        return;

    if(isVirtualArea())
        g_painter.setDrawArea(getVirtualRect());

	if(m_world)
	{
		if(!m_bodiesToRemove.empty())
		{
			for(auto& body : m_bodiesToRemove)
				m_world->DestroyBody(body->getB2Body());

			m_bodiesToRemove.clear();
		}

		if(!isSleeping())
			m_world->Step(1.f / m_timeStep, m_velocityIterations, m_positionIterations);

		Point camera = m_camera.update(this);
		camera.x = offset.x + m_rect.x - (camera.x * m_scale / 100) + getWidth() / 2;
		camera.y = offset.y + m_rect.y - (camera.y * m_scale / 100) + getHeight() / 2;

		for(Body* body = getFirstBody(); body; body = body->getNext())
			body->draw(camera, m_scale / 100.f);
	}

	for(auto it = m_childs.rbegin(); it != m_childs.rend(); it++)
		(*it)->draw({offset.x + m_rect.x, offset.y + m_rect.y}, scale);

    if(isVirtualArea())
        g_painter.resetDrawArea();
}

void UIScene::setStep(int32_t timeStep, int32_t velocityIterations, int32_t positionIterations)
{
	m_timeStep = timeStep;
    m_velocityIterations = velocityIterations;
    m_positionIterations = positionIterations;
}

int32_t UIScene::getBodyCount() const
{
	return m_world->GetBodyCount();
}

int32_t UIScene::getJointCount() const
{
	return m_world->GetJointCount();
}

Body* UIScene::getFirstBody() const
{
	if(b2Body* body = m_world->GetBodyList())
		return reinterpret_cast<Body*>(body->GetUserData());

	return nullptr;
}

void UIScene::removeBody(Body* body)
{
	m_bodiesToRemove.push_back(body);

	if(m_camera.get() == body)
		m_camera.set(nullptr);
}

void UIScene::removeJoint(b2Joint* joint)
{
	m_world->DestroyJoint(joint);
}

void UIScene::queryAABB(const Point& from, const Point& to)
{
    b2AABB query;
    query.lowerBound = b2Vec2(from.x / BOX2D_SCALE, from.y / BOX2D_SCALE);
    query.upperBound = b2Vec2(to.x / BOX2D_SCALE, to.y / BOX2D_SCALE);

    m_fixtures.clear();
    m_bodies.clear();

    m_world->QueryAABB(this, query);
    std::unique(m_bodies.begin(), m_bodies.end());
}

void UIScene::rayCast(const Point& from, const Point& to)
{
    m_fixtures.clear();
    m_bodies.clear();

    m_world->RayCast(this, b2Vec2(from.x / BOX2D_SCALE, from.y / BOX2D_SCALE), b2Vec2(to.x / BOX2D_SCALE, to.y / BOX2D_SCALE));
    std::unique(m_bodies.begin(), m_bodies.end());
}

void UIScene::setGravity(const Point& point)
{
	m_world->SetGravity(b2Vec2(point.x / BOX2D_SCALE, point.y / BOX2D_SCALE));
}

Point UIScene::getGravity() const
{
	const b2Vec2 pos = m_world->GetGravity();
	return {(int32_t)(pos.x * BOX2D_SCALE), (int32_t)(pos.y * BOX2D_SCALE)};
}

float32 UIScene::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	m_fixtures.push_back(reinterpret_cast<Fixture*>(fixture->GetUserData()));
	m_bodies.push_back(reinterpret_cast<Fixture*>(fixture->GetUserData())->getBody());
	return fraction;
}

bool UIScene::ReportFixture(b2Fixture* fixture)
{
	m_fixtures.push_back(reinterpret_cast<Fixture*>(fixture->GetUserData()));
	m_bodies.push_back(reinterpret_cast<Fixture*>(fixture->GetUserData())->getBody());
	return true;
}

void UIScene::BeginContact(b2Contact* contact)
{
	Fixture* fixtureA = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData());
	Fixture* fixtureB = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData());

	g_lua["onBeginContact"].call(fixtureA, fixtureB);
}

void UIScene::EndContact(b2Contact* contact)
{
	Fixture* fixtureA = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData());
	Fixture* fixtureB = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData());

	g_lua["onEndContact"].call(fixtureA, fixtureB);
}

void UIScene::PreSolve(b2Contact* contact, const b2Manifold*)
{
	Fixture* fixtureA = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData());
	Fixture* fixtureB = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData());

    bool enabled = g_lua["onPreSolve"].call<bool>(fixtureA, fixtureB);
	if(!enabled || fixtureA->isPhantom() || fixtureB->isPhantom())
		contact->SetEnabled(false);
	else
		contact->SetEnabled(true);
}

void UIScene::PostSolve(b2Contact* contact, const b2ContactImpulse*)
{
	Fixture* fixtureA = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData());
	Fixture* fixtureB = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData());

	g_lua["onPostSolve"].call(fixtureA, fixtureB);
}

void UIScene::SayGoodbye(b2Joint*) {}; // useless
void UIScene::SayGoodbye(b2Fixture* fixture)
{
	delete reinterpret_cast<Fixture*>(fixture->GetUserData());
}
