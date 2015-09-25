#ifndef __PAINTER_H__
#define __PAINTER_H__

#include "framework/graphic/graphic.h"
#include <stdex/vertices.h>

class Painter
{
    public:
        Painter();

        void init();
        void draw();
        void clear();

        void setImage(const Image* image, uint8_t id = 0);
        void setVertex(const Rect& rect, const Rect& border);
        void setVertex(const Rect& rect);
        void setVertex(const stdex::Vertices& vertices);
        void setCoord(const Image* image, const Rect& rect, const Rect& border);
        void setCoord(const Image* image, const Rect& rect);
        void setCoord(const stdex::Vertices& coords);
        void setShift(float x, float y);
        void setRotation();
        void setRotation(const Rect& rect, float angle = 0);
        void setRotation(const Point& point, float angle = 0);
        void setPosition(const Point& point);
        void scale(float x, float y);
        void setDrawArea(int32_t width, int32_t height);
        void setDrawArea(const Rect& rect);
        void resetDrawArea();

        void setColor(const Color& color, uint8_t opacity = 0xFF);

        void setHorizontalMirror(bool value) { m_horizontalMirror = value; }
        void setVerticalMirror(bool value)   { m_verticalMirror = value; }

        void useShader(Shader* shader = nullptr);
        Shader* getShader() const { return m_shader; }

        void setDrawMode(Shader::DrawMode mode) { m_mode = mode; }
        Shader::DrawMode getDrawMode() const { return m_mode; }

    private:
        Shader* m_defaultShader;
        Shader* m_shader;

        bool m_horizontalMirror, m_verticalMirror;

        Shader::DrawMode m_mode;
        int32_t m_vertexSize;
        Rect m_drawArea;
};
extern Painter g_painter;

#endif // __PAINTER_H__ //
