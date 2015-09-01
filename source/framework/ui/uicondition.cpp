#include "framework/ui/uicondition.h"
#include "framework/physic/uiscene.h"
#include "framework/physic/body.h"

void UICondition::Acceleration(Body* t, const Point& maxForce, const Point& force)
{
	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		Point velocity = t->getLinearVelocity();
		if(maxForce.x)
			velocity.x = std::min(maxForce.x, velocity.x + force.x);

		if(maxForce.y)
			velocity.y = std::min(maxForce.y, velocity.y + force.y);

		t->setLinearVelocity(velocity);
		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::Acceleration, fn);
	t->addCondition(condition);
}

void UICondition::Graviton(Body* t, UIScene* scene, int32_t distance, int32_t strength)
{
	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
        b2Body* body = t->getB2Body();

        Point pos = t->getPosition();
        scene->queryAABB({pos.x - distance / 2, pos.y - distance / 2},
				         {pos.x + distance / 2, pos.y + distance / 2});

		for(auto& target : scene->m_bodies)
		{
			if(t == target || target->getType() != b2BodyType::b2_dynamicBody)
				continue;

            int32_t distance = stdex::toDistance(pos, target->getPosition());
			if(distance > distance)
				continue;

            b2Body* targetBody = target->getB2Body();
            b2Vec2 d = targetBody->GetWorldCenter() - body->GetWorldCenter();
            b2Vec2 force = -strength / 100.f / (distance / BOX2D_SCALE) / std::sqrt(distance / BOX2D_SCALE) *
            		        (body->GetMass() ? body->GetMass() : 1.f) * (targetBody->GetMass() ? targetBody->GetMass() : 1.f) * d;

            targetBody->ApplyForceToCenter(force, true);
        }

		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::Graviton, fn);
	t->addCondition(condition);
}
