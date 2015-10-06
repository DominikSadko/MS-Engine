#include "framework/core/platform.h"
#include "framework/core/filemanager.h"
#include "framework/ui/uiwidget.h"
#include "framework/scheduler/scheduler.h"
#include "framework/lua/lua.h"

Platform g_platform;
Core g_core;

std::string Platform::className(std::string prettyFunction)
{
    size_t pos = prettyFunction.find("(");
    if(pos == std::string::npos)
        return prettyFunction;           // something is not right

    prettyFunction.erase(pos);           // cut away signature
    pos = prettyFunction.rfind(" ");
    if(pos == std::string::npos)
        return prettyFunction;           // something is not right

    prettyFunction.erase(0, pos + 1);    // cut away return type
    return prettyFunction;
}

void Platform::openWebsite(const std::string url)
{
    g_core.openWebsite(url);
}

std::string Platform::GetIpAddr()
{
	return g_core.GetIpAddr();
}

void Platform::ShowAdMob(bool test, const std::string& unitId)
{
    g_core.ShowAdMob(test, unitId);
}

void Platform::HideAdMob()
{
    g_core.HideAdMob();
}

std::string Platform::getPackagePath()
{
    return g_core.getPackagePath();
}

std::string Platform::getCacheDir()
{
    return g_core.getCacheDir();
}



void Platform::init()
{
	m_done = false;

    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        LOGE("Unable to initialize SDL");
        return;
    }

    SDLNet_Init();

    m_rootWidget = new UIWidget();
    m_rootWidget->setPhantom(true);
    m_rootWidget->setId("RootWidget");

    m_focusWidget = nullptr;
    m_grabWidget = nullptr;
    m_hoveredWidget = nullptr;

    for(uint8_t i = 0; i < 6; i++)
    {
        m_mousePressed[i] = false;
        m_mousePosition[i] = {0, 0};
        m_mouseLastPosition[i] = {0, 0};
        m_mouseClickPosition[i] = {0, 0};
    }

    m_offsetPosition = {0, 0};

    g_core.init();

}

void Platform::terminate()
{
	if(m_rootWidget)
		m_rootWidget->remove();

	SDL_Quit();
}

void Platform::loop()
{
    SDL_Event event;
    while(!m_done)
    {
    	Scheduler::Poll();

        while(!m_done && SDL_PollEvent(&event))
        {
            switch(event.type)
            {
#if defined(WIN32)
                // mouse
                case SDL_MOUSEMOTION:
                    onMouseMove(1, {event.motion.x * g_graphic->getDrawDimension().w / g_graphic->getWindowWidth(),
                            		event.motion.y * g_graphic->getDrawDimension().h / g_graphic->getWindowHeight() });
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                {
                	Point point;
                	point.x = event.motion.x * g_graphic->getDrawDimension().w / g_graphic->getWindowWidth();
                	point.y = event.motion.y * g_graphic->getDrawDimension().h / g_graphic->getWindowHeight();
                    onMousePress(event.button.button, point, event.button.state);
                    break;
                }

                // finger
#elif defined(ANDROID)
                case SDL_FINGERMOTION:
                {
                    Point pos = {(int32_t)(event.tfinger.x * g_graphic->getDrawDimension().w),
                                 (int32_t)(event.tfinger.y * g_graphic->getDrawDimension().h)};
                    onMouseMove(1 + event.tfinger.fingerId, pos);
                    break;
                }
                case SDL_FINGERDOWN:
                case SDL_FINGERUP:
                {
                    Point pos = {(int32_t)(event.tfinger.x * g_graphic->getDrawDimension().w),
                                 (int32_t)(event.tfinger.y * g_graphic->getDrawDimension().h)};
                    onMousePress(1 + event.tfinger.fingerId, pos, event.type == SDL_FINGERDOWN);
                    break;
                }
#endif
                case SDL_KEYDOWN:
                	onKeyPress((uint16_t)event.key.keysym.sym, true);
                    break;
                case SDL_KEYUP:
                	onKeyPress((uint16_t)event.key.keysym.sym, false);
                    break;
                case SDL_DROPFILE:
                {
                	std::string path = g_fileManager.resolvePath(event.drop.file);
                	onDragFile(path);

                	SDL_free(event.drop.file);
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    switch(event.window.event)
                    {
                    	case SDL_WINDOWEVENT_FOCUS_GAINED:
                    		m_hasWindowFocus = true;
                            break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                    		m_hasWindowFocus = false;
                            break;

                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        {
                            g_graphic->init(event.window.data1, event.window.data2);
                            g_painter.resetDrawArea();
                            g_graphic->render();

                            g_lua["onWindowResize"].call(event.window.data1, event.window.data2);
                            break;
                        }
                    }

                    break;
                }
                case SDL_APP_LOWMEMORY:
                	LOGI("Low Memory");
                case SDL_APP_TERMINATING:
                	LOGI("Terminating");
                case SDL_QUIT:
                	LOGI("Quit");
                	m_done = true;
                	break;
            }
        }

        if(!m_done)
        	g_graphic->render();
    }
}

