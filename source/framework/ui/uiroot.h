#ifndef __UIROOT_H__
#define __UIROOT_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

// Basic shared functions

class UICondition;
class UIRoot : public Lua::Shared
{
	public:
		UIRoot() {};
		virtual ~UIRoot() {};

        virtual void draw(const Point&, float) = 0;

        virtual void setPosition(const Point&) = 0;
        virtual Point getPosition() const = 0;
        virtual void setRotation(uint16_t) = 0;
        virtual uint16_t getRotation() const = 0;

        virtual void setColor(const Color&) = 0;
        virtual Color getColor() const = 0;
        virtual void setOpacity(uint8_t) = 0;
        virtual uint8_t getOpacity() const = 0;

        virtual void addCondition(UICondition*) = 0;
        virtual void removeCondition(uint8_t) = 0;
};

#endif // __UIROOT_H__ //
