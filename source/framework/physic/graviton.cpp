#include "framework/physic/graviton.h"

#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"

GravityManager::GravityManager()
{

}

GravityManager::~GravityManager()
{

}

void GravityManager::clear()
{
	m_gravitons.clear();
}

void GravityManager::update(UIScene* scene)
{
	for(auto& graviton : m_gravitons)
	{
        b2Body* body = graviton.body->getB2Body();

        Point pos = graviton.body->getPosition();
        scene->queryAABB({pos.x - graviton.distance / 2, pos.y - graviton.distance / 2},
				         {pos.x + graviton.distance / 2, pos.y + graviton.distance / 2});

		for(auto& target : scene->m_bodies)
		{
			if(graviton.body == target || target->getType() != b2BodyType::b2_dynamicBody)
				continue;

            int32_t distance = stdex::toDistance(pos, target->getPosition());
			if(distance > graviton.distance)
				continue;

            b2Body* targetBody = target->getB2Body();
            b2Vec2 d = targetBody->GetWorldCenter() - body->GetWorldCenter();
            b2Vec2 force = -graviton.strength / 100.f / (distance / BOX2D_SCALE) / std::sqrt(distance / BOX2D_SCALE) *
            		        (body->GetMass() ? body->GetMass() : 1.f) * (targetBody->GetMass() ? targetBody->GetMass() : 1.f) * d;

            targetBody->ApplyForceToCenter(force, true);
        }
	}
}

void GravityManager::add(Body* body, int32_t distance, int32_t strength)
{
	for(auto it = m_gravitons.begin(); it != m_gravitons.end(); it++)
	{
		if(it->body == body)
		{
			it->body = body;
			it->distance = distance;
			return;
		}
	}

	Graviton_t graviton;
	graviton.body = body;
	graviton.distance = distance;
	graviton.strength = strength;

	m_gravitons.push_back(graviton);
}

void GravityManager::remove(Body* body)
{
	for(auto it = m_gravitons.begin(); it != m_gravitons.end(); it++)
	{
		if(it->body == body)
		{
			m_gravitons.erase(it);
			return;
		}
	}
}