void Platform::render()
{
    if(!m_rootWidget)
    	return;

    m_rootWidget->updateAnchors();
    m_rootWidget->draw({0, 0}, 1.f);
}

void Platform::setFocusedWidget(UIWidget* widget)
{
    if(m_focusWidget == widget)
        return;

    if(m_focusWidget)
    {
        g_lua["onFocusChange"].call(m_focusWidget, false);
        m_focusWidget->onLayout("!focus");
    }

    m_focusWidget = widget;

    if(m_focusWidget)
    {
        g_lua["onFocusChange"].call(m_focusWidget, true);
        m_focusWidget->onLayout("@focus");
    }
}

void Platform::setGrabbedWidget(UIWidget* widget)
{
    if(m_grabWidget)
    	m_grabWidget->onLayout("!grab");

	m_grabWidget = widget;

    if(m_grabWidget)
    	m_grabWidget->onLayout("@grab");
}

void Platform::onMouseMove(uint8_t id, const Point& position)
{
	Point fromPosition      = m_mousePosition[id];
	m_mouseLastPosition[id] = m_mousePosition[id];
    m_mousePosition[id]     = position;

    if(m_grabWidget)
        m_grabWidget->setPosition({m_mousePosition[id].x - m_mouseClickPosition[id].x + m_offsetPosition.x,
        	                       m_mousePosition[id].y - m_mouseClickPosition[id].y + m_offsetPosition.y});
    else
    {
        UIWidget* toWidget = m_rootWidget->getChildByPoint(m_mousePosition[id]);
        UIWidget* fromWidget = m_rootWidget->getChildByPoint(fromPosition);

        if(toWidget != fromWidget)
        {
            if(toWidget)
                toWidget->onMouseHover(id, true);

            if(fromWidget)
                fromWidget->onMouseHover(id, false);
        }

        if(toWidget)
        {
            Rect rect = toWidget->getVirtualRect();
            Point clickPosition = {m_mousePosition[id].x - rect.x, m_mousePosition[id].y - rect.y};
            g_lua["onMouseMove"].call(toWidget, m_mousePressed[id], id, clickPosition);
        }
    }
}

void Platform::onMousePress(uint8_t id, const Point& position, bool executed)
{
	m_mouseLastPosition[id] = m_mousePosition[id];
    m_mousePosition[id]     = position;
    m_mousePressed[id]      = executed;

    if(executed)
    	m_mouseClickPosition[id] = position;

    if(UIWidget* widget = m_rootWidget->getChildByPoint(m_mousePosition[id]))
    {
        Rect rect = widget->getVirtualRect();
        Point clickPosition = {m_mousePosition[id].x - rect.x, m_mousePosition[id].y - rect.y};

        if(executed)
        {
            widget->focus();

            if(m_grabWidget != widget && widget->grab())
                m_offsetPosition = widget->getPosition();
        }

        if(m_grabWidget && widget != m_grabWidget)
        	m_grabWidget->onMousePress(id, clickPosition, executed);
        else
        	widget->onMousePress(id, clickPosition, executed);
    }

    if(!executed && m_grabWidget)
        m_grabWidget = nullptr;
}

bool Platform::isKeyPressed(uint16_t key) const
{
	const auto it = m_keyState.find(key);
	return it != m_keyState.end() ? it->second : false;
}

void Platform::onKeyPress(uint16_t key, bool executed)
{
	if(executed != m_keyState[key])
	{
		m_keyState[key] = executed;
		g_lua["onKeyPress"].call(key, executed);
	}
}

void Platform::onDragFile(const std::string& path)
{
    UIWidget* widget = m_rootWidget->getChildByPoint(m_mousePosition[1]);
    g_lua["onDragFile"].call(path, widget);
}

Rect Platform::getRootRect() const { return m_rootWidget->getRect(); }
