#ifndef __UITIMER_H__
#define __UITIMER_H__

#include "framework/ui/uiwidget.h"

class UITimer : public UIWidget
{
	public:
		UITimer();
		virtual ~UITimer();
		virtual void remove();

		virtual void draw(const Point& offset, float scale);

		void setStartTime(uint32_t time) { m_startTime = time; }
		uint32_t getStartTime() const { return m_startTime; }

	private:
		uint32_t m_startTime;
};

#endif // __UITIMER_H__ //
