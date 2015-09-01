#include "framework/ui/uitimer.h"

UITimer::UITimer()
{
    setMetatable("UITimer");
	m_startTime = 0;
}

UITimer::~UITimer()
{
}

void UITimer::remove()
{
	UIWidget::remove();
}

void UITimer::draw(const Point& offset, float scale)
{
	uint32_t ticks = getTicks() - m_startTime;
	uint32_t ms = ticks - (ticks / 1000 * 1000);
	uint32_t seconds = (ticks / 1000) % 60;
	uint32_t minutes = (ticks / 1000 / 60) % 60;

	std::string text;
	if(minutes < 10)
		text += stdex::toString("0", minutes);
	else
		text += stdex::toString(minutes);

	if(seconds < 10)
		text += stdex::toString(":0", seconds);
	else
		text += stdex::toString(":", seconds);

	if(ms < 10)
		text += stdex::toString(":00", ms);
	else if(ms < 100)
		text += stdex::toString(":0", ms);
	else
		text += stdex::toString(":", ms);

	setText(text);
	UIWidget::draw(offset, scale);
}
