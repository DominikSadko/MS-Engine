#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"
#include "framework/physic/fixture.h"
#include "framework/physic/joint.h"

// todo b2Rope

Body::Body(UIScene* scene)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_staticBody;
    bodyDef.userData = this;

	m_body = scene->getWorld()->CreateBody(&bodyDef);
	m_image = nullptr;
	m_color = {0xFF, 0xFF, 0xFF, 0xFF};
	m_opacity = 255;
	m_shader = nullptr;

	setHorizontalMirror(false);
	setVerticalMirror(false);
}

Body::~Body()
{
	// Destroy m_body in uiscene
	for(auto condition : m_conditions)
		delete condition;

	m_conditions.clear();
}

void Body::draw(const Point& offset, float scale)
{
	if(!m_image)
		return;

	for(auto it = m_conditions.begin(); it != m_conditions.end(); it++)
	{
		if(!(*it)->onThink())
		{
			m_conditions.erase(it);
			break;
		}
	}

	Point pos = getPosition();
	pos.x = (pos.x * scale + offset.x);
	pos.y = (pos.y * scale + offset.y);

    if(isHorizontalMirror())
        g_painter.setHorizontalMirror(true);

    if(isVerticalMirror())
        g_painter.setVerticalMirror(true);

    g_painter.useShader(m_shader);
    g_painter.setDrawMode(Shader::TriangleFan);
    g_painter.setImage(m_image);
    g_painter.setColor(m_color, m_opacity);
	g_painter.setPosition(pos);
	g_painter.scale(scale, scale);
	g_painter.setRotation(pos, getRotation());

	for(Fixture* fixture = getFixture(); fixture; fixture = fixture->getNext())
	{
		if(fixture->m_vertices.size && fixture->m_coords.size())
		{
			if(fixture->m_patterns.empty())
			{
				g_painter.setVertex(fixture->m_vertices);
				g_painter.setCoord(fixture->m_coords[0]);
				g_painter.draw();
			}
			else
			{
				const Pattern& pattern = fixture->m_patterns[fixture->m_pattern - 1];
				uint32_t coord = 0;

				if(pattern.animations > 0)
					coord = (getTicks() / pattern.time) % pattern.animations;

				g_painter.setVertex(fixture->m_vertices);
				g_painter.setCoord(fixture->m_coords[pattern.offset + coord]);
				g_painter.draw();
			}
		}
	}

    if(isHorizontalMirror())
        g_painter.setHorizontalMirror(false);

    if(isVerticalMirror())
        g_painter.setVerticalMirror(false);

    g_painter.setPosition({0, 0});
    g_painter.clear();
}

void Body::setImage(Image* image)
{
	m_image = image;

	for(Fixture* fixture = getFixture(); fixture; fixture = fixture->getNext())
		fixture->updateImageCoords(m_image);
}

void Body::removeFixture(Fixture* fixture)
{
	m_body->DestroyFixture(fixture->getB2Fixture());
}

Fixture* Body::getFixture() const
{
	if(m_body->GetFixtureList())
		return reinterpret_cast<Fixture*>(m_body->GetFixtureList()->GetUserData());

	return nullptr;
}

b2World* Body::getB2World() const
{
	return m_body->GetWorld();
}

Body* Body::getNext() const
{
	if(m_body->GetNext())
		return reinterpret_cast<Body*>(m_body->GetNext()->GetUserData());

	return nullptr;
}

void Body::setType(uint8_t type)
{
	m_body->SetType((b2BodyType)type);
}

void Body::setPosition(const Point& pos)
{
	m_body->SetTransform(b2Vec2(pos.x / BOX2D_SCALE, pos.y / BOX2D_SCALE), m_body->GetAngle());
}

void Body::setRotation(uint16_t angle)
{
	m_body->SetTransform(m_body->GetPosition(), (angle % 360) * PI / 180.f);
}

void Body::setLinearVelocity(const Point& pos)
{
	m_body->SetLinearVelocity(b2Vec2(pos.x / BOX2D_SCALE, pos.y / BOX2D_SCALE));
}

