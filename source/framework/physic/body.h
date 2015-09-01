#ifndef __UIBODY_H__
#define __UIBODY_H__

#include "framework/core/platform.h"
#include "framework/graphic/graphic.h"
#include "framework/ui/uiroot.h"

#include "framework/lua/lua.h"
#include <Box2D/Box2D.h>

class Fixture;
class Body : public UIRoot
{
	public:
		Body(UIScene* scene);
		virtual ~Body();

		virtual void draw(const Point& offset, float scale);

		void setImage(Image* image);
		Image* getImage() const { return m_image; };

		void removeFixture(Fixture* fixture);
		Fixture* getFixture() const;
		b2Body* getB2Body() const { return m_body; }
		b2World* getB2World() const;
		Body* getNext() const;

		void setType(uint8_t type);
        void setPosition(const Point& pos);
        void setRotation(uint16_t angle);
        void setLinearVelocity(const Point& pos);
        void setAngularVelocity(uint32_t omega);
        void setGravityScale(int32_t scale);
        void setLinearDamping(int32_t linearDamping);
        void setAngularDamping(int32_t angularDamping);
        void setBullet(bool bullet);
        void setFixedRotation(bool fixedRotation);

        UIScene* getScene() const;
        uint8_t  getType() const;
        int32_t  getMass() const;
        int32_t  getInertia() const;
		Point    getPosition() const;
		uint16_t getRotation() const;
        Point    getLinearVelocity() const;
        uint32_t getAngularVelocity() const;
        int32_t  getGravityScale() const;
        int32_t  getLinearDamping() const;
        int32_t  getAngularDamping() const;
        Point    getWorldCenter() const;
        Point    getLocalCenter() const;
        bool     isBullet() const;
        bool     isFixedRotation() const;

        void resetMassData();
        void applyForce(const Point& force, const Point& point);
        void applyForceToCenter(const Point& force);
        void applyTorque(int32_t torque);
        void applyLinearImpulse(const Point& impuls, const Point& point);
        void applyAngularImpulse(int32_t impuls);

        void setColor(const Color& color)         { m_color = color; }
        Color getColor()                    const { return m_color; }
        void setOpacity(uint8_t opacity)          { m_opacity = opacity; }
        uint8_t getOpacity()                const { return m_opacity; }
        void setShader(Shader* shader)            { m_shader = shader; }
        Shader* getShader()                 const { return m_shader; }
        void setVerticalMirror(bool value)        { m_mirror.set(WidgetFlags::VerticalMirror, value); }
        bool isHorizontalMirror()           const { return m_mirror.has(WidgetFlags::HorizontalMirror); }
        void setHorizontalMirror(bool value)      { m_mirror.set(WidgetFlags::HorizontalMirror, value); }
        bool isVerticalMirror()             const { return m_mirror.has(WidgetFlags::VerticalMirror); }

	private:
		b2Body* m_body;
		Image* m_image;
		Color m_color;
		uint8_t m_opacity;
		Shader* m_shader;
        stdex::flag<uint8_t> m_mirror;

	public:
        void addCondition(UICondition* condition)
        {
        	removeCondition(condition->getType());
        	m_conditions.push_back(condition);
        }

        void removeCondition(uint8_t type)
        {
        	for(auto it = m_conditions.begin(); it != m_conditions.end(); it++)
        	{
        		if((*it)->getType() == type)
        		{
        			m_conditions.erase(it);
        			return;
        		}
        	}
        }

	private:
        std::vector<UICondition*> m_conditions;
};


#endif // __UIBODY_H__ //
