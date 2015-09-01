#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"
#include "framework/physic/fixture.h"
#include "framework/physic/joint.h"

// todo b2ChainShape
// todo destructor

Fixture::Fixture(b2Fixture* fixture, const stdex::Vertices& vertices)
{
	m_fixture = fixture;
	m_fixture->SetUserData(this);

	m_vertices = vertices;
	m_pattern = 1;

	setPhantom(false);
}

Fixture* Fixture::Vertice(Body* body, const stdex::Vertices& vertices)
{
	b2FixtureDef fixtureDef;
	b2PolygonShape polygonShape;

	b2Vec2 points[vertices.size];
	for(int32_t i = 0; i < vertices.size; i++)
	{
		points[i].x = vertices.data[i * 2] / BOX2D_SCALE;
		points[i].y = vertices.data[i * 2 + 1] / BOX2D_SCALE;
	}

	polygonShape.Set(points, vertices.size);
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = 0.0;
	fixtureDef.restitution = 0.0;
	fixtureDef.friction = 0.0;
	fixtureDef.isSensor = false;

	Fixture* fixture = new Fixture(body->getB2Body()->CreateFixture(&fixtureDef), vertices);
	fixture->updateImageCoords(body->getImage());
	return fixture;
}

Fixture* Fixture::Circle(Body* body, int32_t radius)
{
	stdex::Vertices vertices;
    vertices.add(-radius / 2.f);  vertices.add(-radius / 2.f);
    vertices.add( radius / 2.f);  vertices.add(-radius / 2.f);
    vertices.add( radius / 2.f);  vertices.add( radius / 2.f);
    vertices.add(-radius / 2.f);  vertices.add( radius / 2.f);
    vertices.size = 4;

	b2FixtureDef fixtureDef;
	b2CircleShape circleShape;

	circleShape.m_radius = radius / 2.f / BOX2D_SCALE;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 0.0;
	fixtureDef.restitution = 0.0;
	fixtureDef.friction = 0.0;
	fixtureDef.isSensor = false;

	Fixture* fixture = new Fixture(body->getB2Body()->CreateFixture(&fixtureDef), vertices);
	fixture->updateImageCoords(body->getImage());
	return fixture;
}

Fixture* Fixture::Edge(Body* body, const Point& from, const Point& to)
{
	stdex::Vertices vertices; vertices.clear(); // unused
	b2FixtureDef fixtureDef;
	b2EdgeShape edgeShape;

	edgeShape.Set(b2Vec2(from.x / BOX2D_SCALE, from.y / BOX2D_SCALE), b2Vec2(to.x / BOX2D_SCALE, to.y / BOX2D_SCALE) );
	fixtureDef.shape = &edgeShape;
	fixtureDef.density = 0.0;
	fixtureDef.restitution = 0.0;
	fixtureDef.friction = 0.0;
	fixtureDef.isSensor = false;

	Fixture* fixture = new Fixture(body->getB2Body()->CreateFixture(&edgeShape, 1.f), vertices);
	return fixture;
}

Fixture* Fixture::Square(Body* body, const Rect& rect)
{
	stdex::Vertices vertices;
    vertices.add(-rect.w / 2.f);  vertices.add(-rect.h / 2.f);
    vertices.add( rect.w / 2.f);  vertices.add(-rect.h / 2.f);
    vertices.add( rect.w / 2.f);  vertices.add( rect.h / 2.f);
    vertices.add(-rect.w / 2.f);  vertices.add( rect.h / 2.f);
    vertices.size = 4;

	return Fixture::Vertice(body, vertices);
}

Fixture::~Fixture()
{

}

Fixture* Fixture::getNext() const
{
	if(m_fixture->GetNext())
		return reinterpret_cast<Fixture*>(m_fixture->GetNext()->GetUserData());

	return nullptr;
}

Body* Fixture::getBody() const
{
	if(m_fixture->GetBody())
		return reinterpret_cast<Body*>(m_fixture->GetBody()->GetUserData());

	return nullptr;
}

uint8_t Fixture::getType() const
{
	return (uint8_t)m_fixture->GetType();
}

bool Fixture::isSensor() const
{
	return m_fixture->IsSensor();
}

int32_t Fixture::getDensity() const
{
	return (int32_t)(m_fixture->GetDensity() * 100);
}

int32_t Fixture::getFriction() const
{
	return (int32_t)(m_fixture->GetFriction() * 100);
}

int32_t Fixture::getRestitution() const
{
	return (int32_t)(m_fixture->GetRestitution() * 100);
}

void Fixture::setFilterData(uint16_t category, uint16_t mask)
{
	b2Filter filter;
	filter.categoryBits = category;
	filter.maskBits = mask;
	m_fixture->SetFilterData(filter);
}

void Fixture::setSensor(bool sensor)
{
	m_fixture->SetSensor(sensor);
}

void Fixture::setDensity(int32_t density)
{
	m_fixture->SetDensity(density / 100.f);
}

void Fixture::setFriction(int32_t friction)
{
	m_fixture->SetFriction(friction / 100.f);
}

void Fixture::setRestitution(int32_t restitution)
{
	m_fixture->SetRestitution(restitution / 100.f);
}

