#include "framework/scheduler/scheduler.h"

std::vector<Scheduler*> Scheduler::SchedulersList;
Scheduler* g_scheduler = new Scheduler();

void Task::execute()
{
	if(m_callBack)
	{
		(*m_callBack)();
		delete this;
	}
}

Task::~Task()
{
    if(!Lua::remove(g_lua.state(), m_callBack))
        delete m_callBack;
};

Scheduler::Scheduler()
{
    m_autoId = 0;
    m_paused = 0;

    SchedulersList.push_back(this);
}

Scheduler::~Scheduler()
{
	while(!m_tasks.empty())
	{
		delete m_tasks.top();
		m_tasks.pop();
	}

	auto it = std::find(SchedulersList.begin(), SchedulersList.end(), this);
	if(it != SchedulersList.end())
		SchedulersList.erase(it);
}

void Scheduler::Poll()
{
	for(auto it : SchedulersList)
		it->poll();
}

void Scheduler::poll()
{
	if(isPaused())
		return;

    uint32_t tick = getTicks();
    Task* task = nullptr;

    while(!m_tasks.empty() && m_tasks.top()->getInterval() <= tick)
    {
        task = m_tasks.top();
        m_tasks.pop();

		auto it = std::find(m_eventIds.begin(), m_eventIds.end(), task->getEventId());
		if(it == m_eventIds.end())
		{
			delete task;
	        continue;
		}

		m_eventIds.erase(it);
        task->execute();
    }
}

int32_t Scheduler::addEvent(std::function<void()>* callBack)
{
	m_eventIds.push_back(getEventId());
    m_tasks.push(new Task(callBack, getTicks() +  1, m_autoId));

    return m_autoId;
}

int32_t Scheduler::scheduleEvent(std::function<void()>* callBack, uint32_t sleep)
{
	m_eventIds.push_back(getEventId());
    m_tasks.push(new Task(callBack, getTicks() +  std::max((uint32_t)1, sleep), m_autoId));

    return m_autoId;
}

uint8_t Scheduler::stopEvent(int32_t event)
{
	if(event)
	{
		auto it = std::find(m_eventIds.begin(), m_eventIds.end(), event);
		if(it != m_eventIds.end())
			m_eventIds.erase(it);
	}

    return 0;
}

