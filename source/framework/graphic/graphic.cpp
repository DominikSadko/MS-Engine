#include "framework/graphic/graphic.h"
#include "framework/font/font.h"
#include "framework/ui/uiwidget.h"

Graphic* g_graphic = nullptr;

Graphic::Graphic()
{
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(0, &mode);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

    // Create our window centered
#ifdef WIN32
    mode.w = 800; mode.h = 400;
#endif

    m_window = SDL_CreateWindow(MODULE_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!m_window)
    {
        LOGE("Unable to create window: %s", SDL_GetError());
        return;
    }

    m_context = SDL_GL_CreateContext(m_window);
    if(!m_context)
    {
        LOGE("Unable to create GL context: %s", SDL_GetError());
        return;
    }

    SDL_GL_MakeCurrent(m_window, m_context);
    SDL_GL_SetSwapInterval(1);

	m_customDimension = false;
	m_drawDimension = {0, 0, mode.w, mode.h};
	m_fps = 0;

    init(mode.w, mode.h);
    Shader::init();
}

void Graphic::init(int32_t width, int32_t height)
{
    m_width = width;
    m_height = height;

    glViewport(0, 0, m_width, m_height);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(!m_customDimension)
    {
    	m_drawDimension = {0, 0, m_width, m_height};
    	g_platform.getRootWidget()->setSize(m_drawDimension);
    }
}

void Graphic::terminate()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Graphic::render()
{
    static int32_t fps = 0, lastTime = 0;

    fps++;
    if(lastTime < time(NULL))
    {
        lastTime = time(NULL);
        m_fps = fps;
        fps = 0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    g_painter.useShader();
    g_platform.render();
    g_painter.clear();

    SDL_GL_SwapWindow(m_window);
}

void Graphic::setDrawDimension(const Rect& rect)
{
	if(!rect.w || !rect.h)
	{
		m_customDimension = false;
    	m_drawDimension = {0, 0, m_width, m_height};
	}
	else
	{
		m_customDimension = true;
		m_drawDimension = rect;
	}

	g_platform.getRootWidget()->setSize(m_drawDimension);
}
