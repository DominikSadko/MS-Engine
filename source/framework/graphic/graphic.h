#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "framework/core/platform.h"
#include "framework/graphic/shader.h"
#include "framework/image/image.h"
#include "framework/graphic/painter.h"

class Graphic
{
    public:
        Graphic();
        ~Graphic();

        void init(int32_t width, int32_t height);
        void terminate();
        void render();

        void setDrawDimension(const Rect& rect);

        uint16_t getWindowWidth()  const { return m_width; }
        uint16_t getWindowHeight() const { return m_height; }
        Rect getDrawDimension()    const { return m_drawDimension; }

        uint16_t getFps() const { return m_fps; }

    private:
        Window* m_window;
        SDL_GLContext m_context;
        uint16_t m_width, m_height;

        bool m_customDimension;
        Rect m_drawDimension;

        uint16_t m_fps;
};
extern Graphic* g_graphic;

#endif // __GRAPHIC_H__ //
