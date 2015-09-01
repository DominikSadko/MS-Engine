#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

#include <stdex/vector_queue.h>

class Task
{
    public:
        Task(std::function<void()>* callBack, uint32_t interval, int32_t eventId)
        {
            m_interval = interval;
            m_callBack = callBack;
            m_eventId  = eventId;
        }

        ~Task();

        void execute();
        void addInterval(uint32_t tick) { m_interval += tick; }

        int32_t getEventId()   const { return m_eventId; }
        uint32_t getInterval() const { return m_interval; }
        bool operator<(const Task& other) const { return m_interval > other.m_interval; }

    private:
        int32_t m_eventId;
        uint32_t m_interval;
        std::function<void()>* m_callBack;
};

class lessSchedulerTask : public std::binary_function<Task*&, Task*&, bool>
{
    public:
        bool operator()(Task*& t1, Task*& t2) { return (*t1) < (*t2); }
};

class Scheduler : public Lua::Shared
{
    public:
        Scheduler();
        virtual ~Scheduler();

        int32_t getEventId()
        {
            m_autoId++;
            if(m_autoId == 0)
                m_autoId++;

            return m_autoId;
        }

        void poll();

        int32_t addEvent(std::function<void()>* callBack);
        int32_t scheduleEvent(std::function<void()>* callBack, uint32_t sleep);
        uint8_t stopEvent(int32_t event);

        void pause() { m_paused = getTicks(); }
        void resume()
        {
            auto elements = m_tasks.elements();
            uint32_t tick = getTicks() - m_paused;
            for(auto it : elements)
                it->addInterval(tick);

            m_paused = 0;
        }

        bool isPaused() const { return m_paused > 0; }

    private:
        int32_t m_autoId;
        stdex::vector_queue<Task*, lessSchedulerTask> m_tasks;
        std::vector<int32_t> m_eventIds;
        uint32_t m_paused;

    public:
        static void Poll();

    private:
        static std::vector<Scheduler*> SchedulersList;
};
extern Scheduler* g_scheduler;

#endif