void Body::setAngularVelocity(uint32_t omega)
{
	m_body->SetAngularVelocity(omega);
}

void Body::setGravityScale(int32_t scale)
{
	m_body->SetGravityScale(scale / 100.f);
}

void Body::setLinearDamping(int32_t linearDamping)
{
	m_body->SetLinearDamping(linearDamping / 100.f);
}

void Body::setAngularDamping(int32_t angularDamping)
{
	m_body->SetAngularDamping(angularDamping / 100.f);
}

void Body::setBullet(bool bullet)
{
	m_body->SetBullet(bullet);
}

void Body::setFixedRotation(bool fixedRotation)
{
	m_body->SetFixedRotation(fixedRotation);
}

UIScene* Body::getScene() const
{
	return reinterpret_cast<UIScene*>(m_body->GetWorld());
}

uint8_t Body::getType() const
{
	return (uint8_t)m_body->GetType();
}

int32_t Body::getMass() const
{
	return (uint32_t)m_body->GetMass() * 100;
}

int32_t Body::getInertia() const
{
	return (uint32_t)m_body->GetInertia() * 100;
}

Point Body::getPosition() const
{
	const b2Vec2 vec = m_body->GetPosition();
	return { (int32_t)(vec.x * BOX2D_SCALE), (int32_t)(vec.y * BOX2D_SCALE) };
}

uint16_t Body::getRotation() const
{
	return (uint16_t)(m_body->GetAngle() * 180.f / PI);
}

Point Body::getLinearVelocity() const
{
	const b2Vec2 pos = m_body->GetLinearVelocity();
	return {(int32_t)(pos.x * BOX2D_SCALE), (int32_t)(pos.y * BOX2D_SCALE)};
}

uint32_t Body::getAngularVelocity() const
{
	return m_body->GetAngularVelocity();
}

int32_t Body::getGravityScale() const
{
	return (int32_t)(m_body->GetGravityScale() * 100);
}

int32_t Body::getLinearDamping() const
{
	return (int32_t)(m_body->GetLinearDamping() * 100);
}

int32_t Body::getAngularDamping() const
{
	return (int32_t)(m_body->GetAngularDamping() * 100);
}

Point Body::getWorldCenter() const
{
	const b2Vec2 pos = m_body->GetWorldCenter();
	return {(int32_t)(pos.x * BOX2D_SCALE), (int32_t)(pos.y * BOX2D_SCALE)};
}

Point Body::getLocalCenter() const
{
	const b2Vec2 pos = m_body->GetLocalCenter();
	return {(int32_t)(pos.x * BOX2D_SCALE), (int32_t)(pos.y * BOX2D_SCALE)};
}

bool Body::isBullet() const
{
	return m_body->IsBullet();
}

bool Body::isFixedRotation() const
{
	return m_body->IsFixedRotation();
}

void Body::resetMassData()
{
	m_body->ResetMassData();
}

void Body::applyForce(const Point& force, const Point& point)
{
	m_body->ApplyForce(b2Vec2(force.x / BOX2D_SCALE, force.y / BOX2D_SCALE), b2Vec2(point.x / BOX2D_SCALE, point.y / BOX2D_SCALE), true);
}

void Body::applyForceToCenter(const Point& force)
{
	m_body->ApplyForceToCenter(b2Vec2(force.x / BOX2D_SCALE, force.y / BOX2D_SCALE), true);
}

void Body::applyTorque(int32_t torque)
{
	m_body->ApplyTorque(torque / 100.f, true);
}

void Body::applyLinearImpulse(const Point& impuls, const Point& point)
{
	m_body->ApplyLinearImpulse(b2Vec2(impuls.x / BOX2D_SCALE, impuls.y / BOX2D_SCALE), b2Vec2(point.x / BOX2D_SCALE, point.y / BOX2D_SCALE), true);
}

void Body::applyAngularImpulse(int32_t impuls)
{
	m_body->ApplyAngularImpulse(impuls / 100.f, true);
}