void Fixture::setPolygonVertices(const stdex::Vertices& vertices)
{
	b2Shape* shape = m_fixture->GetShape();
	if(shape->GetType() == b2Shape::e_polygon)
	{
		b2PolygonShape* polygonShape = ((b2PolygonShape*)shape);
		b2Vec2 points[vertices.size];

		for(int32_t i = 0; i < vertices.size; i++)
		{
			points[i].x = vertices.data[i * 2] / BOX2D_SCALE;
			points[i].y = vertices.data[i * 2 + 1] / BOX2D_SCALE;
		}

		m_vertices = vertices;
	    polygonShape->Set(points, vertices.size);

	    if(m_coords.size() == 0)
	    	updateImageCoords(getBody()->getImage());
    }
	else
		LOGE("Not available: %i", (int32_t)shape->GetType());
}

void Fixture::setCircleSize(int32_t radius)
{
	b2Shape* shape = m_fixture->GetShape();
	if(shape->GetType() == b2Shape::e_circle)
	{
		m_vertices.clear();

		m_vertices.add(-radius / 2.f);  m_vertices.add(-radius / 2.f);
		m_vertices.add( radius / 2.f);  m_vertices.add(-radius / 2.f);
		m_vertices.add( radius / 2.f);  m_vertices.add( radius / 2.f);
		m_vertices.add(-radius / 2.f);  m_vertices.add( radius / 2.f);
		m_vertices.size = 4;

		shape->m_radius = radius / 2.f / BOX2D_SCALE;

	    if(m_coords.size() == 0)
	    	updateImageCoords(getBody()->getImage());
    }
	else
		LOGE("Not available: %i", (int32_t)shape->GetType());
}

void Fixture::setEdgeSize(const Point& from, const Point& to)
{
	b2Shape* shape = m_fixture->GetShape();
	if(shape->GetType() == b2Shape::e_circle)
		((b2EdgeShape*)shape)->Set(b2Vec2(from.x / BOX2D_SCALE, from.y / BOX2D_SCALE), b2Vec2(to.x / BOX2D_SCALE, to.y / BOX2D_SCALE));
	else
		LOGE("Not available: %i", (int32_t)shape->GetType());
}

void Fixture::setSquareSize(const Rect& rect)
{
	b2Shape* shape = m_fixture->GetShape();
	if(shape->GetType() == b2Shape::e_polygon)
	{
		b2PolygonShape* polygonShape = ((b2PolygonShape*)shape);
		stdex::Vertices vertices;
		b2Vec2 points[vertices.size];

	    vertices.add(-rect.w / 2.f);  vertices.add(-rect.h / 2.f);
	    vertices.add( rect.w / 2.f);  vertices.add(-rect.h / 2.f);
	    vertices.add( rect.w / 2.f);  vertices.add( rect.h / 2.f);
	    vertices.add(-rect.w / 2.f);  vertices.add( rect.h / 2.f);
	    vertices.size = 4;

		for(int32_t i = 0; i < vertices.size; i++)
		{
			points[i].x = vertices.data[i * 2] / BOX2D_SCALE;
			points[i].y = vertices.data[i * 2 + 1] / BOX2D_SCALE;
		}

		m_vertices = vertices;
	    polygonShape->Set(points, vertices.size);

	    if(m_coords.size() == 0)
	    	updateImageCoords(getBody()->getImage());
    }
	else
		LOGE("Not available: %i", (int32_t)shape->GetType());
}

void Fixture::addImageCoords(const stdex::Vertices& vertices)
{
	if(vertices.size < m_vertices.size)
	{
		LOGE("Vertices needs %i size.", m_vertices.size);
		return;
	}

	Image* image = getBody()->getImage();
	if(!image)
	{
		LOGE("Image not found.");
		return;
	}

	stdex::Vertices coord;
	for(int32_t i = 0; i < m_vertices.size * 2; i += 2)
	{
		coord.add(vertices.data[i] / (float)image->getTextureWidth());
		coord.add(vertices.data[i + 1] / (float)image->getTextureHeight());
	}

	coord.size = vertices.size;
	m_coords.push_back(coord);
}

void Fixture::setImageCoords(const stdex::Vertices& vertices)
{
	m_coords.clear();
	addImageCoords(vertices);
}

void Fixture::updateImageCoords(const Image* image)
{
	m_coords.clear();

	if(!image || m_vertices.size == 0)
		return;

	stdex::Vertices coord;
	for(int32_t i = 0; i < m_vertices.size * 2; i += 2)
	{
		coord.add(m_vertices.data[i] / (float)image->getTextureWidth());
		coord.add(m_vertices.data[i + 1] / (float)image->getTextureHeight());
	}

	coord.size = m_vertices.size;
	m_coords.push_back(coord);
}

void Fixture::eraseImageCoords()
{
	m_coords.clear();
	m_patterns.clear();
}

void Fixture::addPattern(uint8_t offset, uint8_t animations, uint16_t time)
{
	Pattern pattern;
	pattern.offset = offset;
	pattern.animations = animations;
	pattern.time = time;

	m_patterns.push_back(pattern);
}

void Fixture::setPattern(uint8_t pattern)
{
	m_pattern = pattern;

	if(m_pattern < 1)
		LOGE("Pattern < 1");
}
