#ifndef __UICONDITION_H__
#define __UICONDITION_H__

#include "framework/core/platform.h"
#include "framework/ui/uiroot.h"
#include "framework/lua/lua.h"

enum UIConditionType : uint8_t
{
	None = 0,
	FadeIn = 1,
	FadeOut = 2,
	Shine = 3,
	Blink = 4,
	Move = 5,
	MoveTo = 6,
	Acceleration = 7,
	Graviton = 8,
};

class Body;
class UIScene;

class UICondition
{
	public:
		UICondition(uint8_t type, std::function<bool()>* fn, uint16_t interval = 0, uint16_t count = 0)
		{
			m_type = type;
			m_tick = 0;
			m_count = count;

			m_interval = interval;
			m_function = fn;
		}

		template<typename T>
		UICondition(T t, uint8_t type, std::function<bool()>* fn, uint16_t interval = 0, uint16_t count = 0) :
			UICondition(type, fn, interval, count)
		{
			t->addCondition(this);
		}

		~UICondition()
		{
		    if(!Lua::remove(g_lua.state(), m_function))
		        delete m_function;
		}

		uint8_t getType() const { return m_type; }

		template<typename T> static void FadeIn(T* t, int16_t opacity, uint16_t interval);
		template<typename T> static void FadeOut(T* t, int16_t opacity, uint16_t interval);
		template<typename T> static void Shine(T* t, uint16_t interval);
		template<typename T> static void Blink(T* t, uint16_t interval);
		template<typename T> static void Move(T* t, const Point& point, uint16_t interval, uint16_t count);
		template<typename T> static void MoveTo(T* t, const Point& point, uint16_t interval);
							 static void Acceleration(Body* t, const Point& maxForce, const Point& force);
							 static void Graviton(Body* t, UIScene* scene, int32_t distance, int32_t strength);

		bool onThink()
		{
			if(m_interval)
			{
				if(getTicks() / m_interval == m_tick)
					return true;

				m_tick = getTicks() / m_interval;
			}

			if(m_count)
				return --m_count > 0 && (*m_function)();
			else
				return (*m_function)();
		}

	private:
		uint8_t m_type;
		uint16_t m_tick, m_interval, m_count;
		std::function<bool()>* m_function;
};

template<typename T>
inline void UICondition::FadeIn(T* t, int16_t opacity, uint16_t interval)
{
	t->removeCondition(UIConditionType::FadeOut);
	t->removeCondition(UIConditionType::Shine);
	t->removeCondition(UIConditionType::Blink);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		t->setOpacity(std::max(0, t->getOpacity() + opacity));
		return t->getOpacity() < 0xFF;
	});

	UICondition* condition = new UICondition(UIConditionType::FadeIn, fn, interval);
	t->addCondition(condition);
}

template<typename T>
inline void UICondition::FadeOut(T* t, int16_t opacity, uint16_t interval)
{
	t->removeCondition(UIConditionType::FadeIn);
	t->removeCondition(UIConditionType::Shine);
	t->removeCondition(UIConditionType::Blink);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		t->setOpacity(std::max(0, t->getOpacity() - opacity));
		return t->getOpacity() > 0x00;
	});

	UICondition* condition = new UICondition(UIConditionType::FadeOut, fn, interval);
	t->addCondition(condition);
}

template<typename T>
inline void UICondition::Shine(T* t, uint16_t interval)
{
	t->removeCondition(UIConditionType::FadeIn);
	t->removeCondition(UIConditionType::FadeOut);
	t->removeCondition(UIConditionType::Blink);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		t->setOpacity(128 + 127 * std::sin(getTicks() / (interval / 100.f) * (PI / 180.f)));
		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::Shine, fn);
	t->addCondition(condition);
}

template<typename T>
inline void UICondition::Blink(T* t, uint16_t interval)
{
	t->removeCondition(UIConditionType::FadeIn);
	t->removeCondition(UIConditionType::FadeOut);
	t->removeCondition(UIConditionType::Shine);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		t->setOpacity(t->getOpacity() > 0 ? 0x00 : 0xFF);
		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::Blink, fn, interval);
	t->addCondition(condition);
}

template<typename T>
inline void UICondition::Move(T* t, const Point& point, uint16_t interval, uint16_t count)
{
	t->removeCondition(UIConditionType::MoveTo);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		Point pos = t->getPosition();
		pos.x += point.x;
		pos.y += point.y;

		t->setPosition(pos);
		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::Blink, fn, interval, count);
	t->addCondition(condition);
}

template<typename T>
inline void UICondition::MoveTo(T* t, const Point& point, uint16_t interval)
{
	t->removeCondition(UIConditionType::Move);

	std::function<bool()>* fn = new std::function<bool()>([=]()
	{
		Point pos = t->getPosition();
		if(point.x > 0) pos.x++; else pos.x--;
		if(point.y > 0) pos.y++; else pos.y--;

		t->setPosition(pos);
		return true;
	});

	UICondition* condition = new UICondition(UIConditionType::MoveTo, fn, interval);
	t->addCondition(condition);
}

#endif // __UICONDITION_H__ //
