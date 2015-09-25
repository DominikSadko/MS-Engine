#include "framework/graphic/painter.h"

Painter g_painter = Painter();

Painter::Painter()
{
    m_defaultShader = nullptr;
    m_shader = nullptr;

    m_vertexSize = 1;

    m_horizontalMirror = false;
    m_verticalMirror = false;

    m_mode = Shader::Triangle;
}

void Painter::init()
{
    m_defaultShader = new Shader("assets/data/shaders/default.vert", "assets/data/shaders/default.frag");
    resetDrawArea();
}

void Painter::draw()
{
    glScissor(m_drawArea.x, m_drawArea.y, m_drawArea.w, m_drawArea.h);
    m_shader->draw(m_mode, m_vertexSize);
}

void Painter::clear()
{
    Shader::clear();
    m_shader = nullptr;
    m_mode = Shader::Triangle;
}

void Painter::useShader(Shader* shader/* = nullptr*/)
{
    if(!m_shader || shader)
    {
        m_shader = shader ? shader : m_defaultShader;
        m_shader->use();
        m_shader->setProjection(g_graphic->getDrawDimension().w, g_graphic->getDrawDimension().h);
    }
}

void Painter::setImage(const Image* image, uint8_t id /*= 0*/)
{
    m_shader->setImage(image, id);
}

void Painter::setVertex(const Rect& rect, const Rect& border)
{
    m_vertexSize = 1;

    static float* vertice = new float[12];
    vertice[0] = (rect.x + border.x);             // a
    vertice[1] = (rect.y + border.y);
    vertice[2] = (rect.x + border.x);             // b
    vertice[3] = (rect.y + rect.h - border.h);
    vertice[4] = (rect.x + rect.w - border.w);    // c
    vertice[5] = (rect.y + border.y);
    vertice[6] = (rect.x + border.x);             // d
    vertice[7] = (rect.y + rect.h - border.h);
    vertice[8] = (rect.x + rect.w - border.w);    // e
    vertice[9] = (rect.y + rect.h - border.h);
    vertice[10] = (rect.x + rect.w - border.w);   // f
    vertice[11] = (rect.y + border.y);

    m_shader->setVertexPosition(vertice);
}

void Painter::setVertex(const Rect& rect)
{
    setVertex(rect, {0, 0, 0, 0});
}

void Painter::setVertex(const stdex::Vertices& vertices)
{
    m_shader->setVertexPosition((float*)(&vertices.data[0]));
    m_vertexSize = vertices.size;
}

void Painter::setCoord(const Image* image, const Rect& rect, const Rect& border)
{
    float x = ((float)(rect.x + border.x)          / (float)image->getTextureWidth()),
          w = ((float)(rect.x + rect.w - border.w) / (float)image->getTextureHeight()),
          y = ((float)(rect.y + border.y)          / (float)image->getTextureHeight()),
          h = ((float)(rect.y + rect.h - border.h) / (float)image->getTextureWidth());

    static float* coord = new float[12];
    coord[0] = x;  // a
    coord[1] = y;
    coord[2] = x;  // b
    coord[3] = h;
    coord[4] = w;  // c
    coord[5] = y;
    coord[6] = x;  // d
    coord[7] = h;
    coord[8] = w;  // e
    coord[9] = h;
    coord[10] = w; // f
    coord[11] = y;

    m_shader->setCoordsPosition(coord);
}

void Painter::setCoord(const Image* image, const Rect& rect)
{
    setCoord(image, rect, {0, 0, 0, 0});
}

void Painter::setCoord(const stdex::Vertices& coords)
{
    m_shader->setCoordsPosition((float*)(&coords.data[0]));
}

void Painter::setRotation()
{
    m_shader->setTranslation(-g_graphic->getWindowWidth() / 2, -g_graphic->getWindowHeight() / 2);
    m_shader->setMatrix(0, m_verticalMirror, m_horizontalMirror);
    m_shader->setTranslation( g_graphic->getWindowWidth() / 2,  g_graphic->getWindowHeight() / 2);
}

void Painter::setRotation(const Rect& rect, float angle/* = 0*/)
{
    m_shader->setTranslation(-(rect.x + rect.w / 2), -(rect.y + rect.h / 2));
    m_shader->setMatrix((float)(angle / 180.f * PI), m_verticalMirror, m_horizontalMirror);
    m_shader->setTranslation(  rect.x + rect.w / 2,    rect.y + rect.h / 2);
}

void Painter::setRotation(const Point& point, float angle/* = 0*/)
{
    m_shader->setTranslation(-point.x, -point.y);
    m_shader->setMatrix((float)(angle / 180.f * PI), m_verticalMirror, m_horizontalMirror);
    m_shader->setTranslation( point.x,  point.y);
}

void Painter::setPosition(const Point& point)
{
    m_shader->setPosition(point);
}

void Painter::scale(float x, float y)
{
    m_shader->scale(x, y);
}

void Painter::setDrawArea(int32_t width, int32_t height)
{
	setDrawArea({0, 0, width, height});
}

void Painter::setDrawArea(const Rect& rect)
{
	Rect dimension = g_graphic->getDrawDimension();

    m_drawArea = {rect.x * g_graphic->getWindowWidth() / dimension.w,
                 g_graphic->getWindowHeight() - (rect.y + rect.h) * g_graphic->getWindowHeight() / dimension.h,
    		     rect.w * g_graphic->getWindowWidth()  / dimension.w,
    		     rect.h * g_graphic->getWindowHeight() / dimension.h};
}

void Painter::resetDrawArea()
{
	setDrawArea({0, 0, g_graphic->getWindowWidth(), g_graphic->getWindowHeight()});
}

void Painter::setColor(const Color& color, uint8_t opacity /*= 0xFF*/)
{
    m_shader->setColor(color, opacity);
}
